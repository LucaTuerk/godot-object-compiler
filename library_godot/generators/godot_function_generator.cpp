/**************************************************************************/
/* godot_function_generator.cpp                                           */
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

#include "godot_function_generator.h"

#include "godot_generator_utils.h"

namespace GodotObjectCompiler {

  Ref<GeneratorError> GodotFunctionGenerator::do_generate(Ref<Class> p_target_class,
      Ref<GodotFunctionAttribute> p_attribute, Ref<Context> p_generated_body, Ref<Context> p_generated_sources,
      Ref<Context> p_generated_global) {
    Ref<Node> target_node = p_attribute->resolve_target();
    GEN_ERROR_COND(target_node == nullptr, p_target_class, "Could not resolve target node for function macro.");

    Ref<Function> target_function = target_node->as<Function>();
    GEN_ERROR_COND(target_function == nullptr, p_target_class, "Resolved not for function macro is not a function.");

    Ref<Body> bind_methods_body =
        GodotGeneratorUtils::get_or_create_bind_methods_body(p_target_class, p_generated_body, p_generated_sources);

    Ref<Parameters> parameters = target_function->parameters();
    GEN_ERROR_COND(!parameters, p_target_class,
        "Function does not name parameters, this was probably parsed as a function call. Abort");

    Vector<String> parameter_names;

    for (Ref<Node> child : *parameters) {
      Ref<Parameter> parameter = child->as<Parameter>();
      if (!parameter) {
        continue;
      }
      parameter_names.push_back(parameter->name());
    }

    if (target_function->is_static()) {
      bind_methods_body->add_child(
          GodotGeneratorUtils::bind_static_method(p_target_class->name(), target_function->name(), parameter_names));
    } else {
      bind_methods_body->add_child(
          GodotGeneratorUtils::bind_method(p_target_class->name(), target_function->name(), parameter_names));
    }

    return GeneratorError::OK;
  }

}