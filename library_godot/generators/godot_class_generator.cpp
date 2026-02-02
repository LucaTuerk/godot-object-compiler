
#include "godot_class_generator.h"

#include "../assumptions.h"
#include "godot_generator_utils.h"
#include "library/core/helpers.h"
#include "library/core/string_writer.h"
#include "library/tree/output/output.h"
#include "library/tree/output/output_transformator.h"

namespace GodotObjectCompiler {

  Ref<GeneratorError> GodotClassGenerator::generate_initialization(Ref<Class> target_class,
      Ref<GodotClassAttribute> class_attribute, Ref<Context> initialize_content, Ref<Context> uninitialize_content) {
    using namespace GodotGeneratorUtils;

    Ref<Arguments> arguments = class_attribute->find_child<Arguments>();
    GEN_ERROR_COND(!arguments, target_class, "No arguments found.");

    Ref<GodotModuleInitializationLevelArgument> init_level =
        arguments->find_child<GodotModuleInitializationLevelArgument>();
    GEN_ERROR_COND(!init_level, target_class, "Initialization level argument does not exist.");

    Ref<Identifier> init_level_identifier = init_level->find_child<Identifier>();
    GEN_ERROR_COND(
        !init_level_identifier, target_class, "Could not determine GodotModuleInitializationLevelArgument identifier.");

    String init_level_name = init_level_identifier->name;
    Ref<GodotModuleInitializationLevelParameterType> init_level_type =
        AttributeDB::instance()->get_parameter_type<GodotClassAttribute, GodotModuleInitializationLevelParameterType>();

    String godot_init_level;
    if (!init_level_type->get_godot_init_level_for_value_name(init_level_name, godot_init_level)) {
      GEN_ERROR(target_class, "Failed to get ModuleInitializationLevel enum value for value name " + init_level_name)
    }

    Ref<GodotClassTypeArgument> class_type = arguments->find_child<GodotClassTypeArgument>();
    GEN_ERROR_COND(!class_type, target_class, "Class type argument does not exists.");

    Ref<Identifier> class_type_identifier = class_type->find_child<Identifier>();
    GEN_ERROR_COND(!class_type_identifier, target_class, "Could not determine GodotClassTypeArgument identifier.");

    String class_type_name = class_type_identifier->name;
    Ref<GodotClassTypeParameterType> class_type_type =
        AttributeDB::instance()->get_parameter_type<GodotClassAttribute, GodotClassTypeParameterType>();

    String godot_registration_macro;
    if (class_type_type->get_macro_for_value_name(class_type_name, godot_registration_macro)) {
      Ref<Context> if_clause = Writer::Spaces({Writer::Text(format("if (p_value == %s) ", godot_init_level.c_str()))});
      Ref<Body> body = if_clause->build_child<Body>();
      initialize_content->add_child(if_clause);
      // clang-format off
      body->add_child(build<Function>().with_children({
        build<Identifier>(godot_registration_macro),
        build<Arguments>().with_child(build<Argument>().with_child(Writer::Text(target_class->qualified_name()))),
      }).with_child(Writer::Semicolon()));
      // clang-format on

      OutputTransformator transformator;
      Ref<Writer::IOutputNode> output = transformator.transform(if_clause);
      StreamWriter writer;
      output->get_output(&writer);
      print_err(writer.get_string());
    }
    return GeneratorError::OK;
  }

  Ref<GeneratorError> GodotClassGenerator::generate_startup(Ref<Class> target_class,
      Ref<GodotClassAttribute> class_attribute, Ref<Context> startup_content, Ref<Context> shutdown_content) {
    return GeneratorError::OK;
  }

  Ref<GeneratorError> GodotClassGenerator::do_generate_default_attribute_arguments(
      Ref<Class> target_class, Ref<GodotClassAttribute> attribute, Ref<Context> default_values) {
    // clang-format off
      default_values->add_children({
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

  Ref<GeneratorError> GodotClassGenerator::do_generate(Ref<Class> target_class, Ref<GodotClassAttribute> attribute,
      Ref<Context> generated_body, Ref<Context> generated_sources, Ref<Context> generated_global) {
    using namespace GodotGeneratorUtils;

    GEN_ERROR_COND(
        attribute->resolve_target() != target_class, target_class, "Resolved class is not the provided target class.");

    Vector<String> bases = target_class->direct_bases_names();
    GEN_ERROR_COND(bases.empty(), target_class,
        "Target class does not name base classes and thus cannot inherit from a Godot object type.");

    if (!class_is_godot_object_type(target_class)) {
      GEN_ERROR(target_class, "Target class does not inherit from a godot object class or the class was not found.");
    }

    // clang-format off
    Ref<Function> gd_class = build<Function>().with_children({
      build<Identifier>("GDCLASS"),
      build<Arguments>().with_children({
        build<Argument>().with_child(Writer::Text(target_class->name())),
        build<Argument>().with_child(Writer::Text(bases[0])),
      })
    }).with_child(Writer::Semicolon());
    // clang-format on

    generated_body->add_child(gd_class);
    get_or_create_bind_methods_body(target_class, generated_body, generated_sources);

    return GeneratorError::OK;
  }

}  // namespace GodotObjectCompiler