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

    Ref<Node> get_type_data(const String& qualified_name, Size template_parameter_count = 0);
    Ref<Node> get_type_data(const Ref<Type>& type);

    template <typename T>
    Ref<T> get_type_data(const String& qualified_name, Size template_parameter_count = 0);

    template <typename T>
    Ref<T> get_type_data(const Ref<Type>& type);

    template <typename T>
    AssumptionState validate_t(Assumption<AssumeType<T>>& type_assumption);

    AssumptionState validate_assumption(Assumption<AssumeType<Enum>>& assumption) override;
    AssumptionState validate_assumption(Assumption<AssumeType<Class>>& assumption) override;
    AssumptionState validate_assumption(Assumption<AssumeType<Define>>& assumption) override;

    static String mangle_name(const String& qualified_name, Size template_parameter_count);

   private:

    using Reader = ConfigNodeReaderWriter;
    using Writer = ConfigNodeReaderWriter;

    [[nodiscard]] String _get_cache_file_path(const String& qualified_name) const;
    Dictionary<String, Ref<Node>> _cache;
    String _cache_directory;
  };

  template <typename T>
  Ref<T> TypeDB::get_type_data(const String& qualified_name, Size template_parameter_count) {
    Ref<Node> result = get_type_data(qualified_name, template_parameter_count);
    if (!result) {
      return nullptr;
    }

    return result->as<T>();
  }

  template <typename T>
  Ref<T> TypeDB::get_type_data(const Ref<Type>& type) {
    Ref<Node> result = get_type_data(type);
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

}  // namespace GodotObjectCompiler
