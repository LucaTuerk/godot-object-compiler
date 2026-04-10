/**************************************************************************/
/* godot_hint_generators.cpp                                              */
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

#include "godot_hint_generators.h"

#include "godot_generator_utils.h"
#include "library_godot/assumptions.h"
#include "library_godot/generated_assumptions/parameter_types.h"

namespace GodotObjectCompiler
{

    Ref<GeneratorError> GodotCategoryGenerator::do_generate_default_attribute_arguments(
        Ref<Class> p_target_class, Ref<GodotPropertyCategoryAttribute> p_attribute,
        Ref<Context> p_default_values)
    {
        UNUSED(p_target_class);
        UNUSED(p_attribute);
        p_default_values->B<StringLiteralArgument>()[B<Literal>("\"\"")];
        return GeneratorError::OK;
    }

    Ref<GeneratorError> GodotCategoryGenerator::do_generate(
        Ref<Class> p_target_class, Ref<GodotPropertyCategoryAttribute> p_attribute,
        ClassGeneratorResult& r_result)
    {
        using namespace GodotGeneratorUtils;

        const Ref<Context> p_generated_body = r_result.generated_body;
        const Ref<Context> p_generated_sources = r_result.generated_sources;

        const Ref<Body> bind_methods_body =
            get_bind_methods_body(p_target_class, p_generated_body, p_generated_sources);
        GEN_ERROR_COND(!bind_methods_body, p_attribute, "Failed to get bind_methods body.");

        bind_methods_body->B<Function>()[{
            B<Identifier>(AssumedGodotTypes::ADD_GROUP().type->name()),
            B<Arguments>()[{
                B<Argument>()[Output::StringLiteral(p_attribute->literal_content())],
                B<Argument>()[Output::StringLiteral("")]}]}][Output::Semicolon()];

        ONCE(PRINT_WARNING(
            "Trying to add property category. Categories can not be bound correctly due to "
            "issue https://github.com/godotengine/godot-proposals/issues/14274. Binding a property "
            "group instead."));
        return GeneratorError::OK;
    }

    Ref<GeneratorError> GodotGroupGenerator::do_generate_default_attribute_arguments(
        Ref<Class> p_target_class, Ref<GodotPropertyGroupAttribute> p_attribute,
        Ref<Context> p_default_values)
    {
        UNUSED(p_target_class);
        UNUSED(p_attribute);
        p_default_values->B<StringLiteralArgument>()[B<Literal>("\"\"")];
        return GeneratorError::OK;
    }

    Ref<GeneratorError> GodotGroupGenerator::do_generate(
        Ref<Class> p_target_class, Ref<GodotPropertyGroupAttribute> p_attribute,
        ClassGeneratorResult& r_result)
    {
        using namespace GodotGeneratorUtils;

        const Ref<Context> p_generated_body = r_result.generated_body;
        const Ref<Context> p_generated_sources = r_result.generated_sources;

        const Ref<Body> bind_methods_body =
            get_bind_methods_body(p_target_class, p_generated_body, p_generated_sources);
        GEN_ERROR_COND(!bind_methods_body, p_attribute, "Failed to get bind_methods body.");

        bind_methods_body->B<Function>()[{
            B<Identifier>(AssumedGodotTypes::ADD_GROUP().type->name()),
            B<Arguments>()[{
                B<Argument>()[Output::StringLiteral(p_attribute->literal_content())],
                B<Argument>()[Output::StringLiteral("")]}]}][Output::Semicolon()];

        return GeneratorError::OK;
    }

    Ref<GeneratorError> GodotSubgroupGenerator::do_generate_default_attribute_arguments(
        Ref<Class> p_target_class, Ref<GodotPropertySubgroupAttribute> p_attribute,
        Ref<Context> p_default_values)
    {
        UNUSED(p_target_class);
        UNUSED(p_attribute);
        p_default_values->B<StringLiteralArgument>()[B<Literal>("\"\"")];
        return GeneratorError::OK;
    }

    Ref<GeneratorError> GodotSubgroupGenerator::do_generate(
        Ref<Class> p_target_class, Ref<GodotPropertySubgroupAttribute> p_attribute,
        ClassGeneratorResult& r_result)
    {
        using namespace GodotGeneratorUtils;

        const Ref<Context> p_generated_body = r_result.generated_body;
        const Ref<Context> p_generated_sources = r_result.generated_sources;

        const Ref<Body> bind_methods_body =
            get_bind_methods_body(p_target_class, p_generated_body, p_generated_sources);
        GEN_ERROR_COND(!bind_methods_body, p_attribute, "Failed to get bind_methods body.");

        bind_methods_body->B<Function>()[{
            B<Identifier>(AssumedGodotTypes::ADD_SUBGROUP().type->name()),
            B<Arguments>()[{
                B<Argument>()[Output::StringLiteral(p_attribute->literal_content())],
                B<Argument>()[Output::StringLiteral("")]}]}][Output::Semicolon()];

        return GeneratorError::OK;
    }

} // namespace GodotObjectCompiler