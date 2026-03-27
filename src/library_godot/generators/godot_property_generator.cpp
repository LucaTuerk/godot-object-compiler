/**************************************************************************/
/* godot_property_generator.cpp                                           */
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

#include "library_godot/generators/godot_property_generator.h"

#include "godot_generator_utils.h"
#include "godot_signal_generator.h"
#include "library/tree/output/output.h"
#include "library/tree/syntax/class.h"
#include "library_godot/assumptions.h"
#include "library_godot/attributes/godot_attributes.h"
#include "library_godot/attributes/property_generator_options.h"
#include "library_godot/generated_assumptions/parameter_types.h"

namespace GodotObjectCompiler
{

    Ref<GeneratorError> GodotPropertyGenerator::check_for_property_type_errors(
        const Ref<Node>& p_target, const Ref<Type>& p_type, const Ref<Namespace>& p_from_namespace)
    {
        bool is_object_type = GodotGeneratorUtils::type_is_object_type(p_type, p_from_namespace);
        bool is_ptr = p_type->find_child<Pointer>() != nullptr;

        Ref<Type> inner;
        bool is_refcounted_type =
            GodotGeneratorUtils::type_is_ref_counted_type(p_type, p_from_namespace);
        bool is_ref_type =
            GodotGeneratorUtils::type_is_godot_ref_type(p_type, inner, p_from_namespace);
        bool is_inner_refcounted_type =
            is_ref_type ? GodotGeneratorUtils::type_is_ref_counted_type(inner, p_from_namespace)
                        : false;

        GEN_ERROR_COND(
            is_ref_type && !is_inner_refcounted_type, p_target,
            "Ref<%s> inner type is invalid, %s is not a RefCounted type.",
            inner->type_name().c_str(), inner->type_name().c_str());
        GEN_ERROR_COND(
            is_refcounted_type && !is_ref_type, p_target,
            "RefCounted property backing field needs to be wrapped in Ref<...>.");
        GEN_ERROR_COND(
            is_object_type && !is_ptr, p_target,
            "Object type property backing field needs to be a pointer type.");

        return GeneratorError::OK;
    }

    Ref<GeneratorError> GodotPropertyGenerator::check_for_field_property_errors(
        const Ref<Field>& p_field, const Ref<GodotPropertyAttribute>& p_attribute,
        const Ref<Namespace>& p_from_namespace)
    {
        GEN_ERROR_COND(!p_field, p_attribute, "Failed to get target field for property attribute");
        GEN_ERROR_COND(p_field->is_const(), p_field, "Property target field is constant.");
        GEN_ERROR_COND(p_field->is_static(), p_field, "Property target field is static.");

        Ref<Type> type = p_field->type();
        GEN_ERROR_COND(!type, p_field, "Failed to get type field type.");
        GEN_ERROR_PASS_ON(check_for_property_type_errors(p_field, type, p_from_namespace));

        Ref<Literal> literal = p_field->find_descendant<Literal>();
        bool is_object_type = GodotGeneratorUtils::type_is_object_type(type, p_from_namespace);

        Ref<Enum> enum_;
        Ref<Type> inner;
        bool is_ref_type =
            GodotGeneratorUtils::type_is_godot_ref_type(type, inner, p_from_namespace);
        bool is_enum_type = GodotGeneratorUtils::type_is_enum_type(type, enum_, p_from_namespace);
        bool is_primitive_type = GodotGeneratorUtils::type_is_primitive_type(type);

        GEN_ERROR_COND(
            is_object_type && !is_ref_type && literal == nullptr, p_field,
            "Object type property backing field needs to be initialized.");
        GEN_ERROR_COND(
            is_primitive_type && literal == nullptr, p_field,
            "Primitive type property backing field needs to be initialized.");
        GEN_ERROR_COND(
            is_enum_type && literal == nullptr, p_field,
            "Enum type property backing field needs to be initialized.");
        return GeneratorError::OK;
    }

