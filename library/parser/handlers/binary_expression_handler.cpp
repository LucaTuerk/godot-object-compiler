/**************************************************************************/
/* binary_expression_handler.cpp                                          */
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

#include "binary_expression_handler.h"

#include "library/tree/predicates.h"
#include "library/tree/syntax/enum.h"
#include "library/tree/syntax/literal.h"

namespace GodotObjectCompiler {

  bool BinaryExpressionHandler::handles_node(const Ref<TreeSitterNode>& p_current_src) {
    return p_current_src->type == "binary_expression";
  }

  ParserStep BinaryExpressionHandler::handle(const Ref<TreeSitterNode>& p_current_src, Ref<Context>& r_current_target) {
    if (!r_current_target->is<EnumValue>()) {
      // binary expression only supported for enum value resolution.
      return ParserStep::StepOver();
    }

    const Ref<int> result = calculate_binary_expression(p_current_src->get_child<TreeSitterNode>(0),
        p_current_src->get_child<TreeSitterNode>(1), p_current_src->get_child<TreeSitterNode>(2),
        r_current_target->as<EnumValue>());

    if (!result) {
      node_new<ParserError>(ERROR, p_current_src, "Failed to parse binary expression.");
      return ParserStep::StepOver();
    }

    r_current_target->create_child<Literal>(std::to_string(*result));
    return ParserStep::StepOver();
  }

  Ref<int> BinaryExpressionHandler::calculate_binary_expression(const Ref<TreeSitterNode>& left_operand,
      const Ref<TreeSitterNode>& expr_operator, const Ref<TreeSitterNode>& right_operand,
      const Ref<EnumValue>& current_target) {
    if (!(left_operand && left_operand->type_in({"number_literal", "identifier", "binary_expression"}) &&
            right_operand && right_operand->type_in({"number_literal", "identifier"}) && expr_operator &&
            expr_operator->type_in({"+", "-", "*", "/", "%", "&", "|", "^", "<<", ">>"}))) {
      return nullptr;
    }

    int first = 0;
    if (left_operand->type == "binary_expression") {
      const Ref<int> nested_result = calculate_binary_expression(left_operand->get_child<TreeSitterNode>(0),
          left_operand->get_child<TreeSitterNode>(1), left_operand->get_child<TreeSitterNode>(2), current_target);
      if (!nested_result) {
        return nullptr;
      }
      first = *nested_result;
    } else {
      if (left_operand->type == "number_literal") {
        first = string_to_int(left_operand->content());
      } else {
        const Ref<int> first_opt = find_value_for_identifier(left_operand->content(), current_target);
        if (!first_opt) {
          return nullptr;
        }
        first = *first_opt;
      }
    }

    int second = 0;
    if (right_operand->type == "number_literal") {
      second = string_to_int(right_operand->content());
    } else {
      const Ref<int> second_opt = find_value_for_identifier(right_operand->content(), current_target);
      if (!second_opt) {
        return nullptr;
      }
      second = *second_opt;
    }
    int result = 0;

    if (expr_operator->type == "+") {
      result = first + second;
    } else if (expr_operator->type == "-") {
      result = first - second;
    } else if (expr_operator->type == "*") {
      result = first * second;
    } else if (expr_operator->type == "/") {
      result = first / second;
    } else if (expr_operator->type == "%") {
      result = first % second;
    } else if (expr_operator->type == "&") {
      result = first & second;
    } else if (expr_operator->type == "|") {
      result = first | second;
    } else if (expr_operator->type == "^") {
      result = first ^ second;
    } else if (expr_operator->type == "<<") {
      result = first << second;
    } else if (expr_operator->type == ">>") {
      result = first >> second;
    }

    return make_ref<int>(result);
  }

  Ref<int> BinaryExpressionHandler::find_value_for_identifier(
      const String& identifier, const Ref<EnumValue>& current_target) {
    const Ref<EnumValue> identified =
        current_target->find_previous_sibling<EnumValue>(NamedContextPredicates::name<EnumValue>(identifier.c_str()));
    if (!identified) {
      return nullptr;
    }

    const Ref<Literal> literal = identified->find_child<Literal>();
    if (!literal) {
      return nullptr;
    }

    return make_ref<int>(string_to_int(literal->content));
  }

}