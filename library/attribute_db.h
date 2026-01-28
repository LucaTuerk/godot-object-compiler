
#pragma once
#include "generator/attribute_parameter_type.h"
#include "library/tree/syntax/attribute.h"

namespace GodotObjectCompiler {

  class ClassGenerator;

  enum AttributeParameterType {
    STRING,
    INT,
    ENUM,
    MEMBER_FUNCTION,
  };

  class IAttributeParameters {
   public:

    enum ReturnType {
      FLAG,  // Parameters of this type can be | combined
      SINGULAR,
    };

    enum ParameterType { STRING };

    virtual String return_type_name() = 0;
    virtual Vector<String> names() = 0;
    virtual Vector<ParameterType> parameters() = 0;
  };

  class AttributeDB {
   public:

    static AttributeDB* instance() {
      static AttributeDB singleton;
      return &singleton;
    }

    using CreationFunc = Ref<Attribute> (*)();

    bool register_attribute(const String& class_name, const String& macro, CreationFunc creator);

    bool register_attribute_parameter(const String& class_name, const Ref<IAttributeParameterType>& parameter);

    bool is_known_macro(const String& macro);

    Ref<Attribute> create_for_macro(const String& macro);

    Vector<Ref<IAttributeParameterType>> get_parameters_for_macro(const String& macro);

    Vector<String> get_all_macros();

    bool register_class_generator(const String& generator_name, Ref<ClassGenerator> generator);

    const Vector<Ref<ClassGenerator>>& class_generators() const;

   private:

    HashSet<String> _registered_generator_names;
    Vector<Ref<ClassGenerator>> _class_generators;

    Dictionary<String, HashSet<String>> _registered_parameter_types;
    Dictionary<String, Vector<Ref<IAttributeParameterType>>> _parameters;

    Dictionary<String, CreationFunc> _creation_funcs;
    Dictionary<String, String> _macro_aliases;
  };

}  // namespace GodotObjectCompiler

#define ATTRIBUTE_REGISTER_DEFAULT_MACRO(macro)                                                \
  static Ref<Attribute> attribute_create_static() { return create_static()->as<Attribute>(); } \
  static inline bool attribute_registered =                                                    \
      AttributeDB::instance()->register_attribute(get_type_static(), #macro, &attribute_create_static);

#define ATTRIBUTE_REGISTER_PARAMETERS(type)        \
  static inline bool type##_parameter_registered = \
      AttributeDB::instance()->register_attribute_parameter(get_type_static(), make_ref<type>());
