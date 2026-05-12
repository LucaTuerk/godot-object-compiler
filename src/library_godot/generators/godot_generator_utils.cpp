/**************************************************************************/
/* godot_generator_utils.cpp                                              */
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

#include "library_godot/generators/godot_generator_utils.h"

#include "library/core/file_system_utilities.h"
#include "library/core/resources.h"
#include "library/generator/generator.h"
#include "library/tree/output/output.h"
#include "library/tree/predicates.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/context.h"
#include "library/tree/syntax/function.h"
#include "library/tree/syntax/modifiers.h"
#include "library/tree/syntax/type.h"
#include "library/type_db.h"
#include "library_godot/assumptions.h"
#include "library_godot/attributes/godot_attributes.h"
#include "library_godot/attributes/godot_property_hint.h"
#include "library_godot/attributes/godot_variant_type.h"

namespace GodotObjectCompiler
{

    static bool _variant_types_initialized = false;
    static Dictionary<String, String> _type_to_variant_type;
    static Dictionary<String, Vector<String>> _variant_type_to_types;

    void ensure_type_dicts_initialized()
    {
        if (_variant_types_initialized) {
            return;
        }
        _variant_types_initialized = true;

        const Ref<GodotVariantTypeParameterType> ptype = GodotVariantTypeParameterType::instance();

        for (const String& value_name : ptype->value_names()) {
            String res_path = "res://" + path_concat_ext("variant_types", value_name, "txt");
            if (Resources::instance()->has_resource(res_path)) {
                String content = Resources::instance()->load_text_resource(res_path);
                for (const String& line : string_split(content, "\n")) {
                    String trimmed = string_trim(line);
                    if (trimmed.empty()) {
                        continue;
                    }

                    _type_to_variant_type[trimmed] = value_name;
                    _variant_type_to_types[value_name].push_back(trimmed);
                }
            }
        }
    }

    String GodotGeneratorUtils::get_type_static()
    {
        return "GodotGeneratorUtils";
    }

    String GodotGeneratorUtils::type_name_remove_usings(String p_typename)
    {
        for (const String& _using : LibraryContext::instance()->get_usings()) {
            if (string_prefix(p_typename, format("%s::", _using.c_str()))) {
                return p_typename.substr(_using.size() + 2);
            }
        }
        return p_typename;
    }
    bool GodotGeneratorUtils::get_type_header(
        Ref<Type> p_type, Ref<Namespace> p_from_namespace, String& r_header)
    {
        Result<NamedContext> type_result =
            LibraryContext::instance()->get_type_db()->get_type_data<NamedContext>(
                p_type, p_from_namespace);
        if (type_result.has_result()) {
            r_header = type_result.get_result()->header;
            return !r_header.empty();
        }

        type_result.get_error()->set_handled();
        r_header = "";
        return false;
    }

    Ref<Type> GodotGeneratorUtils::const_ref(const String& p_type_name)
    {
        return B<Type>()[{B<Const>(), B<Identifier>(p_type_name), B<Reference>()}];
    }

    Result<Function> GodotGeneratorUtils::add_signal(
        const Ref<Class>& p_target_class, const String& p_signal_name,
        const Ref<Parameters>& p_parameters, ClassGeneratorResult& r_result)
    {
        Ref<Arguments> arguments;
        Ref<Function> add_signal = B<Function>()[{
            B<Identifier>("ADD_SIGNAL"),
            B<Arguments>()[B<Argument>()[{B<Function>()[{
                B<Identifier>("MethodInfo"),
                R<Arguments>(
                    &arguments)[{B<Argument>()[Output::StringLiteral(p_signal_name)]}]}]}]]}]
                                                [Output::Semicolon()];

        Size i = 1;
        for (const Ref<Parameter>& parameter : p_parameters->find_children<Parameter>()) {
            Ref<Type> type = parameter->find_child<Type>();
            ERROR_COND(type == nullptr, "Failed to find type for signal parameter.");

            type = type->qualified();
            Ref<Identifier> identifier = parameter->find_child<Identifier>();
            String name = identifier ? identifier->name : format("p_param_%d", i);

            Result<Node> property_info_result = build_property_info_defaults(
                type, name, r_result, p_target_class, DEFAULTS_SIGNAL_ARGUMENT);
            RESULT_ERROR_PASS_ON(Error, property_info_result, property_info);

            arguments->B<Argument>()[property_info];
            i += 1;
        }
        return add_signal;
    }

    Ref<Function>
    GodotGeneratorUtils::emit_signal(const String& p_signal_name, const Ref<Arguments>& p_arguments)
    {
        Ref<Arguments> arguments;
        Ref<Function> call = B<Function>()[{
            B<Identifier>("emit_signal"),
            R<Arguments>(&arguments)[{B<Argument>()[Output::StringLiteral(p_signal_name)]}]}]
                                          [Output::Semicolon()];

        for (const Ref<Argument>& argument : p_arguments->find_children<Argument>()) {
            arguments->add_child(argument->clone());
        }
        return call;
    }

