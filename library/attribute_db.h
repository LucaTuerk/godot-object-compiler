
#pragma once
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

    bool register_attribute_params(
        const String& class_name, const String& override_name, std::initializer_list<AttributeParameterType>&& args);

    bool is_known_macro(const String& macro);

    Ref<Attribute> create_for_macro(const String& macro);

    Vector<String> get_all_macros();

    Dictionary<String, Vector<AttributeParameterType>> const* get_parameter_types(const String& macro);

    bool register_parameter(const String& class_name, IAttributeParameters* parameter);

    Vector<IAttributeParameters*> get_parameters(const String& macro);

    bool register_class_generator(const String& generator_name, Ref<ClassGenerator> generator);

    const Vector<Ref<ClassGenerator>>& class_generators() const;

   private:

    HashSet<String> _registered_generator_names;
    Vector<Ref<ClassGenerator>> _class_generators;

    Dictionary<String, HashSet<String>> _registered_parameters;
    Dictionary<String, Vector<IAttributeParameters*>> _parameters;

    Dictionary<String, CreationFunc> _creation_funcs;
    Dictionary<String, String> _macro_aliases;
    Dictionary<String, Dictionary<String, Vector<AttributeParameterType>>> _parameter_type;
  };

}  // namespace GodotObjectCompiler

#define ATTRIBUTE_REGISTER_DEFAULT_MACRO(macro)                                            \
  static Ref<Attribute> attribute_create_static() { return create_static()->as<Attribute>(); } \
  static inline bool attribute_registered =                                                \
      AttributeDB::instance()->register_attribute(get_type_static(), #macro, &attribute_create_static);

#define ATTRIBUTE_REGISTER_PARAMETERS(name, ...)  \
  static inline bool name##_override_registered = \
      AttributeDB::instance()->register_attribute_params(get_type_static(), #name, {__VA_ARGS__});

#define ATTRIBUTE_PARAMS(type)                 \
  static inline bool type##_param_registered = \
      AttributeDB::instance()->register_parameter(get_type_static(), new type());
