/**************************************************************************/
/* godot_property_hint.h                                                  */
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

#pragma once
#include "library/core/lazy.h"
#include "library/generator/attribute_parameter_type.h"
#include "library/tree/syntax/node.h"

namespace GodotObjectCompiler {

class GodotPropertyHintArgument : public Argument {
  NODE_TYPE(GodotPropertyHintArgument);

  LAZY(GodotPropertyHintArgument, String, godot_property_hint)
  LAZY(GodotPropertyHintArgument, String, hint_string)
  LAZY(GodotPropertyHintArgument, String, hint_content)
};

class GodotPropertyHintParameterType : public IAttributeParameterType {
  PARAM_TYPE(GodotPropertyHintParameterType, GodotPropertyHintArgument);

 public:
  String get_return_type() override;

  Vector<String> get_value_names() override;

  Vector<Argument> get_arguments() override;

  bool get_godot_hint_for_value_name(const String& p_name, String& r_macro);

 private:
  LAZY_MUT(GodotPropertyHintParameterType, Vector<String>, value_names);
  Dictionary<String, String> _godot_hint_types;
};

}  // namespace GodotObjectCompiler
