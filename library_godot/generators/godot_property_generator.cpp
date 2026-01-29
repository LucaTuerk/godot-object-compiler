
#include "library_godot/generators/godot_property_generator.h"

#include "../attributes/godot_attributes.h"
#include "godot_generator_utils.h"
#include "library/tree/output/generator_error.h"
#include "library/tree/output/output.h"
#include "library/tree/output/output_file.h"
#include "library/tree/syntax/class.h"

namespace GodotObjectCompiler {

  Ref<GeneratorError> GodotPropertyGenerator::do_generate(Ref<Class> target_class,
      Ref<GodotPropertyAttribute> attribute, Ref<Context> generated_body, Ref<Context> generated_sources,
      Ref<Context> generated_global) {
    using namespace GodotGeneratorUtils;

    Ref<Node> target = attribute->resolve_target();
    Ref<Body> bind_methods_body = get_or_create_bind_methods_body(target_class, generated_body, generated_sources);
    GEN_ERROR_COND(!bind_methods_body, target_class, "Failed to find or generate the _bind_methods function body.");

    if (const Ref<Field> target_field = target->as<Field>()) {
      const String property_name = target_field->name();
      const Ref<Type> field_type = target_field->type();

      GEN_ERROR_COND(target_field->is_const(), target_class, "Target field is a Constant. Abort!");
      GEN_ERROR_COND(target_field->is_static(), target_class, "Target field is a static Field. Abort!");
      GEN_ERROR_COND(!field_type, target_class, "Field does not name a type. Abort!");

      const String type_name = field_type->type_name();
      const String getter_name = "get_" + property_name;
      const String setter_name = "set_" + property_name;

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

      Ref<Function> add_property = build<Function>().with_children({
        build<Identifier>("ADD_PROPERTY"),
        build<Arguments>().with_children({
          build<Argument>().with_children({
            build<Function>().with_children({
                build<Identifier>("PropertyInfo"),
              build<Arguments>().with_children({
                build<Argument>().with_child(Writer::Text("Variant::BOOL")),
                build<Argument>().with_child(Writer::StringLiteral(property_name)),
              })
            })
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