
#include "library_godot/generators/godot_generator_utils.h"

#include "library/tree/output/output.h"
#include "library/tree/predicates.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/context.h"
#include "library/tree/syntax/function.h"
#include "library/tree/syntax/modifiers.h"
#include "library/tree/syntax/type.h"

namespace GodotObjectCompiler {

  Type* GodotGeneratorUtils::const_ref(const String& type_name) {
    return build<Type>().with_children({build<Const>(), build<Identifier>(type_name), build<Reference>()});
  }

  Function* GodotGeneratorUtils::bind_method(
      const String& class_name, const String& method_name, const Vector<String>& parameter_names) {
    // clang-format off
    Arguments* arguments;

    Function* result =  build<Function>().with_children({
    build<Identifier>("ClassDB::bind_method"),
    build<Arguments>().with_children({
      build<Function>().with_children({
        build<Identifier>("D_METHOD"),
        build_ref<Arguments>(&arguments).with_children({
            build<Argument>().with_child(Writer::StringLiteral(method_name))
          })
        }),
    build<Argument>().with_children({
        build<Reference>(),
        build<Identifier>(class_name),
        Writer::Text("::"),
        Writer::Text(method_name),
    })})}).with_child(Writer::Semicolon());

    for ( const String& parameter_name : parameter_names) {
      arguments->build_child<Argument>().with_child(Writer::StringLiteral(parameter_name));
    }

    return result;
    // clang-format on
  }



  Function* GodotGeneratorUtils::bind_static_method(
      const String& class_name, const String& method_name, const Vector<String>& parameter_names) {
    // clang-format off
    Arguments* arguments = node_new<Arguments>();

    Function* result =  build<Function>().with_children({
    build<Identifier>("ClassDB::bind_static_method"),
    build<Arguments>().with_children({
      build<Argument>().with_child(Writer::StringLiteral(class_name)),
      build<Function>().with_children({
        build<Identifier>("D_METHOD"),
        build_ref<Arguments>(&arguments).with_children({
            build<Argument>().with_child(Writer::StringLiteral(method_name))
          })
        }),
    build<Argument>().with_children({
        build<Reference>(),
        build<Identifier>(class_name),
        Writer::Text("::"),
        Writer::Text(method_name),
    })})}).with_child(Writer::Semicolon());

    for ( const String& parameter_name : parameter_names) {
      arguments->build_child<Argument>().with_child(Writer::StringLiteral(parameter_name));
    }

    return result;
    // clang-format on
  }

  Body* GodotGeneratorUtils::get_or_create_bind_methods_body(
      Class* target_class, Context* generated_body, Context* generated_sources) {
    Function* bind_methods = generated_sources->find_child(
        0, NamedContextPredicates::name<Function>((target_class->name() + "::_bind_methods").c_str()));
    Body* bind_methods_body;

    if (!bind_methods) {
      // clang-format off
      generated_sources->build_child<Function>().with_children({
      build<Type>().with_child<Identifier>("void"),
      build<Identifier>(target_class->name() + "::" + "_bind_methods"),
      build<Parameters>(),
      build_ref<Body>(&bind_methods_body)}
      );

      generated_body->build_child<Function>().with_children({
        build<Static>(),
        build<Type>().with_child<Identifier>("void"),
        build<Identifier>("_bind_methods"),
        build<Parameters>()
      }).with_child(Writer::Semicolon());
      // clang-format on
    } else {
      bind_methods_body = bind_methods->find_child<Body>();
    }
    return bind_methods_body;
  }

}  // namespace GodotObjectCompiler