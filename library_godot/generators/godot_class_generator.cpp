
#include "godot_class_generator.h"

#include "godot_generator_utils.h"
#include "library/tree/output/output.h"

namespace GodotObjectCompiler {

  GeneratorError* GodotClassGenerator::do_generate(Class* target_class, GodotClassAttribute* attribute,
      Context* generated_body, Context* generated_sources, Context* generated_global) {
    GEN_ERROR_COND(
        attribute->resolve_target() != target_class, "Resolved class is not the provided target class. Abort!");

    Vector<String> bases = target_class->direct_bases_names();
    GEN_ERROR_COND(bases.empty(),
        "Target class does not name base classes and thus cannot inherit from a Godot object type. Abort!");

    // clang-format off
    Function* gd_class = build<Function>().with_children({
      build<Identifier>("GDCLASS"),
      build<Arguments>().with_children({
        build<Argument>().with_child(Writer::Text(target_class->name())),
        build<Argument>().with_child(Writer::Text(bases[0])),
      })
    }).with_child(Writer::Semicolon());

    // clang-format on

    generated_body->add_child(gd_class);
    GodotGeneratorUtils::get_or_create_bind_methods_body(target_class, generated_body, generated_sources);

    return GeneratorError::OK;
  }

}  // namespace GodotObjectCompiler