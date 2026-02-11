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
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"
#include "library_godot/assumptions.h"

namespace GodotObjectCompiler {

  Ref<GeneratorError> GodotFunctionGenerator::do_generate(Ref<Class> p_target_class,
      Ref<GodotFunctionAttribute> p_attribute, Ref<Context> p_generated_body, Ref<Context> p_generated_sources,
      Ref<Context> p_generated_global) {
    UNUSED(p_attribute);
    UNUSED(p_generated_global);

    const Ref<Node> target_node = p_attribute->resolve_target();
    GEN_ERROR_COND(target_node == nullptr, p_target_class, "Could not resolve target node for function macro.");

    const Ref<Function> target_function = target_node->as<Function>();
    GEN_ERROR_COND(target_function == nullptr, p_target_class, "Resolved not for function macro is not a function.");

    const Ref<Body> bind_methods_body =
        GodotGeneratorUtils::get_or_create_bind_methods_body(p_target_class, p_generated_body, p_generated_sources);

    const Ref<Parameters> parameters = target_function->parameters();
    GEN_ERROR_COND(!parameters, p_target_class,
        "Function does not name parameters, this was probably parsed as a function call. Abort");

    const Vector<String> parameter_names = vector_transform<Ref<Parameter>, String>(
        parameters->find_children<Parameter>(), [](const Ref<Parameter>& parameter, String& out) {
          out = parameter->name();
          return true;
        });

    const Vector<String> default_values = vector_transform<Ref<Literal>, String>(
        target_function->default_values(), [](const Ref<Literal>& literal, String& out) {
          out = literal->content;
          return true;
        });

    if (target_function->is_static()) {
      bind_methods_body->add_child(GodotGeneratorUtils::bind_static_method(
          p_target_class->name(), target_function->name(), parameter_names, default_values));
    } else if (target_function->is_virtual()) {
      Ref<GeneratorError> error = generate_virtual(
          p_target_class, target_function, p_attribute, p_generated_body, p_generated_sources, bind_methods_body);
      if (error != GeneratorError::OK) {
        return error;
      }
    } else {
      bind_methods_body->add_child(GodotGeneratorUtils::bind_method(
          p_target_class->name(), target_function->name(), parameter_names, default_values));
    }

    Ref<Body> function_names_body =
        GodotGeneratorUtils::get_or_create_function_names_body(p_target_class, p_generated_body);
    GEN_ERROR_COND(!function_names_body, p_attribute, "Failed to get function names body.");
    function_names_body->add_child(
        Writer::Text(format("static const StringName& %s() {static const StringName sn = \"%s\"; return sn; }",
            target_function->name().c_str(), target_function->name().c_str())));

    return GeneratorError::OK;
  }

