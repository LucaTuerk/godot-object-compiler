/**************************************************************************/
/* enum_handler.cpp                                                       */
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

#include "enum_handler.h"

#include "library/parser/tree_sitter_node.h"
#include "library/tree/predicates.h"
#include "library/tree/syntax/enum.h"
#include "library/tree/syntax/identifier.h"
#include "library/tree/syntax/literal.h"

namespace GodotObjectCompiler
{

    bool EnumHandler::handles_node(const Ref<TreeSitterNode>& p_current_src)
    {
        return p_current_src->type_in({"enum_specifier", "enumerator_list", "enumerator"});
    }

    ParserStep
    EnumHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target)
    {
        if (p_current_src->type == "enum_specifier") {
            r_current_target = r_current_target->create_child<Enum>();
        }
        if (p_current_src->type == "enumerator_list") {
            r_current_target = r_current_target->create_child<EnumValues>();
        }
        if (p_current_src->type == "enumerator") {
            r_current_target = r_current_target->create_child<EnumValue>();
            if (p_current_src
                    ->find_children<TreeSitterNode>(
                        true, type_in({"number_literal", "binary_expression"}))
                    .empty()) {
                if (const Ref<TreeSitterNode> identifier =
                        p_current_src->find_child(0, type_is("identifier"))) {
                    r_current_target->create_child<Identifier>(identifier->content());
                    identifier->set_handled();
                }

                if (const Ref<EnumValue> prev =
                        r_current_target->find_previous_sibling<EnumValue>()) {
                    if (const Ref<Literal> literal = prev->find_child<Literal>()) {
                        const int val = string_to_int(literal->content);
                        r_current_target->create_child<Literal>(std::to_string(val + 1));
                    }
                } else {
                    r_current_target->create_child<Literal>(std::to_string(0));
                }
            }
        }
        return ParserStep::StepInto();
    }

} // namespace GodotObjectCompiler