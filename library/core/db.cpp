#include "library/core/db.h"

#include "../tree/syntax/include.h"
#include "../tree/syntax/namespace.h"
#include "../tree/syntax/node.h"
#include "config.h"
#include "helpers.h"

namespace GodotObjectCompiler {

  void dump_node(IStructuredWriter* writer, Node* node) {
    writer->write_to_section(node->get_id());
    node->write_to(writer);

    if (Context* context = dynamic_cast<Context*>(node); context && !context->is<Include>()) {
      for (Node* child : context->get_children()) {
        dump_node(writer, child);
      }
    }
  }

  DB DB::init(Namespace* root) {
    DB result;
    result._root = root ? root : new Namespace();
    return result;
  }

  void DB::write_to_config(const String& path) const {
    auto _path = path;

    Config config;
    dump_node(&config, _root);

    config.write_to_section(_root->get_id());
    config.write("_parent", INVALID_ID);

    config.write_to_file(_path);
  }

  DB DB::read_from_config(const String& path) {
    auto _path = path;
    Config config;
    config.read_from_file(_path);
    DB db = DB();
    db._root = nullptr;

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

      Node* node = NodeDB::create(node_class);
      if (node) {
        node->read_from(&config);
      }
    }

    for (const String& section : config.get_sections()) {
      config.read_from_section(section);
      if (!config.has_config_value("_id") || !config.has_config_value("_parent")) {
        continue;
      }

      UID uid = config.read<String, UID>("_id");
      UID parent_uid = config.read<String, UID>("_parent");

      Node* self = ExecutionContext::instance()->get_node_db()->get<Node>(uid);
      Context* parent = ExecutionContext::instance()->get_node_db()->get<Context>(parent_uid);

      if (db._root == nullptr && parent_uid == INVALID_ID) {
        db._root = self->as<Namespace>();
      } else if (parent_uid == INVALID_ID) {
        print_ln("Failed to find parent of type");
      }

      if (parent && self) {
        parent->add_child(self);
      }
    }

    return db;
  }

  Namespace* DB::get_root() const { return _root; }

  TypeDB* TypeDB::instance() {
    static TypeDB instance;
    return &instance;
  }

  void TypeDB::set_cache_directory(const String& path) { _cache_directory = path; }

  String TypeDB::_get_cache_file_path(const String& qualified_name) const {
    return path_concat(_cache_directory, string_replace(qualified_name, "::", "/") + ".gocdb");
  }

  void TypeDB::save_type_data(Namespace* root) {
    String qualified_name = root->qualified_name();
    DB db = DB::init(root);
    auto path = _get_cache_file_path(qualified_name);
    auto base = path_base(path);
    if (!dir_exists(base) && !create_dir_recursive(base)) {
      return;
    }
    db.write_to_config(path);
  }

  Node* TypeDB::get_type_data(const String& qualified_name) {
    if (auto itr = _cache.find(qualified_name); itr != _cache.end()) {
      return itr->second;
    }

    const String& cache_file_path = _get_cache_file_path(qualified_name);

    if (file_exists(cache_file_path)) {
      DB db = DB::read_from_config(cache_file_path);
      Node* root = db.get_root();
      _cache[qualified_name] = db.get_root();
      return root;
    }

    return nullptr;
  }

}  // namespace GodotObjectCompiler
