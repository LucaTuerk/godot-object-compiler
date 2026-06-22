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

#include "godot_generator_utils.h"
#include "library/tree/output/output.h"
#include "library/tree/predicates.h"
#include "library_godot/assumptions.h"

namespace GodotObjectCompiler
{

    Ref<GeneratorError> GodotClassGenerator::generate_initialization(
        Ref<Class> p_target_class, Ref<GodotClassAttribute> p_class_attribute,
        Ref<Context> p_initialize_content, Ref<Context> p_uninitialize_content)
    {
        UNUSED(p_uninitialize_content);
        using namespace GodotGeneratorUtils;

        const Ref<Identifier> init_level_identifier =
            p_class_attribute
                ->find_chain<Identifier, Arguments, GodotModuleInitializationLevelArgument>();
        GEN_ERR_COND(
            !init_level_identifier, p_target_class,
            "Could not determine the initialization level for class %s",
            p_target_class->name().c_str());

        const String init_level_name = init_level_identifier->name;
        const Ref<GodotModuleInitializationLevelParameterType> init_level_type =
            GodotModuleInitializationLevelParameterType::instance();

        String godot_init_level;
        if (!init_level_type->get_godot_init_level_for_value_name(
                init_level_name, godot_init_level)) {
            GEN_ERR(
                p_target_class, "Could not determine the initialization level for class %s",
                p_target_class->name().c_str())
        }

        const Ref<Identifier> class_type_identifier =
            p_class_attribute->find_chain<Identifier, Arguments, GodotClassTypeArgument>();
        GEN_ERR_COND(
            !class_type_identifier, p_target_class,
            "Could not determine the class type for class %s", p_target_class->name().c_str());

        const String class_type_name = class_type_identifier->name;
        const Ref<GodotClassTypeParameterType> class_type_type =
            GodotClassTypeParameterType::instance();

        if (String godot_registration_macro;
            class_type_type->get_macro_for_value_name(class_type_name, godot_registration_macro)) {
            Ref<Body> if_body;
            const Ref<Context> if_clause = Output::Spaces(
                {Output::Text(format("if (p_level == %s) ", godot_init_level.c_str())),
                 R<Body>(&if_body)});
            if_clause->set_tag(p_target_class->first_base_qualifed_name());

            if_body->B<Function>()[{
                B<Identifier>(godot_registration_macro),
                B<Arguments>()[B<Argument>()[Output::Text(p_target_class->qualified_name())]],
            }][Output::Semicolon()];

            Ref<Node> first_inheritor = p_initialize_content->find_child(
                0, NodePredicates::tag<Node>(p_target_class->qualified_name().c_str()));

            if (first_inheritor != nullptr) {
                p_initialize_content->add_child_before(if_clause, first_inheritor);
            } else {
                p_initialize_content->add_child(if_clause);
            }
        }
        return GeneratorError::OK;
    }

    Ref<GeneratorError> GodotClassGenerator::do_generate_default_attribute_arguments(
        Ref<Class> p_target_class, Ref<GodotClassAttribute> p_attribute,
        Ref<Context> p_default_values)
    {
        UNUSED(p_target_class);
        UNUSED(p_attribute);
        p_default_values->add_children(
            {B<GodotModuleInitializationLevelArgument>()[B<Identifier>(
                 AssumedParameterValues::LevelScene())][B<Arguments>()],
             B<GodotClassTypeArgument>()[B<Identifier>(AssumedParameterValues::GodotClass())]
                                        [B<Arguments>()]});

        return GeneratorError::OK;
    }

    Ref<GeneratorError> GodotClassGenerator::do_generate(
        Ref<Class> p_target_class, Ref<GodotClassAttribute> p_attribute,
        ClassGeneratorResult& r_result)
    {
        Ref<Context> p_generated_body = r_result.generated_body;
        Ref<Context> p_generated_sources = r_result.generated_sources;
        Ref<Context> p_generated_global = r_result.generated_global;
        UNUSED(p_generated_global);
        UNUSED(p_attribute);

        using namespace GodotGeneratorUtils;

        GEN_ERR_COND(
            p_attribute->resolve_target() != p_target_class, p_target_class,
            "Resolved class is not the provided target class.");

        Vector<String> bases = p_target_class->direct_bases_names();
        GEN_ERR_COND(
            bases.empty(), p_target_class,
            "No base classes. Target class %s must inherit from a godot object class",
            p_target_class->name().c_str());

        if (!class_is_godot_object_type(p_target_class)) {
            GEN_ERR(
                p_target_class, "Target class %s must inherit from a godot object class",
                p_target_class->name().c_str());
        }

        Ref<Include> last_include = p_target_class->find_ancestor<Include>(BY_SIBLINGS_PREV);
        GEN_ERR_COND(
            !last_include || last_include->include_path != r_result.generated_header_include_path,
            last_include ? last_include->as<Node>() : p_attribute->as<Node>(),
            "Generated header \"%s\" must be the last include.",
            r_result.generated_header_include_path.c_str());

        Ref<Function> gd_class = B<Function>()[{
            B<Identifier>(AssumedGodotTypes::GDCLASS().type->name()),
            B<Arguments>()[{
                B<Argument>()[Output::Text(p_target_class->name())],
                B<Argument>()[Output::Text(bases[0])],
            }]}][Output::Semicolon()];

        p_generated_body->add_children({
            gd_class,
            B<AccessSpecifier>(AccessSpecifier::PUBLIC),
            B<Context>().$("public_members"),
            B<AccessSpecifier>(AccessSpecifier::PROTECTED),
            B<Context>().$("protected_members"),
            B<AccessSpecifier>(AccessSpecifier::PRIVATE),
            B<Context>().$("private_members"),
        });

        get_bind_methods_body(p_target_class, p_generated_body, p_generated_sources);
        get_notification_body(p_target_class, p_generated_body, p_generated_sources);
        get_property_names_body(p_target_class, p_generated_body);
        get_function_names_body(p_target_class, p_generated_body);
        get_signal_names_body(p_target_class, p_generated_body);

        return GeneratorError::OK;
    }

} // namespace GodotObjectCompiler
