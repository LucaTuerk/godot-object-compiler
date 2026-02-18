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
#include "library/tree/output/output.h"
#include "library/tree/predicates.h"
#include "library/tree/syntax/class.h"
#include "library_godot/attributes/godot_attributes.h"
#include "library_godot/generated_assumptions/parameter_types.h"

namespace GodotObjectCompiler {

  Ref<GeneratorError> GodotPropertyGenerator::do_generate_default_attribute_arguments(
      Ref<Class> p_target_class, Ref<GodotPropertyAttribute> p_attribute, Ref<Context> p_default_values) {
    using namespace GodotGeneratorUtils;
    using namespace AssumedParameterValues;

    Ref<Field> target_field = p_attribute->TargetField();
    GEN_ERROR_COND(!target_field, p_attribute, "Failed to get target field for GodotProperty attribute.");

    const Ref<Type> field_type = target_field->type();
    GEN_ERROR_COND(!field_type, target_field, "Target field does not name a type.");

    const String field_type_name = field_type->type_name();
    GEN_ERROR_COND(field_type_name.empty(), target_field, "Invalid type name for target field.");

    Ref<GodotVariantTypeArgument> variant_type;
    Ref<GodotPropertyHintArgument> property_hint;
    Ref<GodotPropertyUsageFlagsArgument> property_usage_flags;

    if (!get_defaults_for_type(field_type, variant_type, property_hint, property_usage_flags, p_target_class)) {
      GEN_ERROR(target_field, "Unknown type. Failed to determine default property info.");
    }

    Ref<PropertyGetAccessSpecifierArgument> property_get_access_specifier =
        node_new<PropertyGetAccessSpecifierArgument>();
    Ref<PropertySetAccessSpecifierArgument> property_set_access_specifier =
        node_new<PropertySetAccessSpecifierArgument>();

    property_get_access_specifier->build_child<Identifier>(PropertyGetAccessSpecifierArgument::PublicGet);
    if (target_field->is_private_member()) {
      property_set_access_specifier->build_child<Identifier>(PropertySetAccessSpecifierArgument::PrivateSet);
    } else if (target_field->is_protected_member()) {
      property_set_access_specifier->build_child<Identifier>(PropertySetAccessSpecifierArgument::ProtectedSet);
    } else {
      property_set_access_specifier->build_child<Identifier>(PropertySetAccessSpecifierArgument::PublicSet);
    }

    p_default_values->add_children({variant_type, property_hint, property_usage_flags, property_get_access_specifier,
        property_set_access_specifier});
    return GeneratorError::OK;
  }