  Ref<GeneratorError> GodotFunctionGenerator::generate_virtual(const Ref<Class>& p_target_class,
      const Ref<Function>& p_target_function, const Ref<GodotFunctionAttribute>& p_attribute,
      const Ref<Context>& p_generated_body, const Ref<Context>& p_generated_sources,
      const Ref<Context>& p_bind_methods_body) {
    UNUSED(p_attribute);

    StreamWriter writer;
    writer.write("GDVIRTUAL");

    GEN_ERROR_COND(!p_target_function->type(), p_target_function, "Failed to get target function type.")
    GEN_ERROR_COND(!p_target_function->parameters(), p_target_function, "Failed to get target function parameters.")

    Vector<String> parameter_names = vector_transform<Ref<Parameter>, String>(
        p_target_function->parameters()->find_children<Parameter>(), [](const Ref<Parameter>& parameter, String& out) {
          out = parameter->name();
          return true;
        });

    const Vector<String> default_values = vector_transform<Ref<Literal>, String>(
        p_target_function->default_values(), [](const Ref<Literal>& literal, String& out) {
          out = literal->content;
          return true;
        });

    if (p_target_function->parameters()->get_child_count() > 0) {
      writer.write_generic(p_target_function->parameters()->get_child_count());
    }
    bool is_void = p_target_function->type()->name() == "void";
    if (!is_void) {
      writer.write("R");
    }
    if (p_target_function->is_const()) {
      writer.write("C");
    }

    String gdvirtual_macro = writer.get_string();

    Assumption<AssumeType<Define>> GDVIRTUAL = {
        {gdvirtual_macro}, "Assume that a macro with name \"" + gdvirtual_macro + "\" exists in the TypeDB."};
    GDVIRTUAL.validate(TypeDB::instance());
    GEN_ERROR_COND(GDVIRTUAL.is_invalid(), p_target_function, "Invalid macro " + gdvirtual_macro + ". Was not found.");

    String virtual_name = p_target_function->name();
    String virtual_caller_name = string_prefix(p_target_function->name(), "_")
        ? p_target_function->name().substr(1)
        : format("%_virtual", p_target_function->name().c_str());

    // clang-format off
      Ref<Arguments> arguments;
      Ref<Function> gdvirtual = build<Function>().with_children({
        build<Identifier>(GDVIRTUAL().qualified_name),
        build_ref<Arguments>(&arguments),
      }).with_child(Writer::Semicolon());

      Ref<Function> virtual_caller = build<Function>().with_children({
        p_target_function->type()->clone(),
        build<Identifier>(virtual_caller_name),
        p_target_function->parameters()->clone(),
      }).with_child(Writer::Semicolon());

      Ref<Arguments> bind_arguments;
      Ref<Function> gdvirtual_bind = build<Function>().with_children({
        build<Identifier>(AssumedGodotTypes::GDVIRTUAL_BIND().qualified_name),
          build_ref<Arguments>(&bind_arguments).with_child(
            build<Argument>().with_child<Identifier>(virtual_name)
          )
      }).with_child(Writer::Semicolon());

      Ref<Body> func_body;
      Ref<Parameters> func_parameters;
      Ref<Function> func_implementation = build<Function>().with_children({
        p_target_function->type()->clone(),
        build<Identifier>(p_target_class->qualified_name()+ "::" + virtual_caller_name),
      build_ref<Parameters>(&func_parameters),
        build_ref<Body>(&func_body),
      });

      if (p_target_function->parameters()) {
        for (const Ref<Parameter>& parameter : p_target_function->parameters()->find_children<Parameter>()) {
          func_parameters->build_child<Parameter>().with_children({
            parameter->type()->clone(),
            build<Identifier>(parameter->name()),
          });
        }
      }

      if (!is_void) {
        func_body->create_child<Writer::SnippetNode>(format("%s __ret_val__ = {};", p_target_function->type()->type_name().c_str()));
      }

      Ref<Arguments> virtual_call_arguments;
      Ref<Arguments> call_arguments;
      Ref<Function> condition = build<Function>().with_children({
        Writer::Text("!"),
        build<Identifier>(AssumedGodotTypes::GDVIRTUAL_CALL().qualified_name),
        build_ref<Arguments>(&virtual_call_arguments).with_child(
          build<Argument>().with_child<Identifier>(virtual_name)
        )
      });

      func_body->add_child(Writer::If(condition, {
        build<Writer::ListNode>(" ", false, false).with_children({
          Writer::Text("return"),
          build<Function>().with_children({
            build<Identifier>(virtual_name),
            build_ref<Arguments>(&call_arguments)
          }).with_child(Writer::Semicolon())
        })
      }));

      if (!is_void) {
        func_body->create_child<Writer::SnippetNode>("return __ret_val__;");
      }
    // clang-format on

    if (!is_void) {
      arguments->build_child<Argument>().with_child<Identifier>(p_target_function->type()->type_name_unmodified_ptr());
    }

    arguments->build_child<Argument>().with_child<Identifier>(virtual_name);
    for (const Ref<Parameter>& parameter : p_target_function->parameters()->find_children<Parameter>()) {
      GEN_ERROR_COND(!parameter->type(), p_target_function, "Failed to get parameter type.");
      arguments->build_child<Argument>().with_child<Identifier>(parameter->type()->type_name_unmodified_ptr());
      bind_arguments->build_child<Argument>().with_child(Writer::StringLiteral(parameter->name()));
      virtual_call_arguments->build_child<Argument>().with_child<Identifier>(parameter->name());
      call_arguments->build_child<Argument>().with_child<Identifier>(parameter->name());
    }

    if (!is_void) {
      virtual_call_arguments->build_child<Argument>().with_child<Identifier>("__ret_val__");
    }

    p_generated_sources->add_child(func_implementation);
    p_generated_body->add_children({virtual_caller, gdvirtual});
    p_bind_methods_body->add_children({gdvirtual_bind,
        GodotGeneratorUtils::bind_method(
            p_target_class->name(), virtual_caller_name, parameter_names, default_values)});

    return GeneratorError::OK;
  }

}
