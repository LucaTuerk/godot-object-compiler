
#include "generated_class_generator.h"
#include "library/tree/output/generator_error.h"
#include "library/tree/output/output.h"
#include "library/tree/output/output_file.h"
#include "library/tree/syntax/attributes_godot.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/namespace.h"

namespace GodotObjectCompiler {

  Context* GodotGeneratedClassGenerator::generate(Context* tree, Node* entry_point) {
    Class* _class = entry_point->as<Class>();

    GEN_ERROR_COND(!_class, "Entry Point is not a Class. Abort!")
    GEN_ERROR_COND(!_class->has_generated_class_attribute(),
        "The generation target class does not have a GodotGeneratedClass attribute. Abort!");

    Context* result = node_new<Context>();
    Context* generated_body = result->create_child<OutputFile>();
    Context* generated_header = result->create_child<OutputFile>();

    for (Namespace* _namespace : _class->namespaces()) {
      Writer::NamespaceOpen(generated_header, _namespace->name());
    }

    for (Attribute* attribute : _class->attributes()) {
      if (GodotPropertyAttribute* property_attribute = attribute->as<GodotPropertyAttribute>()) {
        GEN_ADD_CHILD_ERROR_TO_RESULT(
            generate_property_bindings(_class, property_attribute, generated_body, generated_header))
      }
    }

    return result;
  }

  GeneratorError* GodotGeneratedClassGenerator::generate_property_bindings(
      Class* _class, GodotPropertyAttribute* property, Context* generated_body, Context* generated_header) {
    Node* target = property->resolve_target();

    if (Field* target_field = target->as<Field>()) {
      String property_name = target_field->name();
      Type* field_type = target_field->type();

      GEN_ERROR_COND(target_field->is_const(), "Target field is a Constant. Abort!");
      GEN_ERROR_COND(target_field->is_static(), "Target field is a static Field. Abort!");
      GEN_ERROR_COND(!field_type, "Field does not name a type. Abort!");

      String type_name = field_type->type_name();
      String getter_name = "get_" + property_name;
      String setter_name = "set_" + property_name;

      generated_body->add_child(Writer::MemberFuncDef(type_name, getter_name, {}, "const"));
      generated_body->add_child(Writer::MemberFuncDef("void", setter_name,
          {
              Writer::ConstRefParam(type_name, "p_" + property_name),
          },
          "const"));
      generated_header->add_child(
          Writer::MemberFuncImpl(type_name, _class->name(), getter_name, {}, "const", {Writer::Return(property_name)}));
      generated_header->add_child(Writer::MemberFuncImpl("void", _class->name(), setter_name,
          {Writer::ConstRefParam(type_name, "p_" + property_name)}, "",
          {Writer::Assign(property_name, Writer::Text("p_" + property_name))}));
    }

    return GeneratorError::OK;
  }

}  // namespace GodotObjectCompiler