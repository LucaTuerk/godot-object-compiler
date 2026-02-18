/**************************************************************************/
/* godot_variant_type.cpp                                                 */
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

#include "godot_variant_type.h"

#include "library/core/string_utilities.h"
#include "library/tree/syntax/enum.h"
#include "library/tree/syntax/identifier.h"
#include "library/type_db.h"
#include "library_godot/assumptions.h"

namespace GodotObjectCompiler {

  String GodotVariantTypeParameterType::get_return_type() { return "GOC_VariantType"; }

  Vector<String> GodotVariantTypeParameterType::get_value_names() { return value_names(); }

  Vector<IAttributeParameterType::Argument> GodotVariantTypeParameterType::get_arguments() { return {}; }

  bool GodotVariantTypeParameterType::get_variant_type_for_value_name(
      const String& p_value_name, String& r_variant_type) {
    _value_names_lazy.poke();

    const auto itr = _value_name_to_godot_variant_type.find(p_value_name);
    if (itr == _value_name_to_godot_variant_type.end()) {
      r_variant_type = "";
      return false;
    }

    r_variant_type = itr->second;
    return true;
  }

  Vector<String> GodotVariantTypeParameterType::_value_names_lazy_get() {
    Ref<Enum> variant_type_enum = ExecutionContext::instance()->get_type_db()->get_type_data<Enum>(
        AssumedGodotTypes::VariantTypeEnum().qualified_name);
    if (!variant_type_enum) {
      return {};
    }

    Vector<String> results;
    for (const String& enum_value : variant_type_enum->value_names()) {
      String value_name = macro_case_to_pascal_case("VARIANT_TYPE_" + enum_value);
      results.push_back(value_name);
      _value_name_to_godot_variant_type[value_name] = enum_value;
    }

    return results;
  }

  String GodotVariantTypeArgument::_godot_variant_type_lazy_get() const {
    auto ptype = GodotVariantTypeParameterType::instance();

    const Ref<Identifier> identifier = find_child<Identifier>();
    if (!identifier) {
      PANIC("Malformed GodotVariantTypeArgument");
    }

    String variant_type;
    if (!ptype->get_variant_type_for_value_name(identifier->name, variant_type)) {
      PANIC("Malformed GodotVariantTypeArgument");
    }

    return variant_type;
  }

}