    Ref<Function> GodotGeneratorUtils::bind_method(
        const String& p_class_name, const String& p_bind_name, const String& p_method_name,
        const Vector<String>& p_parameter_names, const Vector<String>& p_default_values)
    {
        return bind_method_as(
            p_class_name, p_bind_name, p_method_name, p_parameter_names, p_default_values);
    }

    Ref<Function> GodotGeneratorUtils::bind_method_as(
        const String& p_class_name, const String& p_bind_name, const String& p_method_name,
        const Vector<String>& p_parameter_names, const Vector<String>& p_default_values)
    {
        Ref<Arguments> bind_args;
        Ref<Arguments> d_method_args;

        Ref<Function> result = B<Function>()[{
            B<Identifier>(format(
                "%s::bind_method", AssumedGodotTypes::ClassDB().type->qualified_name().c_str())),
            R<Arguments>(&bind_args)[{
                B<Function>()[{
                    B<Identifier>("D_METHOD"),
                    R<Arguments>(
                        &d_method_args)[{B<Argument>()[Output::StringLiteral(p_bind_name)]}]}],
                B<Argument>()[{
                    B<Reference>(),
                    B<Identifier>(p_class_name),
                    Output::Text("::"),
                    Output::Text(p_method_name),
                }]}]}][Output::Semicolon()];

        for (const String& parameter_name : p_parameter_names) {
            d_method_args->B<Argument>()[Output::StringLiteral(parameter_name)];
        }

        for (const String& def_val : p_default_values) {
            bind_args->B<Argument>()[Output::Text(format("DEFVAL(%s)", def_val.c_str()))];
        }

        return result;
    }

    Ref<Function> GodotGeneratorUtils::bind_static_method(
        const String& p_class_name, const String& p_bind_name, const String& p_method_name,
        const Vector<String>& p_parameter_names, const Vector<String>& p_default_values)
    {
        Ref<Arguments> bind_args;
        Ref<Arguments> d_method_args;

        Ref<Function> result = B<Function>()[{
            B<Identifier>(format(
                "%s::bind_static_method",
                AssumedGodotTypes::ClassDB().type->qualified_name().c_str())),
            R<Arguments>(&bind_args)[{
                B<Argument>()[Output::StringLiteral(p_class_name)],
                B<Function>()[{
                    B<Identifier>("D_METHOD"),
                    R<Arguments>(
                        &d_method_args)[{B<Argument>()[Output::StringLiteral(p_bind_name)]}]}],
                B<Argument>()[{
                    B<Reference>(),
                    B<Identifier>(p_class_name),
                    Output::Text("::"),
                    Output::Text(p_method_name),
                }]}]}][Output::Semicolon()];

        for (const String& parameter_name : p_parameter_names) {
            d_method_args->B<Argument>()[Output::StringLiteral(parameter_name)];
        }

        for (const String& def_val : p_default_values) {
            bind_args->B<Argument>()[Output::Text(format("DEFVAL(%s)", def_val.c_str()))];
        }

        return result;
    }

    Ref<Body> GodotGeneratorUtils::get_bind_methods_body(
        const Ref<Class>& p_target_class, const Ref<Context>& p_generated_body,
        const Ref<Context>& p_generated_sources)
    {
        Ref<Context> public_members, _protected_members, _private_members;
        unzip_generated_body(
            p_generated_body, public_members, _protected_members, _private_members);
        PANIC_COND(!public_members, "Failed to get public members group.");

        bool bind_methods_defined = p_target_class->has_function_named("_bind_methods");
        const String bind_methods_name =
            bind_methods_defined ? "_generated_bind_methods" : "_bind_methods";
        const String qualified_bind_methods_name =
            format("%s::%s", p_target_class->qualified_name().c_str(), bind_methods_name.c_str());

        Ref<Function> bind_methods = p_generated_sources->find_child(
            0, NamedContextPredicates::name<Function>(qualified_bind_methods_name.c_str()));
        Ref<Body> bind_methods_body;

        if (!bind_methods) {
            p_generated_sources->B<Function>()[{
                B<Type>()[B<Identifier>("void")], B<Identifier>(qualified_bind_methods_name),
                B<Parameters>(), R<Body>(&bind_methods_body)}];

            public_members->B<Function>()[{
                B<Static>(), B<Type>()[B<Identifier>("void")], B<Identifier>(bind_methods_name),
                B<Parameters>()}][Output::Semicolon()];
        } else {
            bind_methods_body = bind_methods->find_child<Body>();
            PANIC_COND(!bind_methods_body, "Body not found.");
        }
        return bind_methods_body;
    }

