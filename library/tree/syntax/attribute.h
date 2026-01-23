#pragma once
#include "class.h"
#include "field.h"
#include "function.h"
#include "library/core/core.h"
#include "node.h"

#define ATTRIBUTE_TYPE(node_type, target_type, target)                                                 \
  NODE_TYPE(node_type)                                                                                 \
 protected:                                                                                            \
  bool _verify_target_class(Node* p_resolved) const override { return p_resolved->is<target_type>(); } \
                                                                                                       \
  Target _get_target() const override { return target; }                                               \
                                                                                                       \
 private:

#define ATTRIBUTE_REGISTER_DEFAULT_MACRO(macro)                                            \
  static Attribute* attribute_create_static() { return create_static()->as<Attribute>(); } \
  static inline bool attribute_registered =                                                \
      AttributeDB::instance()->register_attribute(get_type_static(), #macro, &attribute_create_static);

#define ATTRIBUTE_REGISTER_PARAMETERS(name, ...)  \
  static inline bool name##_override_registered = \
      AttributeDB::instance()->register_attribute_params(get_type_static(), #name, {__VA_ARGS__});

namespace GodotObjectCompiler {

  class Attribute : public NamedContext {
   public:
    enum Target {
      NEXT,        // attribute applies to next sibling in the context
      CONTAINING,  // attribute applies to the containing context
      NONE,
    };

    Node* resolve_target() const;
    bool verify_target(Node* p_resolved) const;

   protected:
    virtual Target _get_target() const = 0;
    virtual bool _verify_target_class(Node* p_resolved) const = 0;
    virtual bool _verify_target(Node* p_resolved) const;
  };

  enum AttributeParameterType {
    STRING,
    INT,
    ENUM,
    MEMBER_FUNCTION,
  };

  class AttributeDB {
   public:
    static AttributeDB* instance() {
      static AttributeDB singleton;
      return &singleton;
    }
    using CreationFunc = Attribute* (*)();

    bool register_attribute(const String& class_name, const String& macro, CreationFunc creator);

    bool register_attribute_params(const String& class_name, const String& override_name,
                                   std::initializer_list<AttributeParameterType>&& args);

    bool is_known_macro(const String& macro);

    Attribute* create_for_macro(const String& macro);

    Vector<String> get_all_macros();

    Dictionary<String, Vector<AttributeParameterType>> const* get_parameter_types(const String& macro);

   private:
    Dictionary<String, CreationFunc> _creation_funcs;
    Dictionary<String, String> _macro_aliases;
    Dictionary<String, Dictionary<String, Vector<AttributeParameterType>>> _parameter_type;
  };

  // This attribute applies to the class it is contained in
  class ClassAttribute : public Attribute {
    ATTRIBUTE_TYPE(ClassAttribute, Class, CONTAINING);
  };

  // This attribute applies to the next sibling function
  class FunctionAttribute : public Attribute {
    ATTRIBUTE_TYPE(FunctionAttribute, Function, NEXT)
  };

  // This attribute applies to the next sibling field
  class FieldAttribute : public Attribute {
    ATTRIBUTE_TYPE(FieldAttribute, Field, NEXT)
  };

  // This attribute does not apply to any specific node in the syntax tree but
  // can provide a hint to the generator.
  class HintAttribute : public Attribute {
    ATTRIBUTE_TYPE(HintAttribute, Attribute, NONE)
  };
}  // namespace GodotObjectCompiler
