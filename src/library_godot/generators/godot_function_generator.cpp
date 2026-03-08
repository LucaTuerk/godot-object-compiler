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
#include "library/core/string_writer.h"
#include "library_godot/assumptions.h"

namespace GodotObjectCompiler {

Ref<GeneratorError> GodotFunctionGenerator::do_generate_default_attribute_arguments(
		Ref<Class> p_target_class, Ref<GodotFunctionAttribute> p_attribute, Ref<Context> p_default_values) {
	UNUSED(p_target_class);
	UNUSED(p_attribute);
	// clang-format off
    p_default_values->add_children({
      build<GodotVirtualArgument>().with_child<Identifier>(AssumedParameterValues::NoVirtual()).with_child<Arguments>(),
      build<GodotRpcModeArgument>().with_child<Identifier>(AssumedParameterValues::Disabled()).with_child<Arguments>(),
      build<GodotRpcSyncArgument>().with_child<Identifier>(AssumedParameterValues::CallRemote()).with_child<Arguments>(),
      build<GodotRpcTransferModeArgument>().with_child<Identifier>(AssumedParameterValues::Reliable()).with_child<Arguments>(),
      build<GodotRpcChannelArgument>().with_children({
        build<Identifier>("Channel"),
        build<Arguments>().with_child(
          build<Argument>().with_child<Literal>("0"))
      })
    });
	// clang-format on
	return GeneratorError::OK;
}

Ref<GeneratorError> GodotFunctionGenerator::do_generate(Ref<Class> p_target_class,
		Ref<GodotFunctionAttribute> p_attribute, ClassGeneratorResult &r_result) {
	Ref<Context> p_generated_body = r_result.generated_body;
	Ref<Context> p_generated_sources = r_result.generated_sources;
	Ref<Context> p_generated_global = r_result.generated_global;

	UNUSED(p_attribute);
	UNUSED(p_generated_global);

	const Ref<Node> target_node = p_attribute->resolve_target();
	GEN_ERROR_COND(target_node == nullptr, p_target_class, "Could not resolve target node for function macro.");

	const Ref<Function> target_function = target_node->as<Function>();
	GEN_ERROR_COND(target_function == nullptr, p_target_class, "Resolved not for function macro is not a function.");

	const Ref<GodotRpcModeArgument> rpc_mode_argument = p_attribute->arguments()->find_child<GodotRpcModeArgument>();
	GEN_ERROR_COND(rpc_mode_argument == nullptr, p_attribute, "Failed to find rpc mode argument.");

	const Ref<GodotVirtualArgument> virtual_argument = p_attribute->arguments()->find_child<GodotVirtualArgument>();
	GEN_ERROR_COND(virtual_argument == nullptr, p_attribute, "Failed to find virtual argument.");

	const Ref<Body> bind_methods_body =
			GodotGeneratorUtils::get_bind_methods_body(p_target_class, p_generated_body, p_generated_sources);

	const Ref<Parameters> parameters = target_function->parameters();
	GEN_ERROR_COND(!parameters, p_target_class,
			"Function does not name parameters, this was probably parsed as a function call. Abort");

	Ref<Context> generated_public_members, generated_protected_members, generated_private_members;
	GEN_ERROR_COND(GodotGeneratorUtils::unzip_generated_body(p_generated_body, &generated_public_members,
						   &generated_protected_members, &generated_private_members) != GeneratorError::OK,
			p_target_class, "Failed to find generated body groups");

	const Vector<String> parameter_names = vector_transform<Ref<Parameter>, String>(
			parameters->find_children<Parameter>(), [](const Ref<Parameter> &parameter, String &out) {
				out = parameter->name();
				return true;
			});

	const Vector<String> default_values = vector_transform<Ref<Literal>, String>(
			target_function->default_values(), [](const Ref<Literal> &literal, String &out) {
				out = literal->content;
				return true;
			});

	if (target_function->is_static()) {
		bind_methods_body->add_child(GodotGeneratorUtils::bind_static_method(
				p_target_class->name(), target_function->name(), parameter_names, default_values));
	} else if (virtual_argument->is_script_virtual()) {
		Ref<GeneratorError> error = generate_virtual(
				p_target_class, target_function, p_attribute, bind_methods_body, r_result);
		if (error != GeneratorError::OK) {
			return error;
		}
	} else {
		bind_methods_body->add_child(GodotGeneratorUtils::bind_method(
				p_target_class->name(), target_function->name(), parameter_names, default_values));
	}

	if (rpc_mode_argument->rpc_mode() != AssumedParameterValues::Disabled()) {
		Ref<GeneratorError> error = generate_rpc(p_target_class, target_function, rpc_mode_argument,
				p_attribute->arguments()->find_child<GodotRpcTransferModeArgument>(),
				p_attribute->arguments()->find_child<GodotRpcSyncArgument>(),
				p_attribute->arguments()->find_child<GodotRpcChannelArgument>(), p_generated_body, p_generated_sources);

		if (error != GeneratorError::OK) {
			return error;
		}
	}

	generated_public_members->add_child(Output::FmtText(
			"%s %s_callable{this, \"%s\"};",
			AssumedGodotTypes::Callable().type->qualified_name().c_str(),
			target_function->name().c_str(),
			target_function->name().c_str()));

	const Ref<Body> function_names_body =
			GodotGeneratorUtils::get_function_names_body(p_target_class, p_generated_body);
	GEN_ERROR_COND(!function_names_body, p_attribute, "Failed to get function names body.");
	function_names_body->add_child(
			Output::Text(format("static const %s& %s() {static const %s sn = \"%s\"; return sn; }",
					AssumedGodotTypes::StringName().type->qualified_name().c_str(),
					target_function->name().c_str(),
					AssumedGodotTypes::StringName().type->qualified_name().c_str(),
					target_function->name().c_str())));

	r_result.header_includes.insert(AssumedGodotTypes::StringName().type->header);
	r_result.header_includes.insert(AssumedGodotTypes::Callable().type->header);
	return GeneratorError::OK;
}

Ref<GeneratorError> GodotFunctionGenerator::generate_virtual(const Ref<Class> &p_target_class,
		const Ref<Function> &p_target_function, const Ref<GodotFunctionAttribute> &p_attribute,
		const Ref<Context> &p_bind_methods_body, ClassGeneratorResult &r_result) {
	Ref<Context> p_generated_body = r_result.generated_body;
	Ref<Context> p_generated_sources = r_result.generated_sources;
	Ref<Context> p_generated_global = r_result.generated_global;
	UNUSED(p_attribute);

	Ref<GodotVirtualArgument> virtual_argument = p_attribute->arguments()->find_child<GodotVirtualArgument>();
	GEN_ERROR_COND(!virtual_argument, p_attribute, "Failed to get virtual argument.");

	StreamWriter writer;
	writer.write("GDVIRTUAL");

	Ref<Type> function_type = p_target_function->type();
	GEN_ERROR_COND(!function_type, p_attribute, "Failed to get function type.");
	function_type = function_type->qualified();

	GEN_ERROR_COND(!p_target_function->parameters(), p_target_function, "Failed to get target function parameters.")

	Ref<Context> generated_private_members;
	GEN_ERROR_COND(GodotGeneratorUtils::unzip_generated_body(
						   p_generated_body, nullptr, nullptr, &generated_private_members) != GeneratorError::OK,
			p_target_class, "Failed to find generated body groups");

	Vector<String> parameter_names = vector_transform<Ref<Parameter>, String>(
			p_target_function->parameters()->find_children<Parameter>(), [](const Ref<Parameter> &parameter, String &out) {
				out = parameter->name();
				return true;
			});

	const Vector<String> default_values = vector_transform<Ref<Literal>, String>(
			p_target_function->default_values(), [](const Ref<Literal> &literal, String &out) {
				out = literal->content;
				return true;
			});

	writer.write_generic(p_target_function->parameters()->get_child_count());

	bool is_void = p_target_function->type()->name() == "void";
	if (!is_void) {
		writer.write("R");
	}
	if (p_target_function->is_const()) {
		writer.write("C");
	}

	if (virtual_argument->is_required()) {
		writer.write("_REQUIRED");
	}

	String macro = writer.get_string();

	Assumption<AssumeType<Define>> GDVIRTUAL = {
		{ macro }, "Assume that a macro with name \"" + macro + "\" exists in the TypeDB."
	};
	GDVIRTUAL.validate(ExecutionContext::instance()->get_type_db());
	GEN_ERROR_COND(GDVIRTUAL.is_invalid(), p_target_function, "Invalid macro " + macro + ". Was not found.");

	String virtual_name = format("_%s", p_target_function->name().c_str());
	String virtual_caller_name = virtual_name;

	// clang-format off
    Ref<Arguments> arguments;
    Ref<Function> gdvirtual = build<Function>().with_children({
      build<Identifier>(GDVIRTUAL().type->qualified_name()),
      build_ref<Arguments>(&arguments),
    }).with_child(Output::Semicolon());

    Ref<Function> virtual_caller = build<Function>().with_children({
      function_type->clone(),
      build<Identifier>(virtual_caller_name),
      p_target_function->parameters()->clone(),
    }).with_child(Output::Semicolon());

    Ref<Arguments> bind_arguments;
    Ref<Function> gdvirtual_bind = build<Function>().with_children({
      build<Identifier>(AssumedGodotTypes::GDVIRTUAL_BIND().type->name()),
        build_ref<Arguments>(&bind_arguments).with_child(
          build<Argument>().with_child<Identifier>(virtual_name)
        )
    }).with_child(Output::Semicolon());

    Ref<Body> func_body;
    Ref<Parameters> func_parameters;
    Ref<Function> func_implementation = build<Function>().with_children({
      function_type->clone(),
      build<Identifier>(p_target_class->qualified_name()+ "::" + virtual_caller_name),
    build_ref<Parameters>(&func_parameters),
      build_ref<Body>(&func_body),
    });

    if (p_target_function->parameters()) {
      for (const Ref<Parameter>& parameter : p_target_function->parameters()->find_children<Parameter>()) {
      	Ref<Type> parameter_type = parameter->type();
      	GEN_ERROR_COND(!parameter_type, p_attribute, "Failed to get paramter type.");
      	parameter_type = parameter_type->qualified();

        func_parameters->build_child<Parameter>().with_children({
          parameter_type->clone(),
          build<Identifier>(parameter->name()),
        });
      }
    }

    if (!is_void) {
      func_body->create_child<Output::SnippetNode>(format("%s return_value = {};", function_type->type_name().c_str()));
    }

    Ref<Arguments> virtual_call_arguments;
    Ref<Arguments> call_arguments;
    Ref<Function> condition = build<Function>().with_children({
      Output::Text("!"),
      build<Identifier>(AssumedGodotTypes::GDVIRTUAL_CALL().type->name()),
      build_ref<Arguments>(&virtual_call_arguments).with_child(
        build<Argument>().with_child<Identifier>(virtual_name)
      )
    });

    func_body->add_child(Output::If(condition, {
      build<Output::ListNode>(" ", false, false).with_children({
        Output::Text("return"),
        build<Function>().with_children({
          build<Identifier>(p_target_function->name()),
          build_ref<Arguments>(&call_arguments)
        }).with_child(Output::Semicolon())
      })
    }));

    if (!is_void) {
      func_body->create_child<Output::SnippetNode>("return return_value;");
    }
	// clang-format on

	if (!is_void) {
		arguments->build_child<Argument>().with_child<Identifier>(function_type->type_name_unmodified_ptr());
	}

	arguments->build_child<Argument>().with_child<Identifier>(virtual_name);
	for (const Ref<Parameter> &parameter : p_target_function->parameters()->find_children<Parameter>()) {
		Ref<Type> parameter_type = parameter->type();
		GEN_ERROR_COND(!parameter_type, p_target_function, "Failed to get parameter type.");
		parameter_type = parameter_type->qualified();

		arguments->build_child<Argument>().with_child<Identifier>(parameter_type->type_name_unmodified_ptr());
		bind_arguments->build_child<Argument>().with_child(Output::StringLiteral(parameter->name()));
		virtual_call_arguments->build_child<Argument>().with_child<Identifier>(parameter->name());
		call_arguments->build_child<Argument>().with_child<Identifier>(parameter->name());
	}

	if (!is_void) {
		virtual_call_arguments->build_child<Argument>().with_child<Identifier>("return_value");
	}

	p_generated_sources->add_child(func_implementation);
	generated_private_members->add_children({ virtual_caller, gdvirtual });
	p_bind_methods_body->add_children({ gdvirtual_bind,
			GodotGeneratorUtils::bind_method_as(
					p_target_class->name(), p_target_function->name(), virtual_caller_name, parameter_names, default_values) });

	r_result.header_includes.insert(GDVIRTUAL().type->header);
	r_result.source_includes.insert(AssumedGodotTypes::GDVIRTUAL_BIND().type->header);
	r_result.source_includes.insert(AssumedGodotTypes::GDVIRTUAL_CALL().type->header);

	return GeneratorError::OK;
}

Ref<GeneratorError> GodotFunctionGenerator::generate_rpc(const Ref<Class> &p_target_class,
		const Ref<Function> &p_target_function, const Ref<GodotRpcModeArgument> &p_rpc_mode,
		const Ref<GodotRpcTransferModeArgument> &p_transport_mode, const Ref<GodotRpcSyncArgument> &p_sync,
		const Ref<GodotRpcChannelArgument> &p_channel, const Ref<Context> &p_generated_body,
		const Ref<Context> &p_generated_sources) {
	PANIC_COND(!p_target_class, "Target class not found");
	PANIC_COND(!p_target_function, "Target function not found");
	GEN_ERROR_COND(!p_rpc_mode, p_target_function, "Target rpc mode not found");
	GEN_ERROR_COND(!p_transport_mode, p_target_function, "Target rpc transport mode not found");
	GEN_ERROR_COND(!p_sync, p_target_function, "Target rpc sync not found");
	GEN_ERROR_COND(!p_channel, p_target_function, "Target rpc channel not found");
	GEN_ERROR_COND(!p_generated_body, p_target_class, "Generated body not found.");
	GEN_ERROR_COND(!p_generated_sources, p_target_class, "Generated sources not found.");

	const Ref<Body> notification_body =
			GodotGeneratorUtils::get_notification_body(p_target_class, p_generated_body, p_generated_sources);
	const Ref<Body> condition_body =
			GodotGeneratorUtils::get_if_body(notification_body, "p_notification == NOTIFICATION_ENTER_TREE");

	// clang-format off
    condition_body->add_child(
      build<Body>().with_children({
        Output::Text("Dictionary opts;"),
        Output::FmtText("opts[\"rpc_mode\"] = MultiplayerAPI::%s;", p_rpc_mode->godot_rpc_mode().c_str()),
        Output::FmtText("opts[\"transfer_mode\"] = MultiplayerPeer::%s;", p_transport_mode->transfer_mode().c_str()),
        Output::FmtText("opts[\"call_local\"] = %s;", (p_sync->rpc_sync() == AssumedParameterValues::CallLocal() ? "true" : "false")),
        Output::FmtText("opts[\"channel\"] = %d;", p_channel->channel()),
        Output::FmtText("rpc_config(\"%s\", opts);", p_target_function->name().c_str()),
      })
    );
	// clang-format on

	return GeneratorError::OK;
}

} //namespace GodotObjectCompiler
