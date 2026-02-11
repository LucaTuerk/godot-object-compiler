/**************************************************************************/
/* godot_property_usage_flags.cpp                                         */
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

#include "godot_property_usage_flags.h"

#include "../../library/type_db.h"
#include "../assumptions.h"
#include "library/core/string_utilities.h"
#include "library/tree/syntax/enum.h"
#include "library/tree/syntax/identifier.h"

namespace GodotObjectCompiler {

  IAttributeParameterType::FeatureFlags GodotPropertyUsageFlagsParameterType::get_features() { return FEATURE_FLAG; }

  String GodotPropertyUsageFlagsParameterType::get_return_type() { return "GOC_PropertyUsageFlags"; }

  Vector<String> GodotPropertyUsageFlagsParameterType::get_value_names() { return value_names(); }

  Vector<IAttributeParameterType::Argument> GodotPropertyUsageFlagsParameterType::get_arguments() { return {}; }

  bool GodotPropertyUsageFlagsParameterType::get_godot_usage_flag_for_value_name(
      const String& p_name, String& r_usage_flag) {
    _value_names_lazy.poke();

    if (auto itr = _godot_usage_flags.find(p_name); itr != _godot_usage_flags.end()) {
      r_usage_flag = itr->second;
      return true;
    }
    r_usage_flag = "";
    return false;
  }

  Ref<Argument> GodotPropertyUsageFlagsParameterType::create_argument() {
    return node_new<GodotPropertyUsageFlagsArgument>();
  }

  Vector<String> GodotPropertyUsageFlagsParameterType::_value_names_lazy_get() {
    Ref<Enum> prop_use_enum =
        TypeDB::instance()->get_type_data<Enum>(AssumedGodotTypes::PropertyUsageFlagsEnum().qualified_name);
    if (!prop_use_enum) {
      return {};
    }

    Vector<String> return_value;
    for (const String& value_name : prop_use_enum->value_names()) {
      auto final_name = macro_case_to_pascal_case(string_replace(value_name, "PROPERTY_", ""));
      _godot_usage_flags[final_name] = value_name;
      return_value.push_back(final_name);
    }
    return return_value;
  }

  String GodotPropertyUsageFlagsArgument::_godot_property_usage_flag_lazy_get() const {
    auto ptype = GodotPropertyUsageFlagsParameterType::instance();

    Ref<Identifier> identifier = find_child<Identifier>();
    if (!identifier) {
      PANIC("Malformed PropertUsageFlagsArgument.");
    }

    String usage_flag;
    if (!ptype->get_godot_usage_flag_for_value_name(identifier->name, usage_flag)) {
      PANIC("Malformed PropertUsageFlagsArgument.");
    }
    return usage_flag;
  }

}