/**************************************************************************/
/* class_handler.cpp                                                      */
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
#include "class_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/function.h"
#include "library/tree/syntax/identifier.h"

namespace GodotObjectCompiler
{

    bool ClassHandler::handles_node(const Ref<TreeSitterNode>& p_current_src)
    {
        return p_current_src->type_in({"class_specifier", "base_class_clause"});
    }

    ParserStep
    ClassHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target)
    {
        if (p_current_src->type == "class_specifier") {
            Ref<TreeSitterNode> found = p_current_src->find_descendant<TreeSitterNode>(
                BFS, type_is("field_declaration_list"));
            if (!found) {
                return ParserStep::StepOver();
            }

            Ref<TreeSitterNode> identifier =
                p_current_src->find_child<TreeSitterNode>(BFS, type_is("type_identifier"));
            if (!identifier) {
                print_err("A class with no identifier.");
                print_ln(p_current_src->pretty_print());
                return ParserStep::StepOver();
            }

            const Ref<Node> previous_node = r_current_target->get_child(-1);
            r_current_target = r_current_target->B<Class>();
            r_current_target->create_child<Identifier>(identifier->content());
            identifier->set_handled();
            if (previous_node && previous_node->is<TemplateParameters>()) {
                r_current_target->add_child(previous_node);
            }
        }

        if (p_current_src->type == "base_class_clause") {
            r_current_target = r_current_target->B<BaseClasses>();
        }

        return ParserStep::StepInto();
    }

} // namespace GodotObjectCompiler