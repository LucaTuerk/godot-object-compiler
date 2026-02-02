#include "library/type_db.h"

#include "core/config.h"
#include "core/helpers.h"
#include "tree/syntax/class.h"
#include "tree/syntax/define.h"
#include "tree/syntax/enum.h"
#include "tree/syntax/include.h"
#include "tree/syntax/node.h"

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
    String qualified_name = root->qualified_name();

    auto path = _get_cache_file_path(qualified_name);
    auto base = path_base(path);
    if (!dir_exists(base) && !create_dir_recursive(base)) {
      return;
    }

    writer.write_to_file(root, path);
  }

  Ref<Node> TypeDB::get_type_data(const String& qualified_name) {
    Reader reader;

    if (auto itr = _cache.find(qualified_name); itr != _cache.end()) {
      return itr->second;
    }

    const String& cache_file_path = _get_cache_file_path(qualified_name);

    if (file_exists(cache_file_path)) {
      Ref<Node> root = reader.read_from_file(cache_file_path);
      _cache[qualified_name] = root;
      return root;
    }

    return nullptr;
  }

  AssumptionState TypeDB::validate_assumption(Assumption<AssumeType<Enum>>& assumption) {
    return validate_t<Enum>(assumption);
  }

  AssumptionState TypeDB::validate_assumption(Assumption<AssumeType<Class>>& assumption) {
    return validate_t<Class>(assumption);
  }

  AssumptionState TypeDB::validate_assumption(Assumption<AssumeType<Define>>& assumption) {
    return validate_t<Define>(assumption);
  }

}  // namespace GodotObjectCompiler
