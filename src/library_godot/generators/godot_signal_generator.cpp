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

namespace GodotObjectCompiler
{

    Ref<GeneratorError> GodotSignalGenerator::bind_signal(
        Ref<Class> p_target_class, const Ref<Node>& p_current_node, const String& p_signal_name,
        const Ref<Parameters>& p_parameters, ClassGeneratorResult& r_result)
    {
        Ref<Context> p_generated_body = r_result.generated_body;
        Ref<Context> p_generated_sources = r_result.generated_sources;
        Ref<Context> p_generated_global = r_result.generated_global;

        using namespace GodotGeneratorUtils;
        const Ref<Body> bind_methods =
            get_bind_methods_body(p_target_class, p_generated_body, p_generated_sources);
        GEN_ERR_COND(!bind_methods, p_target_class, "Failed to get or generate bind methods body.");

        Ref<Arguments> arguments;

        Result<Function> signal_result =
            add_signal(p_target_class, p_signal_name, p_parameters, r_result);
        RESULT_ERROR_PASS_ON(GeneratorError, signal_result, signal);

        bind_methods->add_child(signal);

        Ref<Parameters> func_parameters = node_new<Parameters>();
        Ref<Arguments> emit_arguments = node_new<Arguments>();

        Size i = 1;
        for (const Ref<Parameter>& parameter : p_parameters->find_children<Parameter>()) {
            Ref<Type> type = parameter->find_child<Type>();
            GEN_ERR_COND(!type, p_current_node, "Failed to get function argument type.");
            type = type->qualified();
            const Ref<Identifier> identifier = parameter->find_child<Identifier>();
            const String name = identifier ? identifier->name : format("p_param_%d", i);

            func_parameters->B<Parameter>()[{
                type->clone(),
                B<Identifier>(name),
            }];

            emit_arguments->B<Argument>()[{
                B<Identifier>(name),
            }];

            i += 1;
        }

        p_generated_sources->B<Function>()[{
            B<Type>()[B<Identifier>("void")],
            B<Identifier>(p_target_class->qualified_name() + "::" + p_signal_name),
            func_parameters,
            B<Body>()[emit_signal(p_signal_name, emit_arguments)],
        }];

        const Ref<Body> signal_names_body = get_signal_names_body(p_target_class, p_generated_body);
        GEN_ERR_COND(!signal_names_body, p_current_node, "Failed to get signal names body.");

        signal_names_body->add_child(Output::Text(format(
            "static const StringName& %s() {static const StringName sn = \"%s\"; return sn; }",
            p_signal_name.c_str(), p_signal_name.c_str())));

        return GeneratorError::OK;
    }

    Ref<GeneratorError> GodotSignalGenerator::do_generate_default_attribute_arguments(
        Ref<Class> p_target_class, Ref<GodotSignalAttribute> p_attribute,
        Ref<Context> p_default_values)
    {
        UNUSED(p_target_class);
        UNUSED(p_attribute);
        p_default_values->add_children({B<StringLiteralArgument>()[B<Literal>("")]});
        return GeneratorError::OK;
    }

    Ref<GeneratorError> GodotSignalGenerator::do_generate(
        Ref<Class> p_target_class, Ref<GodotSignalAttribute> p_attribute,
        ClassGeneratorResult& r_result)
    {
        Ref<Function> target_function = p_attribute->TargetFunction();
        GEN_ERR_COND(!target_function, p_target_class, "Failed to get signal target function");

        auto function_type = target_function->type();
        GEN_ERR_COND(!function_type, target_function, "Failed to get function type.");

        const bool is_void = function_type->name() == "void";
        GEN_ERR_COND(!is_void, target_function, "Signal target function does not return void.");

        String signal_name = target_function->name();
        Ref<Literal> name_literal =
            p_attribute->arguments()->find_chain<Literal, StringLiteralArgument>();
        if (String unwrapped; name_literal->unwrap_string_literal(unwrapped)) {
            signal_name = unwrapped;
        }

        return bind_signal(
            p_target_class, target_function, signal_name, target_function->parameters(), r_result);
    }

} // namespace GodotObjectCompiler
