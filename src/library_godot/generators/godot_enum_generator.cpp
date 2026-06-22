/**************************************************************************/
/* godot_enum_generator.cpp                                               */
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

#include "godot_enum_generator.h"

#include "godot_generator_utils.h"
#include "library_godot/assumptions.h"

namespace GodotObjectCompiler
{

    Ref<GeneratorError> GodotEnumGenerator::do_generate_default_attribute_arguments(
        Ref<Class> p_target_class, Ref<GodotEnumAttribute> p_attribute,
        Ref<Context> p_default_values)
    {
        UNUSED(p_target_class);
        UNUSED(p_attribute);
        p_default_values->B<EnumGeneratorOptionsArgument>()[B<Identifier>(
            EnumGeneratorOptionsArgument::EnumDefault)];
        return GeneratorError::OK;
    }

    Ref<GeneratorError> GodotEnumGenerator::do_generate(
        Ref<Class> p_target_class, Ref<GodotEnumAttribute> p_attribute,
        ClassGeneratorResult& r_result)
    {
        Ref<Context> p_generated_body = r_result.generated_body;
        Ref<Context> p_generated_sources = r_result.generated_sources;
        Ref<Context> p_generated_global = r_result.generated_global;

        const Ref<Node> target_node = p_attribute->resolve_target();
        GEN_ERR_COND(!target_node, p_target_class, "Could not resolve target for enum marco.");

        const Ref<Enum> target_enum = target_node->as<Enum>();
        GEN_ERR_COND(
            !target_enum, p_target_class,
            "Resolved target for enum macro is not an enum, but " + target_node->get_type());

        const Ref<Identifier> enum_options_identifier =
            p_attribute->find_chain<Identifier, Arguments, EnumGeneratorOptionsArgument>();
        GEN_ERR_COND(!enum_options_identifier, p_attribute, "Invalid enum options argument.");

        const Ref<Define> cast_define =
            enum_options_identifier->name == EnumGeneratorOptionsArgument::EnumDefault
                ? AssumedGodotTypes::VARIANT_ENUM_CAST().type
            : enum_options_identifier->name == EnumGeneratorOptionsArgument::EnumFlags
                ? AssumedGodotTypes::VARIANT_BITFIELD_CAST().type
                : nullptr;

        const Ref<Define> bind_define =
            enum_options_identifier->name == EnumGeneratorOptionsArgument::EnumDefault
                ? AssumedGodotTypes::BIND_ENUM_CONSTANT().type
            : enum_options_identifier->name == EnumGeneratorOptionsArgument::EnumFlags
                ? AssumedGodotTypes::BIND_BITFIELD_FLAG().type
                : nullptr;

        GEN_ERR_COND(
            cast_define == nullptr || bind_define == nullptr, p_attribute,
            "Unknown enum options name");

        if (p_target_class) {
            const Ref<Body> bind_methods_body = GodotGeneratorUtils::get_bind_methods_body(
                p_target_class, p_generated_body, p_generated_sources);

            for (const String& name : target_enum->value_names()) {
                bind_methods_body->B<Function>()[{
                    B<Identifier>(bind_define->name()),
                    B<Arguments>()[B<Argument>()[Output::Text(name)]],
                }][Output::Semicolon()];
            }
        }

        p_generated_global->B<Function>()[{
            B<Identifier>(cast_define->name()),
            B<Arguments>()[B<Argument>()[Output::Text(target_enum->qualified_name())]],
        }][Output::Semicolon()];

        return GeneratorError::OK;
    }

} // namespace GodotObjectCompiler
