#pragma once

#include "library/tree/output/generator_error.h"
#include "library/tree/syntax/attributes.h"
#include "library/tree/syntax/class.h"

namespace GodotObjectCompiler {

  class IGenerator {
   public:

    virtual ~IGenerator() = default;

    virtual Ref<Context> generate(Ref<Context> tree, Ref<Node> entry_point) = 0;
  };

  class ClassGenerator {
   public:

    virtual ~ClassGenerator() = default;

    bool handles(Ref<Class> target_class, Ref<Attribute> attribute);
    Ref<GeneratorError> generate(Ref<Class> target_class, Ref<Attribute> attribute, Ref<Context> generated_body,
        Ref<Context> generated_sources, Ref<Context> generated_global);

   protected:

    virtual bool _handles(Ref<Class> target_class, Ref<Attribute> attribute) = 0;
    virtual Ref<GeneratorError> _generate(Ref<Class> target_class, Ref<Attribute> attribute,
        Ref<Context> generated_body, Ref<Context> generated_sources, Ref<Context> generated_global) = 0;
  };

  inline bool ClassGenerator::handles(Ref<Class> target_class, Ref<Attribute> attribute) {
    return _handles(target_class, attribute);
  }

  inline Ref<GeneratorError> ClassGenerator::generate(Ref<Class> target_class, Ref<Attribute> attribute,
      Ref<Context> generated_body, Ref<Context> generated_sources, Ref<Context> generated_global) {
    return _generate(target_class, attribute, generated_body, generated_sources, generated_global);
  }

  template <typename AttrT>
  class IClassGenerator : public ClassGenerator {
   protected:

    bool _handles(Ref<Class> target_class, Ref<Attribute> attribute) override;
    Ref<GeneratorError> _generate(Ref<Class> target_class, Ref<Attribute> attribute, Ref<Context> generated_body,
        Ref<Context> generated_sources, Ref<Context> generated_global) override;

   public:

    ~IClassGenerator() override = default;
    virtual Ref<GeneratorError> do_generate(Ref<Class> target_class, Ref<AttrT> attribute, Ref<Context> generated_body,
        Ref<Context> generated_sources, Ref<Context> generated_global) = 0;
  };

  template <typename AttrT>
  bool IClassGenerator<AttrT>::_handles(Ref<Class> target_class, Ref<Attribute> attribute) {
    return attribute->template is<AttrT>();
  }

  template <typename AttrT>
  Ref<GeneratorError> IClassGenerator<AttrT>::_generate(Ref<Class> target_class, Ref<Attribute> attribute,
      Ref<Context> generated_body, Ref<Context> generated_sources, Ref<Context> generated_global) {
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
      AttributeDB::instance()->register_class_generator(#type, make_ref<type>());

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
