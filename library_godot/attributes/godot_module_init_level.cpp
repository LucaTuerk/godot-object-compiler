/**************************************************************************/
/* godot_module_init_level.cpp                                            */
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
#include "godot_module_init_level.h"

#include "../assumptions.h"
#include "library/core/helpers.h"
#include "library/tree/syntax/enum.h"
#include "library/type_db.h"

namespace GodotObjectCompiler {

  String GodotModuleInitializationLevelParameterType::get_return_type() { return "GOC_ModuleInitializationLevel"; }

  Vector<String> GodotModuleInitializationLevelParameterType::get_value_names() { return value_names(); }

  Vector<IAttributeParameterType::Argument> GodotModuleInitializationLevelParameterType::get_arguments() { return {}; }

  Ref<Argument> GodotModuleInitializationLevelParameterType::create_argument() {
    return node_new<GodotModuleInitializationLevelArgument>();
  }

  bool GodotModuleInitializationLevelParameterType::get_godot_init_level_for_value_name(
      const String& p_value_name, String& r_godot_init_level) {
    _value_names_lazy.poke();

    auto itr = _godot_init_levels.find(p_value_name);
    if (itr == _godot_init_levels.end()) {
      r_godot_init_level = "";
      return false;
    }

    r_godot_init_level = itr->second;
    return true;
  }

  Vector<String> GodotModuleInitializationLevelParameterType::_value_names_lazy_get() {
    using namespace AssumedGodotTypes;

    Ref<Enum> init_level_enum = TypeDB::instance()->get_type_data<Enum>(ModuleInitializationLevel().qualified_name);
    if (!init_level_enum) {
      return {};
    }

    Vector<String> return_value;
    for (const String& value_name : init_level_enum->value_names()) {
      String modified = macro_case_to_pascal_case(string_replace(value_name, "MODULE_INITIALIZATION_", ""));
      return_value.push_back(modified);
      _godot_init_levels[modified] = value_name;
    }

    return return_value;
  }

}  // namespace GodotObjectCompiler