
#include "godot_function_generator.h"

#include "godot_generator_utils.h"

namespace GodotObjectCompiler {

  GeneratorError* GodotFunctionGenerator::do_generate(Class* target_class, GodotFunctionAttribute* attribute,
      Context* generated_body, Context* generated_sources, Context* generated_global) {
    Node* target_node = attribute->resolve_target();
    GEN_ERROR_COND(target_node == nullptr, "Could not resolve target node for function macro.");

    Function* target_function = target_node->as<Function>();
    GEN_ERROR_COND(target_function == nullptr, "Resolved not for function macro is not a function.");

    Body* bind_methods_body =
        GodotGeneratorUtils::get_or_create_bind_methods_body(target_class, generated_body, generated_sources);

    Parameters* parameters = target_function->parameters();
    GEN_ERROR_COND(
        !parameters, "Function does not name parameters, this was probably parsed as a function call. Abort");

    Vector<String> parameter_names;

    for (Node* child : *parameters) {
      Parameter* parameter = child->as<Parameter>();
      if (!parameter) {
        continue;
      }
      parameter_names.push_back(parameter->name());
    }

    if (target_function->is_static()) {
      bind_methods_body->add_child(
          GodotGeneratorUtils::bind_static_method(target_class->name(), target_function->name(), parameter_names));
    } else {
      bind_methods_body->add_child(
          GodotGeneratorUtils::bind_method(target_class->name(), target_function->name(), parameter_names));
    }

    return GeneratorError::OK;
  }

}  // namespace GodotObjectCompiler