  Ref<GeneratorError> GodotPropertyGenerator::do_generate(Ref<Class> p_target_class,
      Ref<GodotPropertyAttribute> p_attribute, Ref<Context> p_generated_body, Ref<Context> p_generated_sources,
      Ref<Context> p_generated_global) {
    UNUSED(p_generated_global);

    using namespace GodotGeneratorUtils;

    Ref<Body> bind_methods_body =
        get_bind_methods_body(p_target_class, p_generated_body, p_generated_sources);
    GEN_ERROR_COND(!bind_methods_body, p_target_class, "Failed to find or generate the _bind_methods function body.");

    Ref<Body> get_property_list_body =
        get_get_property_list_body(p_target_class, p_generated_body, p_generated_sources);
    GEN_ERROR_COND(
        !get_property_list_body, p_target_class, "Failed to find or generate the _get_property_list function body.");

    Ref<Context> generated_public_members, generated_protected_members, generated_private_members;
    GEN_ERROR_COND(unzip_generated_body(p_generated_body, &generated_public_members, &generated_protected_members,
                       &generated_private_members) != GeneratorError::OK,
        p_target_class, "Failed to find generated body groups");

    const Ref<Field> target_field = p_attribute->TargetField();

    GEN_ERROR_COND(!target_field, p_attribute, "Failed to get target field for property attribute");
    GEN_ERROR_COND(target_field->is_const(), target_field, "Property target field is constant.");
    GEN_ERROR_COND(target_field->is_static(), target_field, "Property target field is static.");

    const Ref<Type> field_type = target_field->type();
    GEN_ERROR_COND(!field_type, p_target_class, "Field does not name a type. Abort!");

    const String property_name = target_field->name();
    const String type_name = field_type->type_name();
    const String getter_name = format("get_%s", property_name.c_str());
    const String setter_name = format("set_%s", property_name.c_str());

    Ref<Type> ref_inner;
    Ref<Enum> enum_object;
    bool is_obj_type = type_is_object_type(field_type, p_target_class);
    bool is_enum_type = type_is_enum_type(field_type, enum_object, p_target_class);

    bind_methods_body->add_child(bind_method(p_target_class->name(), getter_name, {}));
    bind_methods_body->add_child(bind_method(p_target_class->name(), setter_name, {property_name}));

    Ref<Node> get_type = field_type->clone();
    Ref<Node> set_type = !is_obj_type ? const_ref(type_name) : field_type->clone();
    if (is_enum_type) {
      get_type = build<Type>().with_child<Identifier>("int");
      set_type = build<Type>().with_child<Identifier>("int");
    }

    // clang-format off
    Ref<Function> get_def = build<Function>().with_children({
      get_type->clone(),
      build<Identifier>(getter_name),
      build<Parameters>(),
      build<Const>()
    }).with_child(Output::Semicolon());

    Ref<Function> get_impl = build<Function>().with_children({
      get_type->clone(),
      build<Identifier>(p_target_class->name()  + "::" + getter_name),
      build<Parameters>(),
      build<Const>(),
      build<Body>().with_child(
          Output::Return(property_name)
        )
    });

    Ref<Function> set_def = build<Function>().with_children({
    build<Type>().with_child<Identifier>("void"),
      build<Identifier>(setter_name),
      build<Parameters>().with_child(
        build<Parameter>().with_children({
          set_type->clone(),
          build<Identifier>("p_" + property_name),
      }))
    }).with_child(Output::Semicolon());

    Ref<Function> set_impl = build<Function>().with_children({
      build<Type>().with_child<Identifier>("void"),
      build<Identifier>(p_target_class->name()  + "::" + setter_name),
      build<Parameters>()
          .with_child(
            build<Parameter>()
            .with_children({
              set_type->clone(),
              build<Identifier>("p_"+property_name)
            })
          ),
      build<Body>().with_child(
        (is_enum_type ?
          Output::Assign(property_name, Output::Text(format("static_cast<%s>(p_%s)", enum_object->qualified_name().c_str(),property_name.c_str()))):
          Output::Assign(property_name, Output::Text(format("p_%s", property_name.c_str()))))),
    });

    Ref<GodotVariantTypeArgument> variant_type = p_attribute->arguments()->find_child<GodotVariantTypeArgument>();
    GEN_ERROR_COND(!variant_type, p_attribute, "Failed to get variant type argument");

    Ref<GodotPropertyHintArgument> property_hint = p_attribute->arguments()->find_child<GodotPropertyHintArgument>();
    GEN_ERROR_COND(!property_hint, p_attribute, "Failed to get property hint argument");

    Vector<Ref<GodotPropertyUsageFlagsArgument>> usage_flags = p_attribute->arguments()->find_children<GodotPropertyUsageFlagsArgument>();

    Ref<Node> property_info = build_property_info(variant_type, property_hint, usage_flags, property_name);
    Ref<Node> property_info_no_editor = build_property_info(variant_type, property_hint, usage_flags, property_name,true);

    Ref<Function> add_property = build<Function>().with_children({
      build<Identifier>("ADD_PROPERTY"),
      build<Arguments>().with_children({
        build<Argument>().with_children({
          property_info_no_editor
        }),
        build<Argument>().with_child(Output::StringLiteral(setter_name)),
        build<Argument>().with_child(Output::StringLiteral(getter_name)),
      })
    }).with_child(Output::Semicolon());

    get_property_list_body->build_child<Function>().with_children({
      build<Identifier>("p_list->push_back"),
      build<Arguments>().with_children({
        build<Argument>().with_child(property_info)
      })
    }).with_child(Output::Semicolon());
    // clang-format on

    Ref<PropertyGetAccessSpecifierArgument> get_access_specifier_argument =
        p_attribute->arguments()->find_child<PropertyGetAccessSpecifierArgument>();
    GEN_ERROR_COND(!get_access_specifier_argument, p_attribute, "Failed to get get specifier argument");

    Ref<PropertySetAccessSpecifierArgument> set_access_specifier_argument =
        p_attribute->arguments()->find_child<PropertySetAccessSpecifierArgument>();
    GEN_ERROR_COND(!set_access_specifier_argument, p_attribute, "Failed to get set specifier argument");

    AccessSpecifier::Type get_specifier, set_specifier;
    GEN_ERROR_COND(!get_access_specifier_argument->get_specifier(get_specifier), p_attribute, "Failed to get property getter access specifier.");
    GEN_ERROR_COND(!set_access_specifier_argument->get_specifier(set_specifier), p_attribute, "Failed to get property setter access specifier.");

    switch (get_specifier) {
      case AccessSpecifier::PUBLIC: {
        generated_public_members->add_child(get_def);
      }break;
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
      }break;
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

    bind_methods_body->add_child(add_property);

    Ref<Body> property_names_body = get_property_names_body(p_target_class, p_generated_body);
    GEN_ERROR_COND(!property_names_body, p_attribute, "Failed to get property names body.");

    property_names_body->add_child(
        Output::Text(format("static const StringName& %s() {static const StringName sn = \"%s\"; return sn; }",
            property_name.c_str(), property_name.c_str())));

    return GeneratorError::OK;
  }

}