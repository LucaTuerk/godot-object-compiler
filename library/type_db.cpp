#include "library/type_db.h"

#include "core/config.h"
#include "core/helpers.h"
#include "core/string_writer.h"
#include "tree/syntax/class.h"
#include "tree/syntax/define.h"
#include "tree/syntax/enum.h"
#include "tree/syntax/include.h"
#include "tree/syntax/node.h"
#include "tree/syntax/type.h"

namespace GodotObjectCompiler {

  void dump_node(IStructuredWriter* writer, Ref<Node> node, bool is_root) {
    writer->write_to_section(node->get_id());
    node->write_to(writer);

    if (is_root) {
      writer->write<String, UID>("_parent", INVALID_ID);
    }

    if (Ref<Context> context = node->as<Context>(); context && !context->is<Include>()) {
      for (Ref<Node> child : context->get_children()) {
        dump_node(writer, child, false);
      }
    }
  }

  bool ConfigNodeReaderWriter::write_to_file(Ref<Node> node, const String& path) {
    Config config;
    dump_node(&config, node, true);
    return config.write_to_file(path);
  }

  Ref<Node> ConfigNodeReaderWriter::read_from_file(const String& path) {
    Config config;
    Vector<Ref<Node>> local_store;

    if (!config.read_from_file(path)) {
      return nullptr;
    }

    for (const String& section : config.get_sections()) {
      config.read_from_section(section);
      if (!config.has_config_value("_class")) {
        continue;
      }

      String node_class = config.read<String, String>("_class");
      UID uid = config.read<String, UID>("_id");
      if (ExecutionContext::instance()->get_node_db()->get<Node>(uid) != nullptr) {
        continue;
      }

      Ref<Node> node = NodeDB::create(node_class);
      if (node) {
        node->read_from(&config);
        local_store.push_back(node);
      }
    }

    for (const String& section : config.get_sections()) {
      config.read_from_section(section);
      if (!config.has_config_value("_id") || !config.has_config_value("_parent")) {
        continue;
      }

      UID uid = config.read<String, UID>("_id");
      UID parent_uid = config.read<String, UID>("_parent");

      Ref<Node> self = ExecutionContext::instance()->get_node_db()->get<Node>(uid);
      Ref<Context> parent = ExecutionContext::instance()->get_node_db()->get<Context>(parent_uid);

      if (parent && self) {
        parent->add_child(self);
      }
    }

    Ref<Node> root;
    for (const Ref<Node>& node : local_store) {
      if (node->get_parent() == nullptr) {
        if (root != nullptr) {
          print_err("Multiple root nodes found in read config file. Invalid.");
          return nullptr;
        }
        root = node;
      }
    }

    return root;
  }

  TypeDB* TypeDB::instance() {
    static TypeDB instance;
    return &instance;
  }

  void TypeDB::set_cache_directory(const String& path) { _cache_directory = path; }

  String TypeDB::_get_cache_file_path(const String& qualified_name) const {
    return path_concat(_cache_directory, string_replace(qualified_name, "::", "/") + ".gocdb");
  }

  void TypeDB::save_type_data(Ref<NamedContext> root) {
    Writer writer;
    String qualified_name = root->mangled_name();

    auto path = _get_cache_file_path(qualified_name);
    auto base = path_base(path);
    if (!directory_exits(base) && !create_dir_recursive(base)) {
      return;
    }

    writer.write_to_file(root, path);
  }

  Ref<Node> TypeDB::get_type_data(
      const String& qualified_name, Size template_argument_count, const Ref<Namespace>& from_namespace) {
    Reader reader;

    for (const String& name : resolve_possible_namespaces(qualified_name, from_namespace)) {
      String mangled_name = mangle_name(name, template_argument_count);

      if (auto itr = _cache.find(mangled_name); itr != _cache.end()) {
        return itr->second;
      }
      const String& cache_file_path = _get_cache_file_path(mangled_name);
      const String& godot_cache_file_path = _get_cache_file_path("godot::" + mangled_name);

      if (file_exists(cache_file_path)) {
        Ref<Node> root = reader.read_from_file(cache_file_path);
        _cache[mangled_name] = root;
        return root;
      } else if (file_exists(godot_cache_file_path)) {
        Ref<Node> root = reader.read_from_file(godot_cache_file_path);
        _cache[mangled_name] = root;
        return root;
      }
    }

    return nullptr;
  }

  Ref<Node> TypeDB::get_type_data(const Ref<Type>& type, const Ref<Namespace>& from_namespace) {
    return get_type_data(type->name(), type->template_argument_count());
  }

  AssumptionState TypeDB::validate_assumption(Assumption<AssumeType<Enum>>& p_assumption) {
    return validate_t<Enum>(p_assumption);
  }

  AssumptionState TypeDB::validate_assumption(Assumption<AssumeType<Class>>& p_assumption) {
    return validate_t<Class>(p_assumption);
  }

  AssumptionState TypeDB::validate_assumption(Assumption<AssumeType<Define>>& p_assumption) {
    return validate_t<Define>(p_assumption);
  }

  String TypeDB::mangle_name(const String& qualified_name, Size template_parameter_count) {
    if (template_parameter_count == 0) {
      return qualified_name;
    }
    return format("%s_T_ARGS_%d_", qualified_name.c_str(), template_parameter_count);
  }

  Vector<String> TypeDB::resolve_possible_namespaces(
      const String& qualified_name, const Ref<Namespace>& from_namespace) {
    Vector<String> result;

    if (from_namespace == nullptr) {
      return {qualified_name};
    }

    const Vector<String> namespaces_names = from_namespace->namespaces_names();
    for (Size current_size = 0; current_size <= namespaces_names.size(); current_size++) {
      StreamWriter writer;
      for (Size i = 0; i < current_size; ++i) {
        writer.write(namespaces_names[i]);
        writer.write("::");
      }
      writer.write(qualified_name);
      result.push_back(writer.get_string());
    }
    return result;
  }

}  // namespace GodotObjectCompiler
