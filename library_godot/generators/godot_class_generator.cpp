
#include "godot_class_generator.h"

#include "godot_generator_utils.h"
#include "library/tree/output/output.h"

namespace GodotObjectCompiler {

  Ref<GeneratorError> GodotClassGenerator::do_generate(Ref<Class> target_class, Ref<GodotClassAttribute> attribute,
      Ref<Context> generated_body, Ref<Context> generated_sources, Ref<Context> generated_global) {
    using namespace GodotGeneratorUtils;

    GEN_ERROR_COND(attribute->resolve_target() != target_class, target_class,
        "Resolved class is not the provided target class.");

    Vector<String> bases = target_class->direct_bases_names();
    GEN_ERROR_COND(bases.empty(), target_class,
        "Target class does not name base classes and thus cannot inherit from a Godot object type.");

    if (!class_is_godot_object_type(target_class)) {
      GEN_ERROR(
          target_class, "Target class does not inherit from a godot object class or the class was not found.");
    }

    // clang-format off
    Ref<Function> gd_class = build<Function>().with_children({
      build<Identifier>("GDCLASS"),
      build<Arguments>().with_children({
        build<Argument>().with_child(Writer::Text(target_class->name())),
        build<Argument>().with_child(Writer::Text(bases[0])),
      })
    }).with_child(Writer::Semicolon());
    // clang-format on

    generated_body->add_child(gd_class);
    get_or_create_bind_methods_body(target_class, generated_body, generated_sources);

    return GeneratorError::OK;
  }

}  // namespace GodotObjectCompiler