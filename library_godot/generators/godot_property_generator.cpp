
#include "library_godot/generators/godot_property_generator.h"

#include "../attributes/godot_attributes.h"
#include "godot_generator_utils.h"
#include "library/tree/output/generator_error.h"
#include "library/tree/output/output.h"
#include "library/tree/output/output_file.h"
#include "library/tree/syntax/class.h"
#include "library_godot/generated_assumptions/parameter_types.h"

namespace GodotObjectCompiler {

  Ref<GeneratorError> GodotPropertyGenerator::do_generate_default_attribute_arguments(
      Ref<Class> target_class, Ref<GodotPropertyAttribute> attribute, Ref<Context> default_values) {
    using namespace GodotGeneratorUtils;
    using namespace AssumedParameterValues;

    Ref<Field> target_field = attribute->TargetField();
    GEN_ERROR_COND(!target_field, attribute, "Failed to get target field for GodotProperty attribute.");

    Ref<Type> field_type = target_field->type();
    GEN_ERROR_COND(!field_type, target_field, "Target field does not name a type.");

    String field_type_name = field_type->type_name();
    GEN_ERROR_COND(field_type_name.empty(), target_field, "Invalid type name for target field.");

    if (Ref<Type> ref_inner_type;
        type_is_godot_ref_type(field_type, ref_inner_type) && type_is_ref_counted_type(ref_inner_type)) {
      // clang-format off
      default_values->add_children({
        VariantType(VariantTypeObject()),
        PropertyHint(HintResourceType(), ref_inner_type->name()),
        PropertyUsageFlag(UsageDefault())
      });
      // clang-format on
    } else if (type_is_node_type(field_type)) {
      // clang-format off
      default_values->add_children({
        VariantType(VariantTypeObject()),
        PropertyHint(HintNodeType(), field_type->name()),
        PropertyUsageFlag(UsageDefault())
      });
      // clang-format on
    } else if (type_is_object_type(field_type)) {
      // clang-format off
      default_values->add_children({
        VariantType(VariantTypeObject()),
        PropertyHint(HintResourceType(), field_type->name()),
        PropertyUsageFlag(UsageDefault())
      });
      // clang-format on
    } else if (String variant_type; get_variant_type_from_type(field_type, variant_type)) {
      // clang-format off
      default_values->add_children({
          VariantType(variant_type),
          PropertyHint(HintNone()),
          PropertyUsageFlag(UsageDefault()),
      });
      // clang-format on
    } else {
      GEN_ERROR(target_field, "Unknown type. Failed to determine default property info.");
    }

    return GeneratorError::OK;
  }

  Ref<GeneratorError> GodotPropertyGenerator::do_generate(Ref<Class> target_class,
      Ref<GodotPropertyAttribute> attribute, Ref<Context> generated_body, Ref<Context> generated_sources,
      Ref<Context> generated_global) {
    using namespace GodotGeneratorUtils;

    Ref<Body> bind_methods_body = get_or_create_bind_methods_body(target_class, generated_body, generated_sources);
    GEN_ERROR_COND(!bind_methods_body, target_class, "Failed to find or generate the _bind_methods function body.");

    if (const Ref<Field> target_field = attribute->TargetField()) {
      const String property_name = target_field->name();
      const Ref<Type> field_type = target_field->type();

      GEN_ERROR_COND(target_field->is_const(), target_field, "Property target field is constant.");
      GEN_ERROR_COND(target_field->is_static(), target_field, "Property target field is static.");
      GEN_ERROR_COND(!field_type, target_class, "Field does not name a type. Abort!");

      const String type_name = field_type->type_name();
      const String getter_name = format("get_%s", property_name.c_str());
      const String setter_name = format("set_%s", property_name.c_str());

      bind_methods_body->add_child(bind_method(target_class->name(), getter_name, {}));
      bind_methods_body->add_child(bind_method(target_class->name(), setter_name, {property_name}));

      // clang-format off
      Ref<Function> get_def = build<Function>().with_children({
        const_ref(type_name),
        build<Identifier>(getter_name),
        build<Parameters>(),
        build<Const>()
      }).with_child(Writer::Semicolon());

      Ref<Function> get_impl = build<Function>().with_children({
        const_ref(type_name),
        build<Identifier>(target_class->name()  + "::" + getter_name),
        build<Parameters>(),
        build<Const>(),
        build<Body>().with_child(
            Writer::Return(property_name)
          )
      });

      Ref<Function> set_def = build<Function>().with_children({
      build<Type>().with_child<Identifier>("void"),
        build<Identifier>(setter_name),
        build<Parameters>().with_child(
          build<Parameter>().with_children({
            const_ref(type_name),
            build<Identifier>("p_" + property_name),
        }))
      }).with_child(Writer::Semicolon());

      Ref<Function> set_impl = build<Function>().with_children({
        build<Type>().with_child<Identifier>("void"),
        build<Identifier>(target_class->name()  + "::" + setter_name),
        build<Parameters>()
            .with_child(
              build<Parameter>()
              .with_children({
                const_ref(type_name),
                build<Identifier>("p_"+property_name)
              })
            ),
        build<Body>().with_child(Writer::Assign(property_name, Writer::Text("p_" + property_name))),
      });

      Ref<GodotVariantTypeArgument> variant_type = attribute->arguments()->find_child<GodotVariantTypeArgument>();
      Ref<GodotPropertyHintArgument> property_hint = attribute->arguments()->find_child<GodotPropertyHintArgument>();
      Vector<Ref<GodotPropertyUsageFlagsArgument>> usage_flags = attribute->arguments()->find_children<GodotPropertyUsageFlagsArgument>();

      Ref<Function> add_property = build<Function>().with_children({
        build<Identifier>("ADD_PROPERTY"),
        build<Arguments>().with_children({
          build<Argument>().with_children({
            PropertyInfo(variant_type, property_hint, usage_flags, property_name)
          }),
          build<Argument>().with_child(Writer::StringLiteral(setter_name)),
          build<Argument>().with_child(Writer::StringLiteral(getter_name)),
        })
      }).with_child(Writer::Semicolon());
      // clang-format on

      generated_body->add_child(get_def);
      generated_body->add_child(set_def);
      generated_sources->add_child(get_impl);
      generated_sources->add_child(set_impl);
      bind_methods_body->add_child(add_property);
    }

    return GeneratorError::OK;
  }

}  // namespace GodotObjectCompiler