    Ref<Body> GodotGeneratorUtils::get_notification_body(
        const Ref<Class>& p_target_class, const Ref<Context>& p_generated_body,
        const Ref<Context>& p_generated_sources)
    {
        Ref<Context> public_members, _protected_members, _private_members;
        unzip_generated_body(
            p_generated_body, public_members, _protected_members, _private_members);
        PANIC_COND(!public_members, "Failed to get public members group.");

        bool notification_defined = p_target_class->has_function_named("_notification");
        String notification_name =
            notification_defined ? "_generated_notification" : "_notification";
        String qualified_notification_name =
            format("%s::%s", p_target_class->qualified_name().c_str(), notification_name.c_str());

        Ref<Function> notification = p_generated_sources->find_child(
            0, NamedContextPredicates::name<Function>(qualified_notification_name.c_str()));
        Ref<Body> notification_body;

        if (!notification) {
            p_generated_sources->B<Function>()[{
                B<Type>()[B<Identifier>("void")], B<Identifier>(qualified_notification_name),
                B<Parameters>()[B<Parameter>()[{
                    B<Type>()[B<Identifier>("int")],
                    B<Identifier>("p_notification"),
                }]],
                R<Body>(&notification_body)}];

            public_members->B<Function>()[{
                B<Type>()[B<Identifier>("void")],
                B<Identifier>(notification_name),
                B<Parameters>()[B<Parameter>()[{
                    B<Type>()[B<Identifier>("int")],
                    B<Identifier>("p_notification"),
                }]],
            }][Output::Semicolon()];
        } else {
            notification_body = notification->find_child<Body>();
            PANIC_COND(!notification_body, "Body not found.");
        }

        return notification_body;
    }

    Ref<Body> GodotGeneratorUtils::get_function_names_body(
        const Ref<Class>& p_target_class, const Ref<Context>& p_generated_body)
    {
        Ref<Context> public_members, _protected_members, _private_members;
        unzip_generated_body(
            p_generated_body, public_members, _protected_members, _private_members);
        PANIC_COND(!public_members, "Failed to get public members group.");

        Ref<Struct> function_names =
            public_members->find_child(0, NamedContextPredicates::name<Struct>("FunctionNames"));
        Ref<Body> function_names_body;

        if (!function_names) {
            auto base_names = p_target_class->direct_bases_names();
            auto type_result = LibraryContext::instance()->get_type_db()->get_type_attribute(
                base_names[0], GodotClassAttribute::get_type_static(), 0, p_target_class);
            if (base_names.size() == 1 && type_result.has_result()) {
                public_members->B<Struct>()[{
                    B<Identifier>("FunctionNames"),
                    B<BaseClasses>()[B<Type>()[B<Identifier>(base_names[0] + "::FunctionNames")]],
                    R<Body>(&function_names_body)}];
            } else {
                if (type_result.has_error()) {
                    type_result.get_error()->set_handled();
                }
                public_members
                    ->B<Struct>()[{B<Identifier>("FunctionNames"), R<Body>(&function_names_body)}];
            }
        } else {
            function_names_body = function_names->find_child<Body>();
            PANIC_COND(!function_names_body, "Body not found.");
        }

        return function_names_body;
    }

    Ref<Body> GodotGeneratorUtils::get_property_names_body(
        const Ref<Class>& p_target_class, const Ref<Context>& p_generated_body)
    {
        Ref<Context> public_members, _protected_members, _private_members;
        unzip_generated_body(
            p_generated_body, public_members, _protected_members, _private_members);
        PANIC_COND(!public_members, "Failed to get public members group.");

        const Ref<Struct> property_names =
            public_members->find_child(0, NamedContextPredicates::name<Struct>("PropertyNames"));
        Ref<Body> property_names_body;
        if (!property_names) {
            auto base_names = p_target_class->direct_bases_names();
            auto type_result = LibraryContext::instance()->get_type_db()->get_type_attribute(
                base_names[0], GodotClassAttribute::get_type_static(), 0, p_target_class);
            if (base_names.size() == 1 && type_result.has_result()) {
                public_members->B<Struct>()[{
                    B<Identifier>("PropertyNames"),
                    B<BaseClasses>()[B<Type>()[B<Identifier>(base_names[0] + "::PropertyNames")]],
                    R<Body>(&property_names_body)}];
            } else {
                if (type_result.has_error()) {
                    type_result.get_error()->set_handled();
                }
                public_members
                    ->B<Struct>()[{B<Identifier>("PropertyNames"), R<Body>(&property_names_body)}];
            }
        } else {
            property_names_body = property_names->find_child<Body>();
            PANIC_COND(!property_names_body, "Body not found.");
        }

        return property_names_body;
    }

