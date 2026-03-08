/**************************************************************************/
/* godot_signal_generator.cpp                                             */
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

#include "godot_signal_generator.h"

#include "godot_generator_utils.h"

namespace GodotObjectCompiler {

Ref<GeneratorError> GodotSignalGenerator::do_generate(Ref<Class> p_target_class,
		Ref<GodotSignalAttribute> p_attribute, ClassGeneratorResult &r_result) {
	Ref<Context> p_generated_body = r_result.generated_body;
	Ref<Context> p_generated_sources = r_result.generated_sources;
	Ref<Context> p_generated_global = r_result.generated_global;

	UNUSED(p_generated_global);

	using namespace GodotGeneratorUtils;
	const Ref<Body> bind_methods = get_bind_methods_body(p_target_class, p_generated_body, p_generated_sources);
	GEN_ERROR_COND(!bind_methods, p_target_class, "Failed to get or generate bind methods body.");

	Ref<Function> target_function = p_attribute->TargetFunction();
	GEN_ERROR_COND(!target_function, p_target_class, "Failed to get signal target function");

	const bool is_void = target_function->type()->name() == "void";
	GEN_ERROR_COND(!is_void, target_function, "Signal target function does not return void.");

	// clang-format off
    Ref<Arguments> arguments;
    bind_methods->build_child<Function>().with_children({
        build<Identifier>("ADD_SIGNAL"),
        build<Arguments>().with_child(
          build<Argument>().with_children( {
      build<Function>().with_children({
        build<Identifier>("MethodInfo"),
        build_ref<Arguments>(&arguments).with_children({
            build<Argument>().with_child(
              Output::StringLiteral(target_function->name())
            ),
          })
        })
      }))
    }).with_child(Output::Semicolon());

    Ref<Parameters> func_parameters;
    Ref<Arguments> emit_arguments;

    p_generated_sources->build_child<Function>().with_children({
      build<Type>().with_child<Identifier>("void"),
      build<Identifier>(p_target_class->qualified_name() + "::" + target_function->name()),
      build_ref<Parameters>(&func_parameters),
      build<Body>().with_children({
        build<Function>().with_children({
          build<Identifier>("emit_signal"),
          build_ref<Arguments>(&emit_arguments).with_children({
            build<Argument>().with_child(Output::StringLiteral(target_function->name())),
          })
        }).with_child(Output::Semicolon()),
      })
    });
	// clang-format on

	Size i = 1;
	for (const Ref<Parameter> &parameter : target_function->parameters()->find_children<Parameter>()) {
		Ref<Type> type = parameter->find_child<Type>();
		GEN_ERROR_COND(!type, target_function, "Failed to get function argument type.");
		type = type->qualified();

		Ref<Identifier> identifier = parameter->find_child<Identifier>();
		String name = identifier ? identifier->name : format("p_param_%d", i);

		arguments->build_child<Argument>().with_child(build_property_info_defaults(type, name, DEFAULTS_SIGNAL_ARGUMENT));

		func_parameters->build_child<Parameter>().with_children({
				type->clone(),
				build<Identifier>(name),
		});

		emit_arguments->build_child<Argument>().with_children({
				build<Identifier>(name),
		});

		i += 1;
	}

	const Ref<Body> signal_names_body = get_signal_names_body(p_target_class, p_generated_body);
	GEN_ERROR_COND(!signal_names_body, p_attribute, "Failed to get signal names body.");
	signal_names_body->add_child(
			Output::Text(format("static const StringName& %s() {static const StringName sn = \"%s\"; return sn; }",
					target_function->name().c_str(), target_function->name().c_str())));

	return GeneratorError::OK;
}

} //namespace GodotObjectCompiler