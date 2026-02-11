/**************************************************************************/
/* godot_hint_generators.cpp                                              */
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

#include "godot_hint_generators.h"

#include "godot_generator_utils.h"

namespace GodotObjectCompiler {

  Ref<GeneratorError> GodotGroupGenerator::do_generate_default_attribute_arguments(
      Ref<Class> p_target_class, Ref<GodotPropertyGroupAttribute> p_attribute, Ref<Context> p_default_values) {
    UNUSED(p_target_class);
    UNUSED(p_attribute);
    p_default_values->build_child<StringLiteralArgument>().with_child<Literal>("\"\"");
    return GeneratorError::OK;
  }

  Ref<GeneratorError> GodotGroupGenerator::do_generate(Ref<Class> p_target_class,
      Ref<GodotPropertyGroupAttribute> p_attribute, Ref<Context> p_generated_body, Ref<Context> p_generated_sources,
      Ref<Context> p_generated_global) {
    UNUSED(p_generated_global);

    Ref<Body> bind_methods =
        GodotGeneratorUtils::get_or_create_bind_methods_body(p_target_class, p_generated_body, p_generated_sources);
    GEN_ERROR_COND(!bind_methods, p_attribute, "Failed to get _bind_methods body");

    // clang-format off
    bind_methods->build_child<Function>().with_children({
      build<Identifier>("ADD_GROUP"),
      build<Arguments>().with_children({
          build<Argument>().with_child<Literal>(p_attribute->literal_content()),
        build<Argument>().with_child<Literal>("\"\"")
      })
    }).with_child(Writer::Semicolon());
    // clang-format on
    return GeneratorError::OK;
  }

  Ref<GeneratorError> GodotSubgroupGenerator::do_generate_default_attribute_arguments(
      Ref<Class> p_target_class, Ref<GodotPropertySubgroupAttribute> p_attribute, Ref<Context> p_default_values) {
    UNUSED(p_target_class);
    UNUSED(p_attribute);
    p_default_values->build_child<StringLiteralArgument>().with_child<Literal>("\"\"");
    return GeneratorError::OK;
  }

  Ref<GeneratorError> GodotSubgroupGenerator::do_generate(Ref<Class> p_target_class,
      Ref<GodotPropertySubgroupAttribute> p_attribute, Ref<Context> p_generated_body, Ref<Context> p_generated_sources,
      Ref<Context> p_generated_global) {
    UNUSED(p_generated_global);

    Ref<Body> bind_methods =
        GodotGeneratorUtils::get_or_create_bind_methods_body(p_target_class, p_generated_body, p_generated_sources);
    GEN_ERROR_COND(!bind_methods, p_attribute, "Failed to get _bind_methods body");

    // clang-format off
    bind_methods->build_child<Function>().with_children({
      build<Identifier>("ADD_SUBGROUP"),
      build<Arguments>().with_children({
          build<Argument>().with_child<Literal>(p_attribute->literal_content()),
        build<Argument>().with_child<Literal>("\"\"")
      })
    }).with_child(Writer::Semicolon());
    // clang-format on

    return GeneratorError::OK;
  }

}