    Ref<Body> GodotGeneratorUtils::get_signal_names_body(
        const Ref<Class>& p_target_class, const Ref<Context>& p_generated_body)
    {
        Ref<Context> public_members, _protected_members, _private_members;
        unzip_generated_body(
            p_generated_body, public_members, _protected_members, _private_members);
        PANIC_COND(!public_members, "Failed to get public members group.");

        Ref<Struct> signal_names =
            public_members->find_child(0, NamedContextPredicates::name<Struct>("SignalNames"));
        Ref<Body> signal_names_body;
        if (!signal_names) {
            const auto base_names = p_target_class->direct_bases_names();
            const auto type_result = LibraryContext::instance()->get_type_db()->get_type_attribute(
                base_names[0], GodotClassAttribute::get_type_static(), 0, p_target_class);
            if (base_names.size() == 1 && type_result.has_result()) {
                public_members->B<Struct>()[{
                    B<Identifier>("SignalNames"),
                    B<BaseClasses>()[B<Type>()[B<Identifier>(base_names[0] + "::SignalNames")]],
                    R<Body>(&signal_names_body)}];
            } else {
                if (type_result.has_error()) {
                    type_result.get_error()->set_handled();
                }

                public_members
                    ->B<Struct>()[{B<Identifier>("SignalNames"), R<Body>(&signal_names_body)}];
            }
        } else {
            signal_names_body = signal_names->find_child<Body>();
            PANIC_COND(!signal_names_body, "Body not found.");
        }

        return signal_names_body;
    }

    Ref<Context> GodotGeneratorUtils::get_include_section(const Ref<Context>& p_target)
    {
        Ref<Context> include_section =
            p_target->find_descendant(BFS, NodePredicates::tag<Context>("includes"));
        if (!include_section) {
            include_section = Output::Lines({});
            p_target->add_child(include_section);
        }
        return include_section;
    }

    Ref<Body>
    GodotGeneratorUtils::get_if_body(const Ref<Context>& p_target, const String& condition)
    {
        Ref<Body> if_body =
            p_target->find_descendant(BFS, NodePredicates::tag<Body>(condition.c_str()));
        if (!if_body) {
            p_target->add_child(Output::Spaces(
                {Output::FmtText("if (%s)", condition.c_str()), R<Body>(&if_body).$(condition)}));
        }
        return if_body;
    }

    Ref<GeneratorError> GodotGeneratorUtils::unzip_generated_body(
        const Ref<Context>& p_generated_body, Ref<Context>& r_public_members,
        Ref<Context>& r_protected_members, Ref<Context>& r_private_members)
    {
        r_public_members =
            p_generated_body->find_child(0, NodePredicates::tag<Context>("public_members"));
        GEN_ERROR_COND(
            r_public_members == nullptr, p_generated_body, "Failed to get public members group");
        r_protected_members =
            p_generated_body->find_child(0, NodePredicates::tag<Context>("protected_members"));
        GEN_ERROR_COND(
            r_protected_members == nullptr, p_generated_body,
            "Failed to get protected members group");
        r_private_members =
            p_generated_body->find_child(0, NodePredicates::tag<Context>("private_members"));
        GEN_ERROR_COND(
            r_private_members == nullptr, p_generated_body, "Failed to get private members group");

        return GeneratorError::OK;
    }

    bool GodotGeneratorUtils::check_is_valid_named_argument(const Ref<Node>& p_node, String& p_name)
    {
        if (p_node == nullptr) {
            return false;
        }

        p_name = "";
        return !tree_has_error(p_node) && get_name(p_node, p_name);
    }

    bool GodotGeneratorUtils::get_name(const Ref<Node>& p_node, String& p_name)
    {
        p_name = "";
        if (p_node == nullptr) {
            return false;
        }

        if (const Ref<Identifier> id = p_node->as<Identifier>()) {
            p_name = id->name;
            return true;
        }

        if (const Ref<Context> context = p_node->as<Context>()) {
            for (const Ref<Node>& child : context->get_children()) {
                if (const Ref<Identifier> id = child->as<Identifier>()) {
                    p_name = id->name;
                    return true;
                }
            }
        }

        return false;
    }

    bool GodotGeneratorUtils::tree_has_error(const Ref<Node>& p_tree)
    {
        if (p_tree == nullptr) {
            return false;
        }
        if (p_tree->is<Error>()) {
            return true;
        }

        if (const Ref<Context> context = p_tree->as<Context>()) {
            for (const Ref<Node>& child : *context) {
                if (tree_has_error(child)) {
                    return true;
                }
            }
        }

        return false;
    }

