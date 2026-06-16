/**************************************************************************/
/* parser.cpp                                                             */
/*                        ___  ___  ___   ___ _____                       */
/*                       / __|/ _ \|   \ / _ \_   _|                      */
/*                      | (_ | (_) | |) | (_) || |                        */
/*                       \___|\___/|___/ \___/ |_|                        */
/*   ___  ___    _ ___ ___ _____    ___ ___  __  __ ___ ___ _    ___ ___  */
/*  / _ \| _ )_ | | __/ __|_   _|  / __/ _ \|  \/  | _ \_ _| |  | __| _ \ */
/* | (_) | _ \ || | _| (__  | |   | (_| (_) | |\/| |  _/| || |__| _||   / */
/*  \___/|___/\__/|___\___| |_|    \___\___/|_|  |_|_| |___|____|___|_|_\ */
/*                                                                        */
/*              This file is part of Godot Object Compiler                */
/*                  Copyright (c) 2026 Luca Ian Tuerk                     */
/**************************************************************************/
/*                            MIT LICENCE                                 */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/
#include "parser.h"

#include "library/attribute_db.h"
#include "library/core/file_system_utilities.h"
#include "library/core/string_writer.h"
#include "library/parsers/common.h"
#include "library/parsers/tree-sitter/handlers/all.h"
#include "library/parsers/tree-sitter/node_handler.h"
#include "library/parsers/tree-sitter/tree_sitter_node.h"
#include "library/tree/syntax/all.h"

namespace GodotObjectCompiler
{

    Ref<ParserError> TreeSitterParser::parse_file(const String& p_path, Ref<Context> r_target)
    {
        PARSER_ERROR_COND(!file_exists(p_path), "Input file \"%s\" not found.", p_path.c_str());
        input_path = path_absolute(p_path);
        return parse(read_file(input_path), std::move(r_target));
    }

    Ref<ParserError> TreeSitterParser::parse(const String& p_input, Ref<Context> r_target)
    {
        auto global_namespace = r_target->as<Namespace>();
        PARSER_ERROR_COND(
            !global_namespace || !global_namespace->qualified_name().empty(),
            "TreeSitterParser: Invalid target node, expected to be the global namespace.");

        Dictionary<Size, String> stripped_parameters;
        Dictionary<UID, UID> before_node;
        HashSet<UID> handled;

        String local_input =
            ParserUtilities::strip_known_macro_contents(p_input, stripped_parameters);
        local_input = ParserUtilities::strip_excluded_sections(local_input);

        auto context = ParserContext(local_input);
        context.original_buffer = p_input;
        context.stripped_parameters = stripped_parameters;
        context.parse_attributes = parse_attributes;
        if (!input_path.empty()) {
            context.file_path = input_path;
            input_path = "";
        }

        if (!context.is_valid()) {
            global_namespace->B<Body>();
            return ParserError::OK;
        }

        if (Ref<Node> first_ifdef = context.current_src->find_child(0, type_is("preproc_ifdef"))) {
            if (first_ifdef->get_index() == 0) {
                for (const Ref<Node>& child : *first_ifdef->as<Context>()) {
                    context.current_src->add_child(child->clone());
                }
                context.current_src->remove_child(first_ifdef);
            }
        }

        auto recall = [&]() {
            if (const auto itr = before_node.find(context.current_src->get_id());
                itr != before_node.end()) {
                if (Ref<Context> before =
                        LibraryContext::instance()->get_node_db()->get<Context>(itr->second)) {
                    context.current_target = before;
                }
            }
        };

        Ref<Body> body = global_namespace->create_child<Body>();
        context.current_target = body;

        while (true) {
            bool do_continue = true;

            do {
                ParserStep step = context.current_src->is_handled() ? ParserStep::StepOver()
                                                                    : ParserStep::Undecided();

                if (!context.current_src->is_handled()) {
                    for (const Ref<ITreeSitterNodeHandler>& node_handler : _handlers) {
                        if (node_handler->handles_node(context.current_src)) {
                            Ref<Context> before = context.current_target;
                            step =
                                node_handler->handle(context.current_src, context.current_target);

                            PANIC_COND(
                                !context.current_target->has_parent(),
                                "Invalid orphan parser target after call to NodeHandler %s",
                                node_handler->get_type().c_str());

                            if (before != context.current_target) {
                                before_node[context.current_src->get_id()] = before->get_id();
                            }
                            break;
                        }
                    }
                    context.current_src->set_handled();
                }

                if (step.is_undecided()) {
                    if (!context.current_src->empty()) {
                        step = ParserStep::StepInto();
                    } else {
                        step = ParserStep::StepOver();
                    }
                }

                if (step.is_step_into()) {
                    do_continue = !context.current_src->empty();
                    if (do_continue) {
                        context.current_src =
                            context.current_src->get_child(0)->as<TreeSitterNode>();
                    }
                } else if (step.is_step_over()) {
                    do_continue = context.current_src->has_next_sibling();
                    if (do_continue) {
                        context.current_src =
                            context.current_src->get_next_sibling()->as<TreeSitterNode>();
                    }
                }
            } while (do_continue);

            if (!context.current_src->has_parent()) {
                break;
            }

            context.current_src = context.current_src->get_parent()->as<TreeSitterNode>();
            recall();
        }

        return ParserError::OK;
    }

    void TreeSitterParser::config(Config p_config)
    {
        switch (p_config) {
        case CONFIG_PARSE_ATTRIBUTES:
            parse_attributes = true;
            break;
        case CONFIG_SKIP_ATTRIBUTES:
            parse_attributes = false;
            break;
        }
    }

} // namespace GodotObjectCompiler
