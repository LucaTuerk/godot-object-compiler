/**************************************************************************/
/* function_handler.cpp                                                   */
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
#include "function_handler.h"

#include "library/attribute_db.h"
#include "library/library_context.h"
#include "library/tree/syntax/attribute.h"
#include "library/tree/syntax/field.h"
#include "library/tree/syntax/function.h"
#include "library/tree/syntax/literal.h"
#include "library/tree_sitter_parser/node_handler.h"
#include "library/tree_sitter_parser/tree_sitter_node.h"

namespace GodotObjectCompiler
{

    bool FieldHandler::handles_node(const Ref<TreeSitterNode>& p_current_src)
    {
        return p_current_src->type_in(
            {"field_declaration", "declaration", "function_definition", "expression_statement",
             "call_expression"});
    }

    ParserStep
    FieldHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target)
    {
        if (p_current_src->type == "call_expression") {
            r_current_target->B<Literal>(p_current_src->content());
            return ParserStep::StepOver();
        } else if (p_current_src->type_in({"expression_statement", "declaration"})) {
            Ref<TreeSitterNode> identifier =
                p_current_src->find_descendant<TreeSitterNode>(BFS, type_is("identifier"));
            if (identifier && LibraryContext::instance()->get_attribute_db()->is_known_macro(
                                  identifier->content())) {
                return handle_known_attribute(
                    p_current_src, r_current_target, identifier->content());
            }
        } else if (p_current_src->find_descendant(BFS, type_is("function_declarator")) != nullptr) {
            r_current_target = r_current_target->B<Function>();
        } else if (
            p_current_src->find_descendant(
                BFS, type_in({"class_specifier", "struct_specifier", "enum_specifier"})) !=
            nullptr) {
        } else {
            r_current_target = r_current_target->B<Field>();
        }
        return ParserStep::StepInto();
    }

    ParserStep FieldHandler::handle_known_attribute(
        const Ref<TreeSitterNode>& current_src, Ref<Context>& current_target, const String& macro)
    {
        Result<Attribute> attribute_result =
            LibraryContext::instance()->get_attribute_db()->create_for_macro(macro);
        HANDLER_ERROR_COND(
            attribute_result.has_error(), "Failed to create attribute for known macro %s",
            macro.c_str());
        Ref<Attribute> attribute = attribute_result.get_result();

        current_target->add_child(attribute);
        attribute->start = current_src->start_byte;
        attribute->end = current_src->end_byte;
        attribute->line = current_src->start_point.row + 1;

        if (!current_src->context->parse_attributes) {
            auto itr = current_src->context->stripped_parameters.find(current_src->start_byte);
            if (itr != current_src->context->stripped_parameters.end()) {
                attribute->B<UnparsedAttributeArguments>(itr->second);
            }
            return ParserStep::StepOver();
        }

        Ref<IAttributeArgumentParser> argument_parser = attribute->get_argument_parser();
        if (argument_parser) {
            auto itr = current_src->context->stripped_parameters.find(current_src->start_byte);
            if (itr != current_src->context->stripped_parameters.end()) {
                Ref<ParserError> error = argument_parser->parse_attribute_arguments(
                    itr->second, attribute->create_child<Arguments>());

                if (error != ParserError::OK) {
                    attribute->remove_all_children();
                    attribute->add_child(node_new<ParserError>(
                        error->error_level, "TreeSitterParser",
                        format("Failed to parse attribute arguments.\n%s", error->message.c_str()),
                        current_src->context->file_path, current_src->context->original_buffer,
                        attribute->line, current_src->start_point.column + 1));
                    error->set_handled();
                }
            }
        }
        return ParserStep::StepOver();
    }

} // namespace GodotObjectCompiler
