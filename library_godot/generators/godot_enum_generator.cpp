
#include "godot_enum_generator.h"

#include "godot_generator_utils.h"
#include "library/tree/predicates.h"

namespace GodotObjectCompiler {

  Ref<GeneratorError> GodotEnumGenerator::do_generate(Ref<Class> target_class, Ref<GodotEnumAttribute> attribute,
      Ref<Context> generated_body, Ref<Context> generated_sources, Ref<Context> generated_global) {

    Ref<Node> target_node = attribute->resolve_target();
    GEN_ERROR_COND(!target_node, "Could not find target for Enum marco.");

    Ref<Enum> target_enum = target_node->as<Enum>();
    GEN_ERROR_COND(!target_enum, "Resolved target for enum macro is not an enum");

    Ref<Body> bind_methods_body = GodotGeneratorUtils::get_or_create_bind_methods_body(target_class,generated_body,generated_sources);

    for (const String& name : target_enum->value_names()) {
      // clang-format off
      bind_methods_body->build_child<Function>().with_children({
        build<Identifier>("BIND_ENUM_CONSTANT"),
        build<Arguments>().with_child(
          build<Argument>().with_child(
            Writer::Text(name)
          )
        )
      }).with_child(Writer::Semicolon());
      // clang-format on
    }

    // clang-format off

    generated_global->build_child<Function>().with_children({
      build<Identifier>("VARIANT_ENUM_CAST"),
      build<Arguments>().with_child(
        build<Argument>().with_child(
          Writer::Text(target_enum->qualified_name())
          )
        )
    }).with_child(Writer::Semicolon());

    // clang-format on
    return GeneratorError::OK;
  }

}  // namespace GodotObjectCompiler