    bool GodotGeneratorUtils::class_has_base_class(
        const Ref<Class>& p_target_class, const String& p_base_class_qualified)
    {
        if (p_target_class->qualified_name() == p_base_class_qualified ||
            p_target_class->qualified_name() == "godot::" + p_base_class_qualified) {
            return true;
        }

        auto base_class_names = p_target_class->direct_bases_names();
        if (base_class_names.empty()) {
            return false;
        }

        const String& base = base_class_names.front();

        // TODO: this will not work if the base class name is not fully qualified
        Result<Class> base_result =
            LibraryContext::instance()->get_type_db()->get_type_data<Class>(base);
        if (base_result.has_error()) {
            fmt_print_err(
                "%s: Base class \"%s\" not found!", p_target_class->name().c_str(), base.c_str());
            return false;
        }

        return class_has_base_class(base_result.get_result(), p_base_class_qualified);
    }

    bool GodotGeneratorUtils::class_is_node_type(const Ref<Class>& p_target_class)
    {
        return class_has_base_class(
            p_target_class, AssumedGodotTypes::Node().type->qualified_name());
    }

    bool GodotGeneratorUtils::class_is_resource_type(const Ref<Class>& p_target_class)
    {
        return class_has_base_class(
            p_target_class, AssumedGodotTypes::Resource().type->qualified_name());
    }

    bool GodotGeneratorUtils::class_is_ref_counted_type(const Ref<Class>& p_target_class)
    {
        return class_has_base_class(
            p_target_class, AssumedGodotTypes::RefCounted().type->qualified_name());
    }

    bool GodotGeneratorUtils::class_is_godot_object_type(const Ref<Class>& p_target_class)
    {
        return class_has_base_class(
            p_target_class, AssumedGodotTypes::Object().type->qualified_name());
    }

