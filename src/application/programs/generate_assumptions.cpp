/**************************************************************************/
/* generate_assumptions.cpp                                               */
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
#ifdef DEV_BUILD
#include "generate_assumptions.h"

#include "application/application_context.h"
#include "generate_type_db.h"
#include "library/core/core.h"
#include "library/core/permissions.h"
#include "library/core/string_utilities.h"
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
#include "library_godot/attributes/godot_class_type.h"
#include "library_godot/attributes/godot_module_init_level.h"
#include "library_godot/attributes/godot_property_hint.h"
#include "library_godot/attributes/godot_property_usage_flags.h"
#include "library_godot/attributes/godot_rpc.h"
#include "library_godot/attributes/godot_variant_type.h"
#include "library_godot/attributes/godot_virtual.h"
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
    return Output::Text(format);
  }

  Ref<ProgramError> GenerateAssumptions::run(ApplicationContext& p_context) {
    UNUSED(p_context);

    GenerateTypeDB generate_type_db;
    PROG_ERR_COND(generate_type_db.run(p_context) != ProgramError::OK, "Failed to generate the type db.");

    Permissions::instance()->add_write_path("src/library_godot/generated_assumptions");
    String header_path = "src/library_godot/generated_assumptions/parameter_types.h";
    String source_path = "src/library_godot/generated_assumptions/parameter_types.cpp";

    Vector<Ref<IAttributeParameterType>> parameter_types = {
        make_ref<GodotClassTypeParameterType>(),
        make_ref<GodotModuleInitializationLevelParameterType>(),
        make_ref<GodotVariantTypeParameterType>(),
        make_ref<GodotPropertyHintParameterType>(),
        make_ref<GodotPropertyUsageFlagsParameterType>(),
        make_ref<GodotVirtualParameterType>(),
        make_ref<GodotRpcModeParameterType>(),
        make_ref<GodotRpcSyncParameterType>(),
        make_ref<GodotRpcTransferModeParameterType>(),
        make_ref<GodotRpcChannelParameterType>(),
    };

    Ref<Body> header_body;
    Ref<Body> source_body;
    // clang-format off
    Ref<Context> header_content = build<Context>().with_children({
      Output::PragmaOnce(),
      Output::Include("library/core/assumption.h"),
      build<Namespace>().with_children({
          build<Identifier>("GodotObjectCompiler"),
        build<Body>().with_child(
          build<Namespace>().with_children({build<Identifier>("AssumedParameterValues"), build_ref<Body>(&header_body)})
          )
        })
    });

    Ref<Context> source_content = build<Context>().with_children({
      Output::Include("parameter_types.h"),
      Output::Include("library_godot/attributes/godot_class_type.h"),
      Output::Include("library_godot/attributes/godot_module_init_level.h"),
      Output::Include("library_godot/attributes/godot_variant_type.h"),
      Output::Include("library_godot/attributes/godot_property_hint.h"),
      Output::Include("library_godot/attributes/godot_property_usage_flags.h"),
      Output::Include("library_godot/attributes/godot_rpc.h"),
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
      Output::Semicolon()
    });

    source_body->build_child<Function>().with_children({
      build<Type>().with_child<Identifier>("bool"),
    build<Identifier>("validate_assumptions"),
      build<Parameters>(),
    build_ref<Body>(&validate_body)
    });
    // clang-format on

    validate_body->add_child(Output::Text("bool success = true;"));

    for (const Ref<IAttributeParameterType>& parameter_type : parameter_types) {
      Ref<Body> inner_body = validate_body->build_child<Body>();
      String format = "Ref<PARAM_TYPE> validator = make_ref<PARAM_TYPE>();";
      inner_body->add_child(Output::Text(string_replace(format, "PARAM_TYPE", parameter_type->get_type())));

      for (const String& value_name : parameter_type->get_value_names()) {
        String validate_format = "success &= VALUE_NAME.validate(validator.get()) == STATE_VALID;";
        inner_body->add_child(Output::Text(string_replace(validate_format, "VALUE_NAME", value_name)));
      }
    }
    validate_body->add_child(Output::Text("return success;"));

    OutputTransformator transformator;
    FileWriter header_writer(header_path, true);
    FileWriter source_writer(source_path, true);

    transformator
        .transform(
            build<Output::EnclosingNode>("// clang-format off\n", "\n//clang-format on").with_child(header_content))
        ->get_output(&header_writer);

    transformator
        .transform(
            build<Output::EnclosingNode>("// clang-format off\n", "\n//clang-format on").with_child(source_content))
        ->get_output(&source_writer);

    return ProgramError::OK;
  }

}
#endif