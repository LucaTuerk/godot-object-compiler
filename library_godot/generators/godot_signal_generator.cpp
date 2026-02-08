
#include "godot_signal_generator.h"

#include "godot_generator_utils.h"

namespace GodotObjectCompiler {

  Ref<GeneratorError> GodotSignalGenerator::do_generate(Ref<Class> p_target_class,
      Ref<GodotSignalAttribute> p_attribute, Ref<Context> p_generated_body, Ref<Context> p_generated_sources,
      Ref<Context> p_generated_global) {
    using namespace GodotGeneratorUtils;
    Ref<Body> bind_methods = get_or_create_bind_methods_body(p_target_class, p_generated_body, p_generated_sources);
    GEN_ERROR_COND(!bind_methods, p_target_class, "Failed to get or generate bind methods body.");

    Ref<Function> target_function = p_attribute->TargetFunction();
    GEN_ERROR_COND(!target_function, p_target_class, "Failed to get signal target function");

    bool is_void = target_function->type()->name() == "void";
    GEN_ERROR_COND(!is_void, target_function, "Signal target function does not return void.");

    // clang-format off
    Ref<Arguments> arguments;
    bind_methods->build_child<Function>().with_children({
        build<Identifier>("ADD_SIGNAL"),
        build<Arguments>().with_child(
          build<Argument>().with_children( {
      build<Function>().with_children({
        build<Identifier>("MethodInfo"),
        build_ref<Arguments>(&arguments).with_children({
            build<Argument>().with_child(
              Writer::StringLiteral(target_function->name())
            ),
          })
        })
      }))
    }).with_child(Writer::Semicolon());
    // clang-format on

    Ref<Parameters> func_parameters;
    Ref<Arguments> emit_arguments;
    // clang-format off
    p_generated_sources->build_child<Function>().with_children({
      build<Type>().with_child<Identifier>("void"),
      build<Identifier>(p_target_class->name() + "::" + target_function->name()),
      build_ref<Parameters>(&func_parameters),
      build<Body>().with_children({
        build<Function>().with_children({
          build<Identifier>("emit_signal"),
          build_ref<Arguments>(&emit_arguments).with_children({
            build<Argument>().with_child(Writer::StringLiteral(target_function->name())),
          })
        }).with_child(Writer::Semicolon()),
      })
    });
    // clang-format on

    Size i = 1;
    for (const Ref<Parameter>& parameter : target_function->parameters()->find_children<Parameter>()) {
      Ref<Type> type = parameter->find_child<Type>();
      GEN_ERROR_COND(!type, target_function, "Failed to get function argument type.");

      Ref<Identifier> identifier = parameter->find_child<Identifier>();
      String name = identifier ? "p_" + identifier->name : format("p_param_%d", i);

      arguments->build_child<Argument>().with_child(PropertyInfoDefaultForType(type, name, DEFAULTS_SIGNAL_ARGUMENT));

      func_parameters->build_child<Parameter>().with_children({
          type->clone(),
          build<Identifier>(name),
      });

      emit_arguments->build_child<Argument>().with_children({
          build<Identifier>(name),
      });

      i += 1;
    }

    return GeneratorError::OK;
  }

}  // namespace GodotObjectCompiler