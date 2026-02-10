/**************************************************************************/
/* type_db.h                                                              */
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
#include "core/assumption.h"
#include "core/core.h"
#include "library/tree/syntax/context.h"

namespace GodotObjectCompiler {

  class Type;

  class Define;

  class Class;

  class Enum;

  class ConfigNodeReaderWriter : public INodeReader, public INodeWriter {
   public:

    bool write_to_file(Ref<Node> node, const String& path) override;
    Ref<Node> read_from_file(const String& path) override;
  };

  template <typename T>
  struct AssumeType {
    String qualified_name;
    Size template_parameter_count;

    AssumeType() = default;

    AssumeType(const String& type_name, const Size& template_arg_count = 0)
        : qualified_name(type_name), template_parameter_count(template_arg_count) {}
  };

  class TypeDB : public IAssumptionValidator<AssumeType<Enum>>,
                 public IAssumptionValidator<AssumeType<Class>>,
                 public IAssumptionValidator<AssumeType<Define>> {
   public:

    static TypeDB* instance();
    void set_cache_directory(const String& path);
    void save_type_data(Ref<NamedContext> root);

    Ref<Node> get_type_data(const String& qualified_name, Size template_parameter_count = 0,
        const Ref<Namespace>& from_namespace = nullptr);

    Ref<Node> get_type_data(const Ref<Type>& type, const Ref<Namespace>& from_namespace = nullptr);

    template <typename T>
    Ref<T> get_type_data(const String& qualified_name, Size template_parameter_count = 0,
        const Ref<Namespace>& from_namespace = nullptr);

    template <typename T>
    Ref<T> get_type_data(const Ref<Type>& type, const Ref<Namespace>& from_namespace = nullptr);

    template <typename T>
    AssumptionState validate_t(Assumption<AssumeType<T>>& type_assumption);

    AssumptionState validate_assumption(Assumption<AssumeType<Enum>>& p_assumption) override;
    AssumptionState validate_assumption(Assumption<AssumeType<Class>>& p_assumption) override;
    AssumptionState validate_assumption(Assumption<AssumeType<Define>>& p_assumption) override;

    static String mangle_name(const String& qualified_name, Size template_parameter_count);
    static Vector<String> resolve_possible_namespaces(
        const String& qualified_name, const Ref<Namespace>& from_namespace);

   private:

    using Reader = ConfigNodeReaderWriter;
    using Writer = ConfigNodeReaderWriter;

    [[nodiscard]] String _get_cache_file_path(const String& qualified_name) const;
    Dictionary<String, Ref<Node>> _cache;
    String _cache_directory;
  };

  template <typename T>
  Ref<T> TypeDB::get_type_data(
      const String& qualified_name, Size template_parameter_count, const std::shared_ptr<Namespace>& from_namespace) {
    Ref<Node> result = get_type_data(qualified_name, template_parameter_count, from_namespace);
    if (!result) {
      return nullptr;
    }

    return result->as<T>();
  }

  template <typename T>
  Ref<T> TypeDB::get_type_data(const Ref<Type>& type, const std::shared_ptr<Namespace>& from_namespace) {
    Ref<Node> result = get_type_data(type, from_namespace);
    if (!result) {
      return nullptr;
    }
    return result->as<T>();
  }

  template <typename T>
  AssumptionState TypeDB::validate_t(Assumption<AssumeType<T>>& type_assumption) {
    Ref<T> result = get_type_data<T>(type_assumption().qualified_name, type_assumption().template_parameter_count);
    if (!result) {
      return STATE_INVALID;
    }
    return STATE_VALID;
  }

}
