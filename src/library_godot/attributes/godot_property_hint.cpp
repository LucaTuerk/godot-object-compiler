/**************************************************************************/
/* godot_property_hint.cpp                                                */
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

#include "godot_property_hint.h"

#include "library/core/string_utilities.h"
#include "library/tree/syntax/enum.h"
#include "library/tree/syntax/identifier.h"
#include "library/type_db.h"
#include "library_godot/assumptions.h"

namespace GodotObjectCompiler {

  String GodotPropertyHintParameterType::get_return_type() { return "GOC_PropertyHint"; }

  Vector<String> GodotPropertyHintParameterType::get_value_names() { return value_names(); }

  Vector<IAttributeParameterType::Argument> GodotPropertyHintParameterType::get_arguments() {
    return {{ARG_STRING, "hint_string", true}};
  }

  bool GodotPropertyHintParameterType::get_godot_hint_for_value_name(const String& p_name, String& r_macro) {
    _value_names_lazy.poke();

    if (auto itr = _godot_hint_types.find(p_name); itr != _godot_hint_types.end()) {
      r_macro = itr->second;
      return true;
    }
    r_macro = "";
    return false;
  }

  Ref<Argument> GodotPropertyHintParameterType::create_argument() { return node_new<GodotPropertyHintArgument>(); }

  Vector<String> GodotPropertyHintParameterType::_value_names_lazy_get() {
    Ref<Enum> prop_hint_enum = ExecutionContext::instance()->get_type_db()->get_type_data<Enum>(
        AssumedGodotTypes::PropertyHintEnum().qualified_name);
    if (!prop_hint_enum) {
      return {};
    }

    Vector<String> return_value;
    for (const String& enum_value : prop_hint_enum->value_names()) {
      auto value_name = macro_case_to_pascal_case(string_replace(enum_value, "PROPERTY_", ""));
      _godot_hint_types[value_name] = enum_value;
      return_value.push_back(value_name);
    }
    return return_value;
  }

  String GodotPropertyHintArgument::_godot_property_hint_lazy_get() const {
    auto ptype = GodotPropertyHintParameterType::instance();
    String hint;
    if (Ref<Identifier> identifier = find_child<Identifier>();
        identifier && ptype->get_godot_hint_for_value_name(identifier->name, hint)) {
      return hint;
    }
    PANIC("Malformed property hint argument");
  }

  String GodotPropertyHintArgument::_hint_string_lazy_get() const {
    Ref<Arguments> arguments = find_child<Arguments>();
    if (!arguments) {
      return "\"\"";
    }

    Ref<Argument> argument = arguments->find_child<Argument>();
    if (!argument) {
      return "\"\"";
    }

    Ref<Literal> literal = argument->find_child<Literal>();
    if (!literal) {
      return "\"\"";
    }

    return literal->content;
  }

  String GodotPropertyHintArgument::_hint_content_lazy_get() const {
    String hint = hint_string();
    if (hint.size() < 2) {
      return "";
    }

    return hint.substr(1, hint.size() - 2);
  }

}