    Ref<GeneratorError> GodotPropertyGenerator::check_for_custom_bound_property_errors(
        const GodotCustomPropertyBind& custom_bind, const Ref<GodotPropertyAttribute>& p_attribute,
        const Ref<Namespace>& p_from_namespace)
    {
        GEN_ERROR_COND(
            custom_bind.property_name.empty(), p_attribute,
            "Empty property name in custom bound property.");
        GEN_ERROR_COND(
            !custom_bind.getter, p_attribute,
            "Could not find getter function \"%s\" for custom bound property.",
            custom_bind.getter_name.c_str());
        GEN_ERROR_COND(
            !custom_bind.setter, p_attribute,
            "Could not find setter function \"%s\" for custom bound property.",
            custom_bind.setter_name.c_str());

        Ref<Type> getter_type = custom_bind.getter->type();
        GEN_ERROR_COND(!getter_type, custom_bind.getter, "Failed to get getter type.");

        Ref<Type> setter_type = custom_bind.setter->type();
        GEN_ERROR_COND(!setter_type, custom_bind.setter, "Failed to get setter type.");

        GEN_ERROR_COND(
            !custom_bind.getter->parameters() ||
                custom_bind.getter->parameters()->get_child_count() != 0,
            custom_bind.getter, "Invalid parameter count on property getter, expected to be 0");
        GEN_ERROR_COND(
            !custom_bind.setter->parameters() ||
                custom_bind.setter->parameters()->get_child_count() != 1,
            custom_bind.setter, "Invalid parameter count on property setter, expected to be 1");

        Ref<Type> setter_param_type = custom_bind.setter->find_chain<Type, Parameters, Parameter>();
        GEN_ERROR_COND(
            !setter_param_type, custom_bind.setter, "Failed to get setter parameter type.");
        GEN_ERROR_COND(
            setter_type->type_name() != "void", custom_bind.setter,
            "Invalid return type on property setter, expected to be void");
        GEN_ERROR_COND(
            setter_param_type->type_name_unmodified() != getter_type->type_name_unmodified(),
            p_attribute, "Invalid non matching types on getter / setter pair");
        GEN_ERROR_PASS_ON(
            check_for_property_type_errors(p_attribute, getter_type, p_from_namespace));
        return GeneratorError::OK;
    }

    Ref<GeneratorError> GodotPropertyGenerator::do_generate_default_attribute_arguments(
        Ref<Class> p_target_class, Ref<GodotPropertyAttribute> p_attribute,
        Ref<Context> p_default_values)
    {
        using namespace GodotGeneratorUtils;
        using namespace AssumedParameterValues;

        Ref<Type> property_type;

        Ref<PropertyGetAccessSpecifierArgument> property_get_access_specifier =
            node_new<PropertyGetAccessSpecifierArgument>();
        Ref<PropertySetAccessSpecifierArgument> property_set_access_specifier =
            node_new<PropertySetAccessSpecifierArgument>();

        if (Ref<Field> target_field = p_attribute->TargetField(); target_field) {
            property_type = target_field->type();

            property_get_access_specifier->B<Identifier>(
                PropertyGetAccessSpecifierArgument::PublicGet);
            if (target_field->is_private_member()) {
                property_set_access_specifier->B<Identifier>(
                    PropertySetAccessSpecifierArgument::PrivateSet);
            } else if (target_field->is_protected_member()) {
                property_set_access_specifier->B<Identifier>(
                    PropertySetAccessSpecifierArgument::ProtectedSet);
            } else {
                property_set_access_specifier->B<Identifier>(
                    PropertySetAccessSpecifierArgument::PublicSet);
            }
        } else if (Opt<GodotCustomPropertyBind> custom_bind = p_attribute->CustomBind();
                   custom_bind) {
            GEN_ERROR_COND(
                !custom_bind->getter, p_attribute,
                "Could not find getter function \"%s\" for custom bound property.",
                custom_bind->getter_name.c_str());
            property_type = custom_bind->getter->type();
            property_get_access_specifier->B<Identifier>(
                PropertyGetAccessSpecifierArgument::PrivateGet);
            property_set_access_specifier->B<Identifier>(
                PropertySetAccessSpecifierArgument::PrivateSet);
        } else {
            GEN_ERROR(
                p_attribute,
                "Failed to get target field or custom bind for GodotProperty attribute.");
        }
        GEN_ERROR_COND(!property_type, p_attribute, "Could not get property type.");
        property_type = property_type->qualified();

        const String property_type_name = property_type->type_name();
        GEN_ERROR_COND(
            property_type_name.empty(), p_attribute, "Invalid type name for target property.");

        Ref<GodotVariantTypeArgument> variant_type;
        Ref<GodotPropertyHintArgument> property_hint;
        Ref<GodotPropertyUsageFlagsArgument> property_usage_flags;

        if (!get_defaults_for_type(
                property_type, variant_type, property_hint, property_usage_flags, p_target_class)) {
            GEN_ERROR(
                p_attribute, "Unknown property type. Failed to determine default property info.");
        }

        Ref<StringLiteralArgument> string_literal_argument =
            B<StringLiteralArgument>()[make_ref<Literal>("")];

        Ref<GodotCustomPropertyGetSetArgument> custom_bind_argument =
            B<GodotCustomPropertyGetSetArgument>()[{
                B<Identifier>(GodotCustomPropertyGetSetParameterType::AutoGetSet), B<Arguments>()}];

        Ref<PropertyGeneratorOptionsArgument> property_generator_options_argument =
            B<PropertyGeneratorOptionsArgument>()[{
                B<Identifier>(PropertyGeneratorOptionsArgument::Default), B<Arguments>()}];

        p_default_values->add_children(
            {custom_bind_argument, string_literal_argument, custom_bind_argument, variant_type,
             property_hint, property_usage_flags, property_get_access_specifier,
             property_set_access_specifier, property_generator_options_argument});

        return GeneratorError::OK;
    }

