#pragma once

#include "library/tree/output/generator_error.h"
#include "library/tree/syntax/attributes.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/parser_error.h"
#include "library_godot/attributes/godot_attributes.h"

namespace GodotObjectCompiler {

  class IGenerator {
   public:

    virtual ~IGenerator() = default;

    virtual Ref<Context> generate(Ref<Context> tree, Ref<Node> entry_point) = 0;
  };

  class ClassGenerator {
   public:

    virtual ~ClassGenerator() = default;

    bool handles(const Ref<Class>& target_class, const Ref<Attribute>& attribute);
    static void merge_default_attribute_arguments(
        const Ref<Attribute>& target_attribute, const Ref<Context>& default_values);

    Ref<GeneratorError> generate_default_attribute_arguments(
        const Ref<Class>& target_class, const Ref<Attribute>& attribute, const Ref<Context>& default_values);

    Ref<GeneratorError> generate(const Ref<Class>& target_class, const Ref<Attribute>& attribute,
        const Ref<Context>& generated_body, const Ref<Context>& generated_sources,
        const Ref<Context>& generated_global);

   protected:

    virtual bool _handles(Ref<Class> target_class, Ref<Attribute> attribute) = 0;
    virtual Ref<GeneratorError> _generate_default_attribute_arguments(
        Ref<Class> target_class, Ref<Attribute> attribute, Ref<Context> default_values) = 0;
    virtual Ref<GeneratorError> _generate(Ref<Class> target_class, Ref<Attribute> attribute,
        Ref<Context> generated_body, Ref<Context> generated_sources, Ref<Context> generated_global) = 0;
  };

  inline bool ClassGenerator::handles(const Ref<Class>& target_class, const Ref<Attribute>& attribute) {
    return _handles(target_class, attribute);
  }

  inline void ClassGenerator::merge_default_attribute_arguments(
      const Ref<Attribute>& target_attribute, const Ref<Context>& default_values) {
    HashSet<String> had_value_set;

    Ref<Arguments> arguments = target_attribute->find_child<Arguments>();
    if (!arguments) {
      arguments = target_attribute->build_child<Arguments>();
    }

    for (const Ref<Node>& child : arguments->get_children()) {
      had_value_set.insert(child->get_type());
    }

    for (const Ref<Node>& child : *default_values) {
      if (had_value_set.find(child->get_type()) == had_value_set.end()) {
        arguments->add_child(child->clone());
      }
    }
  }

  inline Ref<GeneratorError> ClassGenerator::generate_default_attribute_arguments(
      const Ref<Class>& target_class, const Ref<Attribute>& attribute, const Ref<Context>& default_values) {
    return _generate_default_attribute_arguments(target_class, attribute, default_values);
  }

  inline Ref<GeneratorError> ClassGenerator::generate(const Ref<Class>& target_class, const Ref<Attribute>& attribute,
      const Ref<Context>& generated_body, const Ref<Context>& generated_sources, const Ref<Context>& generated_global) {
    return _generate(target_class, attribute, generated_body, generated_sources, generated_global);
  }

  template <typename AttrT>
  class IClassGenerator : public ClassGenerator {
   protected:

    bool _handles(Ref<Class> target_class, Ref<Attribute> attribute) override;
    Ref<GeneratorError> _generate_default_attribute_arguments(
        Ref<Class> target_class, Ref<Attribute> attribute, Ref<Context> default_values) override;

    Ref<GeneratorError> _generate(Ref<Class> target_class, Ref<Attribute> attribute, Ref<Context> generated_body,
        Ref<Context> generated_sources, Ref<Context> generated_global) override;

   public:

    ~IClassGenerator() override = default;
    virtual Ref<GeneratorError> do_generate_default_attribute_arguments(
        Ref<Class> target_class, Ref<AttrT> attribute, Ref<Context> default_values);
    virtual Ref<GeneratorError> do_generate(Ref<Class> target_class, Ref<AttrT> attribute, Ref<Context> generated_body,
        Ref<Context> generated_sources, Ref<Context> generated_global) = 0;
  };

  template <typename AttrT>
  bool IClassGenerator<AttrT>::_handles(Ref<Class> target_class, Ref<Attribute> attribute) {
    return attribute->template is<AttrT>();
  }

  template <typename AttrT>
  Ref<GeneratorError> IClassGenerator<AttrT>::_generate_default_attribute_arguments(
      Ref<Class> target_class, Ref<Attribute> attribute, std::shared_ptr<Context> default_values) {
    return do_generate_default_attribute_arguments(target_class, attribute->as<AttrT>(), default_values);
  }

  template <typename AttrT>
  Ref<GeneratorError> IClassGenerator<AttrT>::_generate(Ref<Class> target_class, Ref<Attribute> attribute,
      Ref<Context> generated_body, Ref<Context> generated_sources, Ref<Context> generated_global) {
    return do_generate(
        target_class, attribute->template as<AttrT>(), generated_body, generated_sources, generated_global);
  }

  template <typename AttrT>
  Ref<GeneratorError> IClassGenerator<AttrT>::do_generate_default_attribute_arguments(
      Ref<Class> target_class, Ref<AttrT> attribute, Ref<Context> default_values) {
    return GeneratorError::OK;
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

#define ERROR_NODE_LEVEL(error_type, error_level, ...)     \
  if constexpr (error_level == ErrorLevel::ERROR) {        \
    return node_new<error_type>(error_level, __VA_ARGS__); \
  } else {                                                 \
    node_new<error_type>(error_level, __VA_ARGS__);        \
  }

#define GEN_ERROR(target, message) \
  ERROR_NODE_LEVEL(GeneratorError, ErrorLevel::ERROR, get_type_static(), message, target)

#define GEN_ERROR_COND(condition, target, message)                                          \
  if ((condition)) {                                                                        \
    ERROR_NODE_LEVEL(GeneratorError, ErrorLevel::ERROR, get_type_static(), message, target) \
  }

#define GEN_WARNING(target, message) \
  ERROR_NODE_LEVEL(GeneratorError, ErrorLevel::WARNING, get_type_static(), message, target)

#define GEN_WARNING_COND(condition, target, message)                                          \
  if ((condition)) {                                                                          \
    ERROR_NODE_LEVEL(GeneratorError, ErrorLevel::WARNING, get_type_static(), message, target) \
  }
