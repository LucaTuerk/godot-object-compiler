/**************************************************************************/
/* godot_class_generator.cpp                                              */
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

#include "godot_class_generator.h"

#include "godot_generator_utils.h"
#include "library/tree/output/output.h"
#include "library_godot/assumptions.h"

namespace GodotObjectCompiler {

  Ref<GeneratorError> GodotClassGenerator::generate_initialization(Ref<Class> p_target_class,
      Ref<GodotClassAttribute> p_class_attribute, Ref<Context> p_initialize_content,
      Ref<Context> p_uninitialize_content) {
    UNUSED(p_uninitialize_content);
    using namespace GodotGeneratorUtils;

    const Ref<Identifier> init_level_identifier =
        p_class_attribute->find_chain<Identifier, Arguments, GodotModuleInitializationLevelArgument>();
    GEN_ERROR_COND(!init_level_identifier, p_target_class,
        "Could not determine GodotModuleInitializationLevelArgument identifier.");

    const String init_level_name = init_level_identifier->name;
    const Ref<GodotModuleInitializationLevelParameterType> init_level_type =
        GodotModuleInitializationLevelParameterType::instance();

    String godot_init_level;
    if (!init_level_type->get_godot_init_level_for_value_name(init_level_name, godot_init_level)) {
      GEN_ERROR(p_target_class, "Failed to get ModuleInitializationLevel enum value for value name " + init_level_name)
    }

    const Ref<Identifier> class_type_identifier =
        p_class_attribute->find_chain<Identifier, Arguments, GodotClassTypeArgument>();
    GEN_ERROR_COND(!class_type_identifier, p_target_class, "Could not determine GodotClassTypeArgument identifier.");

    const String class_type_name = class_type_identifier->name;
    const Ref<GodotClassTypeParameterType> class_type_type = GodotClassTypeParameterType::instance();

    if (String godot_registration_macro;
        class_type_type->get_macro_for_value_name(class_type_name, godot_registration_macro)) {
      const Ref<Context> if_clause =
          Output::Spaces({Output::Text(format("if (p_level == %s) ", godot_init_level.c_str()))});

      const Ref<Body> condition_body =
          get_if_body(p_initialize_content, format("p_level == %s", godot_init_level.c_str()));
      // clang-format off
      condition_body->add_child(build<Function>().with_children({
        build<Identifier>(godot_registration_macro),
        build<Arguments>().with_child(build<Argument>().with_child(Output::Text(p_target_class->qualified_name()))),
      }).with_child(Output::Semicolon()));
      // clang-format on
    }
    return GeneratorError::OK;
  }

  Ref<GeneratorError> GodotClassGenerator::generate_startup(Ref<Class> p_target_class,
      Ref<GodotClassAttribute> p_class_attribute, Ref<Context> p_startup_content, Ref<Context> p_shutdown_content) {
    UNUSED(p_target_class);
    UNUSED(p_class_attribute);
    UNUSED(p_startup_content);
    UNUSED(p_shutdown_content);
    return GeneratorError::OK;
  }

  Ref<GeneratorError> GodotClassGenerator::do_generate_default_attribute_arguments(
      Ref<Class> p_target_class, Ref<GodotClassAttribute> p_attribute, Ref<Context> p_default_values) {
    UNUSED(p_target_class);
    UNUSED(p_attribute);
    // clang-format off
      p_default_values->add_children({
      build<GodotModuleInitializationLevelArgument>()
          .with_child<Identifier>(AssumedParameterValues::LevelScene())
          .with_child<Arguments>(),
      build<GodotClassTypeArgument>()
          .with_child<Identifier>(AssumedParameterValues::GodotClass())
          .with_child<Arguments>()
      });
    // clang-format on

    return GeneratorError::OK;
  }

  Ref<GeneratorError> GodotClassGenerator::do_generate(Ref<Class> p_target_class, Ref<GodotClassAttribute> p_attribute,
      Ref<Context> p_generated_body, Ref<Context> p_generated_sources, Ref<Context> p_generated_global) {
    UNUSED(p_generated_global);
    UNUSED(p_attribute);

    using namespace GodotGeneratorUtils;

    GEN_ERROR_COND(p_attribute->resolve_target() != p_target_class, p_target_class,
        "Resolved class is not the provided target class.");

    Vector<String> bases = p_target_class->direct_bases_names();
    GEN_ERROR_COND(bases.empty(), p_target_class,
        "Target class does not name base classes and thus cannot inherit from a Godot object type.");

    if (!class_is_godot_object_type(p_target_class)) {
      GEN_ERROR(p_target_class, "Target class does not inherit from a godot object class or the class was not found.");
    }

    // clang-format off
    Ref<Function> gd_class = build<Function>().with_children({
      build<Identifier>(AssumedGodotTypes::GDCLASS().qualified_name),
      build<Arguments>().with_children({
        build<Argument>().with_child(Output::Text(p_target_class->name())),
        build<Argument>().with_child(Output::Text(bases[0])),
      })
    }).with_child(Output::Semicolon());
    // clang-format on

    p_generated_body->add_children({
        gd_class,
        build<AccessSpecifier>(AccessSpecifier::PUBLIC),
        build<Context>().with_tag("public_members"),
        build<AccessSpecifier>(AccessSpecifier::PROTECTED),
        build<Context>().with_tag("protected_members"),
        build<AccessSpecifier>(AccessSpecifier::PRIVATE),
        build<Context>().with_tag("private_members"),
    });

    get_bind_methods_body(p_target_class, p_generated_body, p_generated_sources);
    get_notification_body(p_target_class, p_generated_body, p_generated_sources);
    get_get_property_list_body(p_target_class, p_generated_body, p_generated_sources);
    get_property_names_body(p_target_class, p_generated_body);
    get_function_names_body(p_target_class, p_generated_body);
    get_signal_names_body(p_target_class, p_generated_body);

    return GeneratorError::OK;
  }

}
