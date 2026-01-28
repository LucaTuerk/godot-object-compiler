
#include "library_godot/generators/godot_generator_utils.h"

#include "../../library/type_db.h"
#include "library/tree/output/output.h"
#include "library/tree/predicates.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/context.h"
#include "library/tree/syntax/function.h"
#include "library/tree/syntax/modifiers.h"
#include "library/tree/syntax/type.h"

namespace GodotObjectCompiler {

  Ref<Type> GodotGeneratorUtils::const_ref(const String& type_name) {
    return build<Type>().with_children({build<Const>(), build<Identifier>(type_name), build<Reference>()});
  }

  Ref<Function> GodotGeneratorUtils::bind_method(
      const String& class_name, const String& method_name, const Vector<String>& parameter_names) {
    // clang-format off
    Ref<Arguments> arguments;

    Ref<Function> result =  build<Function>().with_children({
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



  Ref<Function> GodotGeneratorUtils::bind_static_method(
      const String& class_name, const String& method_name, const Vector<String>& parameter_names) {
    // clang-format off
    Ref<Arguments> arguments = node_new<Arguments>();

    Ref<Function> result =  build<Function>().with_children({
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

  Ref<Body> GodotGeneratorUtils::get_or_create_bind_methods_body(
      Ref<Class> target_class, Ref<Context> generated_body, Ref<Context> generated_sources) {
    Ref<Function> bind_methods = generated_sources->find_child(
        0, NamedContextPredicates::name<Function>((target_class->name() + "::_bind_methods").c_str()));
    Ref<Body> bind_methods_body;

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

  bool GodotGeneratorUtils::class_has_base_class(Ref<Class> target_class, const String& base_class_qualified) {
    if (target_class->qualified_name() == base_class_qualified) {
      return true;
    }

    // TODO: this will not work if the base class name is not fully qualified
    for ( const String& base : target_class->direct_bases_names()) {
      Ref<Class> base_class = TypeDB::instance()->get_type_data<Class>(base);
      if (!base_class) {
        print_err("Base class not found!");
        return false;
      }

      return class_has_base_class(base_class, base_class_qualified);
    }

    return false;
  }

  bool GodotGeneratorUtils::class_is_node_type(Ref<Class> target_class) {
    return class_has_base_class(target_class, "Node");
  }

  bool GodotGeneratorUtils::class_is_resource_type(Ref<Class> target_class) {
    return class_has_base_class(target_class, "Resource");

  }

  bool GodotGeneratorUtils::class_is_ref_counted_type(Ref<Class> target_class) {
    return class_has_base_class(target_class, "RefCounted");

  }

  bool GodotGeneratorUtils::class_is_object_type(Ref<Class> target_class) {
    return class_has_base_class(target_class, "Object");
  }

  bool GodotGeneratorUtils::class_is_variant_type(Ref<Class> target_class) {
    // TODO
    return true;
  }

}  // namespace GodotObjectCompiler