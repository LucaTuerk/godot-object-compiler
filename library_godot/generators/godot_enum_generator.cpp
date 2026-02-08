/**************************************************************************/
/* godot_enum_generator.cpp                                               */
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

#include "godot_enum_generator.h"

#include "godot_generator_utils.h"
#include "library/tree/predicates.h"

namespace GodotObjectCompiler {

  Ref<GeneratorError> GodotEnumGenerator::do_generate(Ref<Class> p_target_class, Ref<GodotEnumAttribute> p_attribute,
      Ref<Context> p_generated_body, Ref<Context> p_generated_sources, Ref<Context> p_generated_global) {
    Ref<Node> target_node = p_attribute->resolve_target();
    GEN_ERROR_COND(!target_node, p_target_class, "Could not find target for Enum marco.");

    Ref<Enum> target_enum = target_node->as<Enum>();
    GEN_ERROR_COND(
        !target_enum, p_target_class, "Resolved target for enum macro is not an enum, but " + target_node->get_type());

    Ref<Body> bind_methods_body =
        GodotGeneratorUtils::get_or_create_bind_methods_body(p_target_class, p_generated_body, p_generated_sources);

    for (const String& name : target_enum->value_names()) {
      // clang-format off
      bind_methods_body->build_child<Function>().with_children({
        build<Identifier>("BIND_ENUM_CONSTANT"),
        build<Arguments>().with_child(
          build<Argument>().with_child(
            Writer::Text(name)
          )
        )
      }).with_child(Writer::Semicolon());
      // clang-format on
    }

    // clang-format off

    p_generated_global->build_child<Function>().with_children({
      build<Identifier>("VARIANT_ENUM_CAST"),
      build<Arguments>().with_child(
        build<Argument>().with_child(
          Writer::Text(target_enum->qualified_name())
          )
        )
    }).with_child(Writer::Semicolon());

    // clang-format on
    return GeneratorError::OK;
  }

}  // namespace GodotObjectCompiler