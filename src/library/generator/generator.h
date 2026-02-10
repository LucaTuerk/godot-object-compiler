/**************************************************************************/
/* generator.h                                                            */
/*                        ___  ___  ___   ___ _____                       */
/*                       / __|/ _ \|   \ / _ \_   _|                      */
/*                      | (_ | (_) | |) | (_) || |                        */
/*                       \___|\___/|___/ \___/ |_|                        */
/*   ___  ___    _ ___ ___ _____    ___ ___  __  __ ___ ___ _    ___ ___  */
/*  / _ \| _ )_ | | __/ __|_   _|  / __/ _ \|  \/  | _ \_ _| |  | __| _ \ */
/* | (_) | _ \ || | _| (__  | |   | (_| (_) | |\/| |  _/| || |__| _||   / */
/*  \___/|___/\__/|___\___| |_|    \___\___/|_|  |_|_| |___|____|___|_|_\ */
/*                                                                        */
/*              This file is part of Godot Object Compiler                */
/*                  Copyright (c) 2026 Luca Ian Tuerk                     */
/**************************************************************************/
/*                            MIT LICENCE                                 */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/
#pragma once

#include "library/tree/syntax/attributes.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/parser_error.h"
#include "library_godot/attributes/godot_attributes.h"

namespace GodotObjectCompiler {

  class IGenerator {
   public:

    virtual ~IGenerator() = default;

    virtual Ref<Context> generate(Ref<Context> p_tree, Ref<Node> p_entry_point) = 0;
  };

  class ClassGenerator {
   public:

    virtual ~ClassGenerator() = default;

    bool handles(const Ref<Class>& p_target_class, const Ref<Attribute>& p_attribute);

    static void merge_default_attribute_arguments(
        const Ref<Attribute>& p_target_attribute, const Ref<Context>& p_default_values);

    Ref<GeneratorError> generate_default_attribute_arguments(
        const Ref<Class>& p_target_class, const Ref<Attribute>& p_attribute, const Ref<Context>& r_default_values);

    Ref<GeneratorError> generate(const Ref<Class>& p_target_class, const Ref<Attribute>& p_attribute,
        const Ref<Context>& r_generated_body, const Ref<Context>& r_generated_sources,
        const Ref<Context>& r_generated_global);

   protected:

    virtual bool _handles(Ref<Class> p_target_class, Ref<Attribute> p_attribute) = 0;

    virtual Ref<GeneratorError> _generate_default_attribute_arguments(
        Ref<Class> p_target_class, Ref<Attribute> p_attribute, Ref<Context> r_default_values) = 0;

    virtual Ref<GeneratorError> _generate(Ref<Class> p_target_class, Ref<Attribute> p_attribute,
        Ref<Context> r_generated_body, Ref<Context> r_generated_sources, Ref<Context> r_generated_global) = 0;
  };

  inline bool ClassGenerator::handles(const Ref<Class>& p_target_class, const Ref<Attribute>& p_attribute) {
    return _handles(p_target_class, p_attribute);
  }

  inline void ClassGenerator::merge_default_attribute_arguments(
      const Ref<Attribute>& p_target_attribute, const Ref<Context>& p_default_values) {
    HashSet<String> had_value_set;

    Ref<Arguments> arguments = p_target_attribute->find_child<Arguments>();
    if (!arguments) {
      arguments = p_target_attribute->build_child<Arguments>();
    }

    for (const Ref<Node>& child : arguments->get_children()) {
      had_value_set.insert(child->get_type());
    }

    for (const Ref<Node>& child : *p_default_values) {
      if (had_value_set.find(child->get_type()) == had_value_set.end()) {
        arguments->add_child(child->clone());
      }
    }
  }

  inline Ref<GeneratorError> ClassGenerator::generate_default_attribute_arguments(
      const Ref<Class>& p_target_class, const Ref<Attribute>& p_attribute, const Ref<Context>& r_default_values) {
    return _generate_default_attribute_arguments(p_target_class, p_attribute, r_default_values);
  }

  inline Ref<GeneratorError> ClassGenerator::generate(const Ref<Class>& p_target_class,
      const Ref<Attribute>& p_attribute, const Ref<Context>& r_generated_body, const Ref<Context>& r_generated_sources,
      const Ref<Context>& r_generated_global) {
    return _generate(p_target_class, p_attribute, r_generated_body, r_generated_sources, r_generated_global);
  }

  template <typename AttrT>
  class IClassGenerator : public ClassGenerator {
   protected:

    bool _handles(Ref<Class> p_target_class, Ref<Attribute> p_attribute) override;
    Ref<GeneratorError> _generate_default_attribute_arguments(
        Ref<Class> p_target_class, Ref<Attribute> p_attribute, Ref<Context> r_default_values) override;

    Ref<GeneratorError> _generate(Ref<Class> p_target_class, Ref<Attribute> p_attribute, Ref<Context> r_generated_body,
        Ref<Context> r_generated_sources, Ref<Context> r_generated_global) override;

   public:

    ~IClassGenerator() override = default;

    virtual Ref<GeneratorError> do_generate_default_attribute_arguments(
        Ref<Class> p_target_class, Ref<AttrT> p_attribute, Ref<Context> p_default_values);

    virtual Ref<GeneratorError> do_generate(Ref<Class> p_target_class, Ref<AttrT> p_attribute,
        Ref<Context> p_generated_body, Ref<Context> p_generated_sources, Ref<Context> p_generated_global) = 0;
  };

  template <typename AttrT>
  bool IClassGenerator<AttrT>::_handles(Ref<Class> p_target_class, Ref<Attribute> p_attribute) {
    UNUSED(p_target_class);
    return p_attribute->template is<AttrT>();
  }

  template <typename AttrT>
  Ref<GeneratorError> IClassGenerator<AttrT>::_generate_default_attribute_arguments(
      Ref<Class> p_target_class, Ref<Attribute> p_attribute, Ref<Context> r_default_values) {
    return do_generate_default_attribute_arguments(p_target_class, p_attribute->as<AttrT>(), r_default_values);
  }

  template <typename AttrT>
  Ref<GeneratorError> IClassGenerator<AttrT>::_generate(Ref<Class> p_target_class, Ref<Attribute> p_attribute,
      Ref<Context> r_generated_body, Ref<Context> r_generated_sources, Ref<Context> r_generated_global) {
    return do_generate(
        p_target_class, p_attribute->template as<AttrT>(), r_generated_body, r_generated_sources, r_generated_global);
  }

  template <typename AttrT>
  Ref<GeneratorError> IClassGenerator<AttrT>::do_generate_default_attribute_arguments(
      Ref<Class> p_target_class, Ref<AttrT> p_attribute, Ref<Context> p_default_values) {
    UNUSED(p_target_class);
    UNUSED(p_attribute);
    UNUSED(p_default_values);
    return GeneratorError::OK;
  }

}

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
