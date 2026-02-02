#include "application/application_context.h"
#include "library/core/core.h"
#include "library/core/helpers.h"
#include "library/core/string_writer.h"
#include "library/generator/attribute_parameter_type.h"
#include "library/tree/output/output.h"
#include "library/tree/output/output_transformator.h"
#include "library/tree/syntax/context.h"
#include "library/tree/syntax/function.h"
#include "library/tree/syntax/identifier.h"
#include "library/tree/syntax/namespace.h"
#include "library/tree/syntax/node.h"
#include "library/tree/syntax/type.h"
#include "library_godot/assumptions.h"
#include "library_godot/attributes/godot_class_type.h"
#include "library_godot/attributes/godot_module_init_level.h"
#include "library_godot/attributes/godot_property_hint.h"
#include "library_godot/attributes/godot_property_usage_flags.h"
#include "library_godot/attributes/godot_variant_type.h"
#include "program.h"
#ifdef DEV_BUILD
#include "generate_assumptions.h"
#include "library/core/string_writer.h"
#include "library/tree/output/output.h"
#include "library/tree/output/output_transformator.h"
#include "library_godot/attributes/godot_class_type.h"
#include "library_godot/attributes/godot_module_init_level.h"
#include "library_godot/attributes/godot_property_hint.h"
#include "library_godot/attributes/godot_property_usage_flags.h"
#include "library_godot/attributes/godot_variant_type.h"
#include "program.h"

namespace GodotObjectCompiler {

  Ref<Node> generate_value_name_assumption(const String& return_type, const String& value_name) {
    String format =
        "inline Assumption<String> VALUE_NAME {\n"
        "\"VALUE_NAME\",\n"
        "\"Assume that the value \\\"VALUE_NAME\\\" exists in the RETURN_TYPE generated type.\"\n"
        "};";

    format = string_replace(format, "VALUE_NAME", value_name);
    format = string_replace(format, "RETURN_TYPE", return_type);
    return Writer::Text(format);
  }

  Ref<ProgramError> GenerateAssumptions::run(ApplicationContext& context) {
    String header_path = "./library_godot/generated_assumptions/parameter_types.h";
    String source_path = "./library_godot/generated_assumptions/parameter_types.cpp";

    Vector<Ref<IAttributeParameterType>> parameter_types = {
        make_ref<GodotClassTypeParameterType>(),
        make_ref<GodotModuleInitializationLevelParameterType>(),
        make_ref<GodotVariantTypeParameterType>(),
        make_ref<GodotPropertyHintParameterType>(),
        make_ref<GodotPropertyUsageFlagsParameterType>(),
    };

    Ref<Body> header_body;
    Ref<Body> source_body;
    // clang-format off
    Ref<Context> header_content = build<Context>().with_children({
      Writer::PragmaOnce(),
      Writer::Include("library/core/assumption.h"),
      build<Namespace>().with_children({
          build<Identifier>("GodotObjectCompiler"),
        build<Body>().with_child(
          build<Namespace>().with_children({build<Identifier>("AssumedParameterValues"), build_ref<Body>(&header_body)})
          )
        })
    });

    Ref<Context> source_content = build<Context>().with_children({
      Writer::Include("parameter_types.h"),
      Writer::Include("library_godot/attributes/godot_class_type.h"),
      Writer::Include("library_godot/attributes/godot_module_init_level.h"),
      Writer::Include("library_godot/attributes/godot_variant_type.h"),
      Writer::Include("library_godot/attributes/godot_property_hint.h"),
      Writer::Include("library_godot/attributes/godot_property_usage_flags.h"),
      build<Namespace>().with_children({
          build<Identifier>("GodotObjectCompiler"),
        build<Body>().with_child(
          build<Namespace>().with_children({build<Identifier>("AssumedParameterValues"), build_ref<Body>(&source_body)})
          )
        })
    });
    // clang-format on

    for (const Ref<IAttributeParameterType>& parameter_type : parameter_types) {
      for (const String& value_name : parameter_type->get_value_names()) {
        header_body->add_child(generate_value_name_assumption(parameter_type->get_return_type(), value_name));
      }
    }

    Ref<Body> validate_body;
    // clang-format off
    header_body->build_child<Function>().with_children({
        build<Type>().with_child<Identifier>("bool"),
      build<Identifier>("validate_assumptions"),
        build<Parameters>(),
      Writer::Semicolon()
    });

    source_body->build_child<Function>().with_children({
      build<Type>().with_child<Identifier>("bool"),
    build<Identifier>("validate_assumptions"),
      build<Parameters>(),
    build_ref<Body>(&validate_body)
    });
    // clang-format on

    validate_body->add_child(Writer::Text("bool success = true;"));

    for (const Ref<IAttributeParameterType>& parameter_type : parameter_types) {
      Ref<Body> inner_body = validate_body->build_child<Body>();
      String format = "Ref<PARAM_TYPE> validator = make_ref<PARAM_TYPE>();";
      inner_body->add_child(Writer::Text(string_replace(format, "PARAM_TYPE", parameter_type->get_type())));

      for (const String& value_name : parameter_type->get_value_names()) {
        String validate_format = "success &= VALUE_NAME.validate(validator.get()) == STATE_VALID;";
        inner_body->add_child(Writer::Text(string_replace(validate_format, "VALUE_NAME", value_name)));
      }
    }
    validate_body->add_child(Writer::Text("return success;"));

    OutputTransformator transformator;
    FileWriter header_writer(header_path, true);
    FileWriter source_writer(source_path, true);

    transformator
        .transform(
            build<Writer::EnclosingNode>("// clang-format off\n", "\n//clang-format on").with_child(header_content))
        ->get_output(&header_writer);

    transformator
        .transform(
            build<Writer::EnclosingNode>("// clang-format off\n", "\n//clang-format on").with_child(source_content))
        ->get_output(&source_writer);

    return ProgramError::OK;
  }

}  // namespace GodotObjectCompiler
#endif