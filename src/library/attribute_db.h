/**************************************************************************/
/* attribute_db.h                                                         */
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
#include "generator/attribute_parameter_type.h"

namespace GodotObjectCompiler {

class Attribute;
class ClassGenerator;

enum AttributeParameterType {
  STRING,
  INT,
  ENUM,
  MEMBER_FUNCTION,
};

class IAttributeParameters {
 public:
  virtual ~IAttributeParameters() = default;

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
 private:
  struct Private {};

 public:
  using CreationFunc = Ref<Attribute> (*)();

  bool register_attribute(const String& p_class_name, const String& p_macro,
                          CreationFunc p_creator);

  bool register_attribute_parameter(
      const String& p_class_name,
      const Ref<IAttributeParameterType>& p_parameter);

  bool is_known_macro(const String& p_macro);

  Result<Attribute> create_for_macro(const String& p_macro);

  Vector<Ref<IAttributeParameterType>> get_parameters_for_macro(
      const String& p_macro);

  template <typename AttributeT, typename ParamT>
  Ref<ParamT> get_parameter_type();

  String get_macro_for_attribute(const String& p_class_name);

  Vector<String> get_all_macros();

  bool register_class_generator(const String& p_generator_name,
                                Ref<ClassGenerator> p_generator);

  [[nodiscard]] const Vector<Ref<ClassGenerator>>& class_generators() const;

  AttributeDB(Private) {}
  AttributeDB() = delete;

 private:
  HashSet<String> _registered_generator_names;
  Vector<Ref<ClassGenerator>> _class_generators;

  Dictionary<String, HashSet<String>> _registered_parameter_types;
  Dictionary<String, Vector<Ref<IAttributeParameterType>>> _parameters;

  Dictionary<String, CreationFunc> _creation_funcs;
  Dictionary<String, String> _macro_aliases;

  friend ExecutionContext;
};

template <typename AttributeT, typename ParamT>
Ref<ParamT> AttributeDB::get_parameter_type() {
  const String attribute_type_name = AttributeT::get_type_static();

  const auto itr = _parameters.find(attribute_type_name);
  if (itr == _parameters.end()) {
    PANIC("Unknown attribute type %s", attribute_type_name.c_str());
  }

  const Vector<Ref<IAttributeParameterType>>& params = itr->second;

  auto params_itr = std::find_if(params.begin(), params.end(), [](auto val) {
    return val->get_type() == ParamT::get_type_static();
  });
  if (params_itr == params.end()) {
    PANIC("Unknown parameter type %s for attribute type %s",
          ParamT::get_type_static().c_str(), attribute_type_name.c_str());
  }

  Ref<ParamT> param = std::dynamic_pointer_cast<ParamT>(*params_itr);
  if (!param) {
    PANIC("Failed to convert parameter to requested type %s",
          ParamT::get_type_static().c_str());
  }

  return param;
}

}  // namespace GodotObjectCompiler

#define ATTRIBUTE_DEFAULT_MACRO(macro)                                      \
  static Ref<Attribute> attribute_create_static() {                         \
    return create_static()->as<Attribute>();                                \
  }                                                                         \
  static inline bool attribute_registered =                                 \
      ExecutionContext::instance()->get_attribute_db()->register_attribute( \
          get_type_static(), #macro, &attribute_create_static);

#define ATTRIBUTE_REGISTER_PARAMETERS(type)                 \
  static inline bool type##_parameter_registered =          \
      ExecutionContext::instance()                          \
          ->get_attribute_db()                              \
          ->register_attribute_parameter(get_type_static(), \
                                         type##ParameterType::instance());