    bool GodotGeneratorUtils::get_defaults_for_type(
        const Ref<Type>& p_target_type, Ref<GodotVariantTypeArgument>& p_variant_type,
        Ref<GodotPropertyHintArgument>& p_property_hint,
        Ref<GodotPropertyUsageFlagsArgument>& p_property_usage_flags,
        const Ref<Namespace>& p_from_namespace, DefaultsUsage p_defaults_usage)
    {
        using namespace AssumedParameterValues;
        if (Ref<Type> ref_inner_type;
            type_is_godot_ref_type(p_target_type, ref_inner_type, p_from_namespace) &&
            type_is_ref_counted_type(ref_inner_type, p_from_namespace)) {
            p_variant_type = build_variant_type_argument(VariantTypeObject());
            p_property_hint =
                build_property_hint_argument(HintResourceType(), ref_inner_type->name());
            p_property_usage_flags = build_property_usage_flags_argument(UsageDefault());
            return true;
        }

        if (type_is_node_type(p_target_type, p_from_namespace)) {
            p_variant_type = build_variant_type_argument(VariantTypeObject());
            p_property_hint = build_property_hint_argument(HintNodeType(), p_target_type->name());
            p_property_usage_flags = build_property_usage_flags_argument(UsageDefault());
            return true;
        }

        if (type_is_object_type(p_target_type)) {
            p_variant_type = build_variant_type_argument(VariantTypeObject());
            p_property_hint =
                build_property_hint_argument(HintResourceType(), p_target_type->name());
            p_property_usage_flags = build_property_usage_flags_argument(UsageDefault());
            return true;
        }

        if (Ref<Enum> enum_object;
            type_is_enum_type(p_target_type, enum_object, p_from_namespace)) {
            p_variant_type = build_variant_type_argument(VariantTypeInt());
            Vector<String> hints;
            for (const Ref<EnumValue>& value : enum_object->values()) {
                hints.push_back(format(
                    "%s:%s", cpp_enum_case_to_exposed_enum_case(value->name()).c_str(),
                    value->literal()->content.c_str()));
            }

            Result<GodotEnumAttribute> attribute_result =
                LibraryContext::instance()->get_type_db()->get_type_attribute<GodotEnumAttribute>(
                    p_target_type, p_from_namespace);

            bool is_flags = false;

            if (attribute_result.has_result()) {
                if (const Ref<Identifier> argument_identifier =
                        attribute_result.get_result()
                            ->find_chain<Identifier, Arguments, EnumGeneratorOptionsArgument>()) {
                    is_flags = argument_identifier->name == EnumGeneratorOptionsArgument::EnumFlags;
                }
            }

            if (is_flags) {
                p_property_hint =
                    build_property_hint_argument(HintFlags(), string_vector_combine(hints, ","));
            } else {
                p_property_hint =
                    build_property_hint_argument(HintEnum(), string_vector_combine(hints, ","));
            }
            p_property_usage_flags = build_property_usage_flags_argument(UsageDefault());

            return true;
        }

        if (Ref<Type> inner_type;
            type_is_godot_typed_array_type(p_target_type, inner_type, p_from_namespace)) {
            Ref<GodotVariantTypeArgument> inner_variant_type;
            Ref<GodotPropertyHintArgument> inner_property_hint;
            Ref<GodotPropertyUsageFlagsArgument> _;

            if (!get_defaults_for_type(
                    inner_type, inner_variant_type, inner_property_hint, _, p_from_namespace)) {
                return false;
            }

            p_variant_type = build_variant_type_argument(VariantTypeArray());

            switch (p_defaults_usage) {
            case DEFAULTS_PROPERTY_BINDING: {
                p_property_hint = build_property_hint_argument(
                    HintArrayType(),
                    "vformat(\"%s/%s:%s\", " +
                        format(
                            "%s,%s,%s)",
                            ("Variant::" + inner_variant_type->godot_variant_type()).c_str(),
                            inner_property_hint->godot_property_hint().c_str(),
                            inner_property_hint->hint_string().c_str()),
                    false);
            } break;
            case DEFAULTS_SIGNAL_ARGUMENT: {
                p_property_hint =
                    build_property_hint_argument(HintArrayType(), inner_type->name(), true);
            } break;
            }

            p_property_usage_flags = build_property_usage_flags_argument(UsageDefault());
            return true;
        }

        if (Ref<Type> key_type, value_type; type_is_godot_typed_dictionary_type(
                p_target_type, key_type, value_type, p_from_namespace)) {
            Ref<GodotVariantTypeArgument> key_variant_type;
            Ref<GodotPropertyHintArgument> key_property_hint;
            Ref<GodotVariantTypeArgument> value_variant_type;
            Ref<GodotPropertyHintArgument> value_property_hint;
            Ref<GodotPropertyUsageFlagsArgument> _;

            if (!get_defaults_for_type(
                    key_type, key_variant_type, key_property_hint, _, p_from_namespace) ||
                !get_defaults_for_type(
                    value_type, value_variant_type, value_property_hint, _, p_from_namespace)) {
                return false;
            }
            p_variant_type = build_variant_type_argument(VariantTypeDictionary());
            p_property_usage_flags = build_property_usage_flags_argument(UsageDefault());

            switch (p_defaults_usage) {
            case DEFAULTS_PROPERTY_BINDING: {
                p_property_hint = build_property_hint_argument(
                    HintDictionaryType(),
                    "vformat(\"%s/%s:%s;%s/%s:%s\", " +
                        format(
                            "%s,%s,%s,%s,%s,%s)",
                            ("Variant::" + key_variant_type->godot_variant_type()).c_str(),
                            key_property_hint->godot_property_hint().c_str(),
                            key_property_hint->hint_string().c_str(),
                            ("Variant::" + value_variant_type->godot_variant_type()).c_str(),
                            value_property_hint->godot_property_hint().c_str(),
                            value_property_hint->hint_string().c_str()),
                    false);
            } break;
            case DEFAULTS_SIGNAL_ARGUMENT: {
                p_property_hint = build_property_hint_argument(
                    HintDictionaryType(),
                    format("%s;%s", key_type->name().c_str(), value_type->name().c_str()), true);
            } break;
            }

            return true;
        }

        if (String variant_type_name;
            get_variant_type_from_type(p_target_type, variant_type_name)) {
            p_variant_type = build_variant_type_argument(variant_type_name);
            p_property_hint = build_property_hint_argument(HintNone());
            p_property_usage_flags = build_property_usage_flags_argument(UsageDefault());
            return true;
        }

        return false;
    }

    bool GodotGeneratorUtils::type_is_godot_ref_type(
        const Ref<Type>& p_target_type, Ref<Type>& p_inner_type,
        const Ref<Namespace>& p_from_namespace)
    {
        UNUSED(p_from_namespace);
        Vector<Ref<Type>> inner_types;
        if (type_is_assumed_template_type(
                p_target_type, AssumedGodotTypes::GodotRef(), inner_types)) {
            p_inner_type = inner_types[0];
            return true;
        }
        p_inner_type = nullptr;
        return false;
    }

    bool GodotGeneratorUtils::type_is_godot_typed_array_type(
        const Ref<Type>& p_target_type, Ref<Type>& p_inner_type,
        const Ref<Namespace>& p_from_namespace)
    {
        UNUSED(p_from_namespace);
        Vector<Ref<Type>> inner_types;
        if (type_is_assumed_template_type(
                p_target_type, AssumedGodotTypes::TypedArray(), inner_types)) {
            p_inner_type = inner_types[0];
            return true;
        }
        p_inner_type = nullptr;
        return false;
    }

