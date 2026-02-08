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

#include "../assumptions.h"
#include "godot_generator_utils.h"

#include "library/core/string_writer.h"
#include "library/tree/output/output.h"
#include "library/tree/output/output_transformator.h"

namespace GodotObjectCompiler {

  Ref<GeneratorError> GodotClassGenerator::generate_initialization(Ref<Class> p_target_class,
      Ref<GodotClassAttribute> p_class_attribute, Ref<Context> p_initialize_content,
      Ref<Context> p_uninitialize_content) {
    using namespace GodotGeneratorUtils;

    Ref<Arguments> arguments = p_class_attribute->find_child<Arguments>();
    GEN_ERROR_COND(!arguments, p_target_class, "No arguments found.");

    Ref<GodotModuleInitializationLevelArgument> init_level =
        arguments->find_child<GodotModuleInitializationLevelArgument>();
    GEN_ERROR_COND(!init_level, p_target_class, "Initialization level argument does not exist.");

    Ref<Identifier> init_level_identifier = init_level->find_child<Identifier>();
    GEN_ERROR_COND(
        !init_level_identifier, p_target_class, "Could not determine GodotModuleInitializationLevelArgument identifier.");

    String init_level_name = init_level_identifier->name;
    Ref<GodotModuleInitializationLevelParameterType> init_level_type =
        AttributeDB::instance()->get_parameter_type<GodotClassAttribute, GodotModuleInitializationLevelParameterType>();

    String godot_init_level;
    if (!init_level_type->get_godot_init_level_for_value_name(init_level_name, godot_init_level)) {
      GEN_ERROR(p_target_class, "Failed to get ModuleInitializationLevel enum value for value name " + init_level_name)
    }

    Ref<GodotClassTypeArgument> class_type = arguments->find_child<GodotClassTypeArgument>();
    GEN_ERROR_COND(!class_type, p_target_class, "Class type argument does not exists.");

    Ref<Identifier> class_type_identifier = class_type->find_child<Identifier>();
    GEN_ERROR_COND(!class_type_identifier, p_target_class, "Could not determine GodotClassTypeArgument identifier.");

    String class_type_name = class_type_identifier->name;
    Ref<GodotClassTypeParameterType> class_type_type =
        AttributeDB::instance()->get_parameter_type<GodotClassAttribute, GodotClassTypeParameterType>();

    String godot_registration_macro;
    if (class_type_type->get_macro_for_value_name(class_type_name, godot_registration_macro)) {
      Ref<Context> if_clause = Writer::Spaces({Writer::Text(format("if (p_level == %s) ", godot_init_level.c_str()))});
      Ref<Body> body = if_clause->build_child<Body>();
      p_initialize_content->add_child(if_clause);
      // clang-format off
      body->add_child(build<Function>().with_children({
        build<Identifier>(godot_registration_macro),
        build<Arguments>().with_child(build<Argument>().with_child(Writer::Text(p_target_class->qualified_name()))),
      }).with_child(Writer::Semicolon()));
      // clang-format on
    }
    return GeneratorError::OK;
  }

  Ref<GeneratorError> GodotClassGenerator::generate_startup(Ref<Class> p_target_class,
      Ref<GodotClassAttribute> p_class_attribute, Ref<Context> p_startup_content, Ref<Context> p_shutdown_content) {
    return GeneratorError::OK;
  }

  Ref<GeneratorError> GodotClassGenerator::do_generate_default_attribute_arguments(
      Ref<Class> p_target_class, Ref<GodotClassAttribute> p_attribute, Ref<Context> p_default_values) {
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
    using namespace GodotGeneratorUtils;

    GEN_ERROR_COND(
        p_attribute->resolve_target() != p_target_class, p_target_class, "Resolved class is not the provided target class.");

    Vector<String> bases = p_target_class->direct_bases_names();
    GEN_ERROR_COND(bases.empty(), p_target_class,
        "Target class does not name base classes and thus cannot inherit from a Godot object type.");

    if (!class_is_godot_object_type(p_target_class)) {
      GEN_ERROR(p_target_class, "Target class does not inherit from a godot object class or the class was not found.");
    }

    // clang-format off
    Ref<Function> gd_class = build<Function>().with_children({
      build<Identifier>("GDCLASS"),
      build<Arguments>().with_children({
        build<Argument>().with_child(Writer::Text(p_target_class->name())),
        build<Argument>().with_child(Writer::Text(bases[0])),
      })
    }).with_child(Writer::Semicolon());
    // clang-format on

    p_generated_body->add_child(gd_class);
    get_or_create_bind_methods_body(p_target_class, p_generated_body, p_generated_sources);

    return GeneratorError::OK;
  }

}