    Ref<GeneratorError> GodotPropertyGenerator::do_generate(
        Ref<Class> p_target_class, Ref<GodotPropertyAttribute> p_attribute,
        ClassGeneratorResult& r_result)
    {
        Ref<Context> p_generated_body = r_result.generated_body;
        Ref<Context> p_generated_sources = r_result.generated_sources;
        Ref<Context> p_generated_global = r_result.generated_global;
        UNUSED(p_generated_global);

        using namespace GodotGeneratorUtils;

        Ref<Body> bind_methods_body =
            get_bind_methods_body(p_target_class, p_generated_body, p_generated_sources);
        GEN_ERROR_COND(
            !bind_methods_body, p_target_class,
            "Failed to find or generate the _bind_methods function body.");

        Ref<Body> get_property_list_body =
            get_get_property_list_body(p_target_class, p_generated_body, p_generated_sources);
        GEN_ERROR_COND(
            !get_property_list_body, p_target_class,
            "Failed to find or generate the _get_property_list function body.");

        Ref<Context> generated_public_members, generated_protected_members,
            generated_private_members;
        GEN_ERROR_COND(
            unzip_generated_body(
                p_generated_body, generated_public_members, generated_protected_members,
                generated_private_members) != GeneratorError::OK,
            p_target_class, "Failed to find generated body groups");

        Ref<Type> property_type;
        String property_name;
        String backing_field_name;
        bool generate_getters_and_setters = false;
        bool bind_getter = true;
        bool bind_setter = true;
        String getter_name;
        String setter_name;

        if (const Ref<Field> target_field = p_attribute->TargetField(); target_field) {
            GEN_ERROR_PASS_ON(
                check_for_field_property_errors(target_field, p_attribute, p_target_class));
            property_name = target_field->name();
            backing_field_name = target_field->name();

            Ref<Literal> name_literal =
                p_attribute->arguments()->find_chain<Literal, StringLiteralArgument>();
            if (String unwrapped; name_literal && name_literal->unwrap_string_literal(unwrapped)) {
                property_name = unwrapped;
            }

            property_type = target_field->type();
            generate_getters_and_setters = true;
            getter_name = format("get_%s", property_name.c_str());
            setter_name = format("set_%s", property_name.c_str());
        } else if (const Opt<GodotCustomPropertyBind> custom_bind = p_attribute->CustomBind();
                   custom_bind) {
            GEN_ERROR_PASS_ON(
                check_for_custom_bound_property_errors(*custom_bind, p_attribute, p_target_class));
            property_name = custom_bind->property_name;
            property_type = custom_bind->getter->type();
            getter_name = custom_bind->getter->name();
            setter_name = custom_bind->setter->name();
            bind_getter =
                custom_bind->getter->get_previous_sibling<GodotFunctionAttribute>() == nullptr;
            bind_setter =
                custom_bind->setter->get_previous_sibling<GodotFunctionAttribute>() == nullptr;
        }
        property_type = property_type->qualified();

        GEN_ERROR_COND(property_name.empty(), p_attribute, "Could not get property name.");
        GEN_ERROR_COND(!property_type, p_attribute, "Could not get property type.");

        const String type_name = property_type->type_name();
        GEN_ERROR_COND(type_name.empty(), p_attribute, "Invalid empty type name.");

        Ref<Type> ref_inner;
        Ref<Enum> enum_object;
        bool is_obj_type = type_is_object_type(property_type, p_target_class);
        bool is_enum_type = type_is_enum_type(property_type, enum_object, p_target_class);

        if (bind_getter) {
            bind_methods_body->add_child(
                bind_method(p_target_class->name(), getter_name, getter_name, {}));
        }
        if (bind_setter) {
            bind_methods_body->add_child(
                bind_method(p_target_class->name(), setter_name, setter_name, {property_name}));
        }

        if (generate_getters_and_setters) {
            Ref<Node> get_type = property_type->clone();
            Ref<Node> set_type = !is_obj_type ? const_ref(type_name) : property_type->clone();
            if (is_enum_type) {
                get_type = B<Type>()[B<Identifier>("int")];
                set_type = B<Type>()[B<Identifier>("int")];
            }

            Ref<PropertyGeneratorOptionsArgument> generator_options =
                p_attribute->arguments()->find_child<PropertyGeneratorOptionsArgument>();
            GEN_ERROR_COND(
                generator_options == nullptr, p_attribute, "Failed to get generator options.");

            Ref<Identifier> generator_options_identifier =
                generator_options->find_child<Identifier>();
            GEN_ERROR_COND(
                generator_options_identifier == nullptr, p_attribute,
                "Malformed generator options argument.");

            Ref<Literal> generator_options_hint =
                generator_options->find_chain<Literal, Arguments, Argument>();
            String generator_hint_string = "";
            if (generator_options_hint) {
                GEN_ERROR_COND(
                    !generator_options_hint->unwrap_string_literal(generator_hint_string),
                    p_attribute, "Failed to get property hint literal content.");
            }

            Ref<Function> get_def = B<Function>()[{
                get_type->clone(), B<Identifier>(getter_name), B<Parameters>(), B<Const>()}]
                                                 [Output::Semicolon()];

            Ref<Function> get_impl = B<Function>()[{
                get_type->clone(),
                B<Identifier>(p_target_class->qualified_name() + "::" + getter_name),
                B<Parameters>(), B<Const>(), B<Body>()[Output::Return(backing_field_name)]}];

            Ref<Function> set_def = B<Function>().with_children(
                {B<Type>()[B<Identifier>("void")], B<Identifier>(setter_name),
                 B<Parameters>()[B<Parameter>()[{
                     set_type->clone(),
                     B<Identifier>(format("p_%s", property_name.c_str())),
                 }]]})[Output::Semicolon()];

            Ref<Body> set_body = node_new<Body>();
            Ref<Node> assign =
                is_enum_type
                    ? Output::Assign(
                          backing_field_name,
                          Output::Text(format(
                              "static_cast<%s>(p_%s)", enum_object->qualified_name().c_str(),
                              property_name.c_str())))
                    : Output::Assign(
                          backing_field_name, Output::Text(format("p_%s", property_name.c_str())));

            if (generator_options_identifier->name != PropertyGeneratorOptionsArgument::Default) {
                String signal_name = generator_hint_string;
                if (signal_name.empty()) {
                    signal_name = format("%s_changed", property_name.c_str());

                    Ref<Parameters> signal_parameters = node_new<Parameters>();
                    if (generator_options_identifier->name ==
                        PropertyGeneratorOptionsArgument::EmitChangedValue) {
                        signal_parameters->B<Parameter>()[{
                            property_type->clone(), node_new<Identifier>(property_name)}];
                    }

                    generated_private_members->B<Function>()[{
                        B<Type>()[B<Identifier>("void")], B<Identifier>(signal_name),
                        signal_parameters->clone()}][Output::Semicolon()];

                    GodotSignalGenerator::bind_signal(
                        p_target_class, p_attribute, signal_name, signal_parameters, r_result);
                }

                Ref<Arguments> emit_arguments = node_new<Arguments>();
                if (generator_options_identifier->name ==
                    PropertyGeneratorOptionsArgument::EmitChangedValue) {
                    emit_arguments->B<Argument>()[B<Literal>(property_name)];
                }

                set_body->add_children(
                    {Output::If(
                         Output::FmtText(
                             "%s == %s", property_name.c_str(),
                             format("p_%s", property_name.c_str()).c_str()),
                         {Output::Return("")}),
                     assign, emit_signal(signal_name, emit_arguments)});
            } else {
                set_body->add_child(assign);
            }

            Ref<Function> set_impl = B<Function>()[{
                B<Type>()[B<Identifier>("void")],
                B<Identifier>(p_target_class->qualified_name() + "::" + setter_name),
                B<Parameters>()[B<Parameter>()[{
                    set_type->clone(), B<Identifier>("p_" + property_name)}]],
                set_body}];

            Ref<PropertyGetAccessSpecifierArgument> get_access_specifier_argument =
                p_attribute->arguments()->find_child<PropertyGetAccessSpecifierArgument>();

            GEN_ERROR_COND(
                !get_access_specifier_argument, p_attribute,
                "Failed to get get specifier argument");

            Ref<PropertySetAccessSpecifierArgument> set_access_specifier_argument =
                p_attribute->arguments()->find_child<PropertySetAccessSpecifierArgument>();

            GEN_ERROR_COND(
                !set_access_specifier_argument, p_attribute,
                "Failed to get set specifier argument");

            AccessSpecifier::Type get_specifier, set_specifier;
            GEN_ERROR_COND(
                !get_access_specifier_argument->get_specifier(get_specifier), p_attribute,
                "Failed to get property getter access specifier.");
            GEN_ERROR_COND(
                !set_access_specifier_argument->get_specifier(set_specifier), p_attribute,
                "Failed to get property setter access specifier.");

            switch (get_specifier) {
            case AccessSpecifier::PUBLIC: {
                generated_public_members->add_child(get_def);
            } break;
            case AccessSpecifier::PRIVATE:
                generated_private_members->add_child(get_def);
                break;
            case AccessSpecifier::PROTECTED:
                generated_protected_members->add_child(get_def);
                break;
            }

            switch (set_specifier) {
            case AccessSpecifier::PUBLIC: {
                generated_public_members->add_child(set_def);
            } break;
            case AccessSpecifier::PRIVATE:
                generated_private_members->add_child(set_def);
                break;
            case AccessSpecifier::PROTECTED:
                generated_protected_members->add_child(set_def);
                break;
            }

            p_generated_sources->add_children({
                get_impl,
                set_impl,
            });
        }

        Ref<GodotVariantTypeArgument> variant_type =
            p_attribute->arguments()->find_child<GodotVariantTypeArgument>();
        GEN_ERROR_COND(!variant_type, p_attribute, "Failed to get variant type argument");

        Ref<GodotPropertyHintArgument> property_hint =
            p_attribute->arguments()->find_child<GodotPropertyHintArgument>();
        GEN_ERROR_COND(!property_hint, p_attribute, "Failed to get property hint argument");

        Vector<Ref<GodotPropertyUsageFlagsArgument>> usage_flags =
            p_attribute->arguments()->find_children<GodotPropertyUsageFlagsArgument>();

        Ref<Node> property_info =
            build_property_info(variant_type, property_hint, usage_flags, property_name, r_result);
        Ref<Node> property_info_no_editor = build_property_info(
            variant_type, property_hint, usage_flags, property_name, r_result, true);

        Ref<Function> add_property = B<Function>()[{
            B<Identifier>(AssumedGodotTypes::ADD_PROPERTY().type->name()),
            B<Arguments>()[{
                B<Argument>()[property_info_no_editor],
                B<Argument>()[Output::StringLiteral(setter_name)],
                B<Argument>()[Output::StringLiteral(getter_name)],
            }]}][Output::Semicolon()];

        get_property_list_body->B<Function>()[{
            B<Identifier>("p_list->push_back"), B<Arguments>()[B<Argument>()[property_info]]}]
                                             [Output::Semicolon()];

        bind_methods_body->add_child(add_property);

        Ref<Body> property_names_body = get_property_names_body(p_target_class, p_generated_body);
        GEN_ERROR_COND(!property_names_body, p_attribute, "Failed to get property names body.");

        property_names_body->add_child(Output::Text(format(
            "static const %s& %s() {static const %s sn = \"%s\"; return sn; }",
            AssumedGodotTypes::StringName().type->qualified_name().c_str(), property_name.c_str(),
            AssumedGodotTypes::StringName().type->qualified_name().c_str(),
            property_name.c_str())));

        r_result.header_includes.insert(AssumedGodotTypes::StringName().type->header);
        r_result.source_includes.insert(AssumedGodotTypes::ADD_PROPERTY().type->header);
        return GeneratorError::OK;
    }

} // namespace GodotObjectCompiler