    bool GodotGeneratorUtils::type_is_godot_typed_dictionary_type(
        const Ref<Type>& p_target_type, Ref<Type>& p_key_type, Ref<Type>& p_value_type,
        const Ref<Namespace>& p_from_namespace)
    {
        UNUSED(p_from_namespace);
        Vector<Ref<Type>> inner_types;
        if (type_is_assumed_template_type(
                p_target_type, AssumedGodotTypes::TypedDictionary(), inner_types)) {
            p_key_type = inner_types[0];
            p_value_type = inner_types[1];
            return true;
        }
        p_key_type = nullptr;
        p_value_type = nullptr;
        return false;
    }

    bool GodotGeneratorUtils::type_is_ref_counted_type(
        const Ref<Type>& p_inner_type, const Ref<Namespace>& p_from_namespace)
    {
        const Result<Class> class_result =
            LibraryContext::instance()->get_type_db()->get_type_data<Class>(
                p_inner_type->type_name_unmodified(), 0, p_from_namespace);
        if (class_result.has_error()) {
            class_result.get_error()->set_handled();
            return false;
        }

        return class_is_ref_counted_type(class_result.get_result());
    }

    bool GodotGeneratorUtils::type_is_object_type(
        const Ref<Type>& p_inner_type, const Ref<Namespace>& p_from_namespace)
    {
        const Result<Class> class_result =
            LibraryContext::instance()->get_type_db()->get_type_data<Class>(
                p_inner_type->type_name_unmodified(), 0, p_from_namespace);
        if (class_result.has_error()) {
            class_result.get_error()->set_handled();
            return false;
        }

        return class_is_godot_object_type(class_result.get_result());
    }

    bool GodotGeneratorUtils::type_is_godot_collection_type(
        const Ref<Type>& p_target_type, const Ref<Namespace>& p_from_namespace)
    {
        UNUSED(p_from_namespace);
        Vector<Ref<Type>> inner_types;
        return p_target_type->type_name_unmodified() ==
                   AssumedGodotTypes::Array().type->qualified_name() ||
               p_target_type->type_name_unmodified() ==
                   AssumedGodotTypes::Dictionary().type->qualified_name() ||
               type_is_assumed_template_type(
                   p_target_type, AssumedGodotTypes::TypedArray(), inner_types) ||
               type_is_assumed_template_type(
                   p_target_type, AssumedGodotTypes::TypedDictionary(), inner_types);
    }

    bool GodotGeneratorUtils::type_is_node_type(
        const Ref<Type>& p_target_type, const Ref<Namespace>& p_from_namespace)
    {
        const Result<Class> class_result =
            LibraryContext::instance()->get_type_db()->get_type_data<Class>(
                p_target_type->type_name_unmodified(), 0, p_from_namespace);
        if (class_result.has_error()) {
            class_result.get_error()->set_handled();
            return false;
        }

        return class_is_node_type(class_result.get_result());
    }

    bool GodotGeneratorUtils::type_is_enum_type(
        const Ref<Type>& p_target_type, Ref<Enum>& p_enum_object,
        const Ref<Namespace>& p_from_namespace)
    {
        const Result<Enum> enum_result =
            LibraryContext::instance()->get_type_db()->get_type_data<Enum>(
                p_target_type->type_name_unmodified(), 0, p_from_namespace);
        if (enum_result.has_error()) {
            enum_result.get_error()->set_handled();
            p_enum_object = nullptr;
            return false;
        }
        p_enum_object = enum_result.get_result();
        return true;
    }

    bool GodotGeneratorUtils::type_is_variant_type(
        const Ref<Type>& p_target_type, const Ref<Namespace>& p_from_namespace)
    {
        UNUSED(p_from_namespace);

        ensure_type_dicts_initialized();
        String type_name = type_name_remove_usings(p_target_type->type_name_unmodified());
        return _type_to_variant_type.find(type_name) != _type_to_variant_type.end();
    }

    bool GodotGeneratorUtils::type_is_primitive_type(const Ref<Type>& p_target_type)
    {
        String variant_type;
        if (get_variant_type_from_type(p_target_type, variant_type)) {
            if (variant_type == AssumedParameterValues::VariantTypeInt() ||
                variant_type == AssumedParameterValues::VariantTypeFloat() ||
                variant_type == AssumedParameterValues::VariantTypeBool()) {
                return true;
            }
        }
        return false;
    }

    bool GodotGeneratorUtils::get_variant_type_from_type(
        const Ref<Type>& p_target_type, String& p_variant_type)
    {
        ensure_type_dicts_initialized();
        String type_name = type_name_remove_usings(p_target_type->type_name_unmodified());
        if (auto itr = _type_to_variant_type.find(type_name); itr != _type_to_variant_type.end()) {
            p_variant_type = itr->second;
            return true;
        }
        p_variant_type = "";
        return false;
    }

