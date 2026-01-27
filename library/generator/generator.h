#pragma once

#include "library/tree/output/generator_error.h"
#include "library/tree/syntax/attributes.h"
#include "library/tree/syntax/class.h"

namespace GodotObjectCompiler {

  class IGenerator {
   public:

    virtual Context* generate(Context* tree, Node* entry_point) = 0;
  };

  class ClassGenerator {
   public:

    bool handles(Class* target_class, Attribute* attribute);
    GeneratorError* generate(Class* target_class, Attribute* attribute, Context* generated_body,
        Context* generated_sources, Context* generated_global);

   protected:

    virtual bool _handles(Class* target_class, Attribute* attribute) = 0;
    virtual GeneratorError* _generate(Class* target_class, Attribute* attribute, Context* generated_body,
        Context* generated_sources, Context* generated_global) = 0;
  };

  inline bool ClassGenerator::handles(Class* target_class, Attribute* attribute) {
    return _handles(target_class, attribute);
  }

  inline GeneratorError* ClassGenerator::generate(Class* target_class, Attribute* attribute, Context* generated_body,
      Context* generated_sources, Context* generated_global) {
    return _generate(target_class, attribute, generated_body, generated_sources, generated_global);
  }

  template <typename AttrT>
  class IClassGenerator : public ClassGenerator {
   protected:

    bool _handles(Class* target_class, Attribute* attribute) override;
    GeneratorError* _generate(Class* target_class, Attribute* attribute, Context* generated_body,
        Context* generated_sources, Context* generated_global) override;

   public:

    virtual ~IClassGenerator() = default;
    virtual GeneratorError* do_generate(Class* target_class, AttrT* attribute, Context* generated_body,
        Context* generated_sources, Context* generated_global) = 0;
  };

  template <typename AttrT>
  bool IClassGenerator<AttrT>::_handles(Class* target_class, Attribute* attribute) {
    return attribute->template is<AttrT>();
  }

  template <typename AttrT>
  GeneratorError* IClassGenerator<AttrT>::_generate(Class* target_class, Attribute* attribute, Context* generated_body,
      Context* generated_sources, Context* generated_global) {
    return do_generate(
        target_class, attribute->template as<AttrT>(), generated_body, generated_sources, generated_global);
  }

}  // namespace GodotObjectCompiler

#define GENERATOR(type)                             \
 public:                                            \
                                                    \
  static String get_type_static() { return #type; } \
                                                    \
 private:

#define REGISTER_CLASS_GENERATOR(type)                \
  static inline bool _generator_##type##_registered = \
      AttributeDB::instance()->register_class_generator(#type, new type());

#define GEN_ERROR(message) return node_new<GeneratorError>(get_type_static(), message)
#define GEN_ERROR_COND(condition, message) \
  if ((condition)) {                       \
    GEN_ERROR(message);                    \
  }

#define GEN_ADD_CHILD_ERROR_TO_RESULT(func) \
  {                                         \
    auto error = (func);                    \
    if (error != GeneratorError::OK) {      \
      result->add_child(error);             \
    }                                       \
  }

#define GEN_RETURN_ON_CHILD_ERROR(func) \
  {                                     \
    auto error = (func);                \
    if (error != GeneratorError::OK) {  \
      return error;                     \
    }                                   \
  }