    Ref<GodotVariantTypeArgument>
    GodotGeneratorUtils::build_variant_type_argument(const Ref<Type>& p_type)
    {
        Ref<Type> inner_type;
        if (type_is_object_type(p_type) || type_is_godot_ref_type(p_type, inner_type)) {
            return build_variant_type_argument(AssumedParameterValues::VariantTypeObject());
        }

        if (String variant_type; get_variant_type_from_type(p_type, variant_type)) {
            return build_variant_type_argument(variant_type);
        }

        return build_variant_type_argument(AssumedParameterValues::VariantTypeNil());
    }

    Ref<GodotVariantTypeArgument>
    GodotGeneratorUtils::build_variant_type_argument(const String& p_variant_type)
    {
        return B<GodotVariantTypeArgument>()[B<Identifier>(p_variant_type)];
    }

    Ref<GodotPropertyUsageFlagsArgument>
    GodotGeneratorUtils::build_property_usage_flags_argument(const String& p_usage)
    {
        return B<GodotPropertyUsageFlagsArgument>()[B<Identifier>(p_usage)];
    }

    Ref<Node> GodotGeneratorUtils::build_property_info(
        const Ref<GodotVariantTypeArgument>& p_variant_type,
        const Ref<GodotPropertyHintArgument>& p_hint,
        const Vector<Ref<GodotPropertyUsageFlagsArgument>>& p_usages, const String& p_property_name)
    {
        Ref<Arguments> arguments;
        Ref<Output::ListNode> flags;

        Ref<Node> property_usage = R<Output::ListNode>(&flags, " | ", false, false);

        Ref<Node> result = B<Function>()[{
            B<Identifier>(AssumedGodotTypes::PropertyInfo().type->qualified_name()),
            R<Arguments>(&arguments)[{
                B<Argument>()[B<Literal>(format(
                    "%s::%s", AssumedGodotTypes::Variant().type->qualified_name().c_str(),
                    p_variant_type->godot_variant_type().c_str()))],
                B<Argument>()[Literal::StringLiteral(p_property_name)],
                B<Argument>()[B<Literal>(format(
                    "%s::%s", AssumedGodotTypes::PropertyHintEnum().type->qualified_name().c_str(),
                    p_hint->godot_property_hint().c_str()))],
                B<Argument>()[B<Literal>(p_hint->hint_string())], property_usage}]}];

        if (p_usages.empty()) {
            arguments->remove_child(flags);
        } else {
            for (const Ref<GodotPropertyUsageFlagsArgument>& usage : p_usages) {
                flags->add_child(B<Literal>(format(
                    "%s::%s",
                    AssumedGodotTypes::PropertyUsageFlagsEnum().type->qualified_name().c_str(),
                    usage->godot_property_usage_flag().c_str())));
            }
        }

        return result;
    }

    Ref<Node> GodotGeneratorUtils::build_property_info(
        const Ref<GodotVariantTypeArgument>& p_variant_type, const String& p_property_name,
        ClassGeneratorResult& r_result)
    {
        r_result.add_source_include(AssumedGodotTypes::Variant().type->header);
        r_result.add_source_include(AssumedGodotTypes::PropertyInfo().type->header);
        return B<Function>()[{
            B<Identifier>(AssumedGodotTypes::PropertyInfo().type->qualified_name()),
            B<Arguments>()[{
                B<Argument>()[B<Literal>(format(
                    "%s::%s", AssumedGodotTypes::Variant().type->qualified_name().c_str(),
                    p_variant_type->godot_variant_type().c_str()))],
                B<Argument>()[Literal::StringLiteral(p_property_name)],
            }]}];
    }

    Result<Node> GodotGeneratorUtils::build_property_info_defaults(
        const Ref<Type>& p_type, const String& p_property_name, ClassGeneratorResult& r_result,
        const Ref<Namespace>& p_from_namespace, DefaultsUsage p_usage)
    {
        UNUSED(r_result);

        Ref<GodotVariantTypeArgument> variant_type;
        Ref<GodotPropertyHintArgument> property_hint;
        Ref<GodotPropertyUsageFlagsArgument> usage_flags;

        if (!get_defaults_for_type(
                p_type, variant_type, property_hint, usage_flags, p_from_namespace, p_usage)) {
            ERROR("Failed to get default property info.")
        }

        return build_property_info(variant_type, property_hint, {usage_flags}, p_property_name);
    }

    Ref<GodotPropertyHintArgument> GodotGeneratorUtils::build_property_hint_argument(
        const String& p_value, const String& p_hint_string, bool p_is_string_literal)
    {
        auto literal = p_is_string_literal ? Literal::StringLiteral(p_hint_string)
                                           : node_new<Literal>(p_hint_string);
        return B<GodotPropertyHintArgument>()[{
            B<Identifier>(p_value), B<Arguments>()[{
                                        B<Argument>()[literal],
                                    }]}];
    }

} // namespace GodotObjectCompiler
