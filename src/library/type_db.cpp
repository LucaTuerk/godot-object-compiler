/**************************************************************************/
/* type_db.cpp                                                            */
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
#include "library/type_db.h"

#include "core/config.h"
#include "core/file_system_utilities.h"
#include "core/string_utilities.h"
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
    Dictionary<UID, Ref<Node>> local;
    HashSet<UID> was_existing;

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
      if (Ref<Node> existing = ExecutionContext::instance()->get_node_db()->get<Node>(uid); existing != nullptr) {
        local.insert({uid, existing->clone()});
        was_existing.insert(uid);
        continue;
      }

      Ref<Node> node = NodeDB::create(node_class);
      if (node) {
        node->read_from(&config);
        local.insert({uid, node});
      }
    }

    for (const String& section : config.get_sections()) {
      config.read_from_section(section);
      if (!config.has_config_value("_id") || !config.has_config_value("_parent")) {
        continue;
      }

      UID uid = config.read<String, UID>("_id");
      UID parent_uid = config.read<String, UID>("_parent");

      auto self_itr = local.find(uid);
      auto parent_itr = local.find(parent_uid);

      if (was_existing.find(parent_uid) != was_existing.end()) {
        local.erase(self_itr);
        continue;
      }

      if (self_itr == local.end() || parent_itr == local.end()) {
        continue;
      }

      Ref<Node> self = self_itr->second;
      Ref<Context> parent = parent_itr->second->as<Context>();

      if (parent && self) {
        parent->add_child(self);
      }
    }

    Ref<Node> root;
    for (const auto& [uid, node] : local) {
      if (node->get_parent() == nullptr) {
        if (root != nullptr) {
          ERR("Multiple root nodes found in read config file. Invalid.");
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

  String TypeDB::_get_attribute_cache_file_path(const String& p_qualified_name, const String& p_attribute_name) {
    String base = path_concat(_cache_directory, string_replace(p_qualified_name, "::", "/"));
    String path = path_concat(base, format("attr_%s.gocdb", p_attribute_name.c_str()));
    return path;
  }

  void TypeDB::save_type_data(const Ref<NamedContext>& p_type) {
    Writer writer;
    String qualified_name = p_type->mangled_name();

    auto path = _get_cache_file_path(qualified_name);
    auto base = path_base(path);
    if (!directory_exits(base) && !create_dir_recursive(base)) {
      return;
    }

    writer.write_to_file(p_type, path);
  }

  void TypeDB::save_type_attribute(const Ref<NamedContext>& p_type, const Ref<Attribute>& p_attribute) {
    Writer writer;
    String qualified_name = p_type->mangled_name();
    String path = _get_attribute_cache_file_path(qualified_name, p_attribute->get_type());
    String base = path_base(path);
    if (!directory_exits(base) && !create_dir_recursive(base)) {
      return;
    }
    writer.write_to_file(p_attribute, path);
  }

  Ref<Node> TypeDB::get_type_data(
      const String& qualified_name, Size template_argument_count, const Ref<Namespace>& from_namespace) {
    Reader reader;

    for (const String& name : resolve_possible_namespaces(qualified_name, from_namespace)) {
      String mangled_name = mangle_name(name, template_argument_count);
      const String& cache_file_path = _get_cache_file_path(mangled_name);

      if (auto itr = _cache.find(cache_file_path); itr != _cache.end()) {
        return itr->second->clone();
      }

      const String& godot_cache_file_path = _get_cache_file_path("godot::" + mangled_name);

      if (file_exists(cache_file_path)) {
        Ref<Node> root = reader.read_from_file(cache_file_path);
        _cache[cache_file_path] = root->clone();
        return root;
      } else if (file_exists(godot_cache_file_path)) {
        Ref<Node> root = reader.read_from_file(godot_cache_file_path);
        _cache[cache_file_path] = root->clone();
        return root;
      }
    }

    return nullptr;
  }

  Ref<Attribute> TypeDB::get_type_attribute(const String& p_qualified_name, const String& p_attribute_name,
      Size p_template_parameter_count, const Ref<Namespace>& p_from_namespace) {
    Reader reader;

    for (const String& name : resolve_possible_namespaces(p_qualified_name, p_from_namespace)) {
      String mangled_name = mangle_name(name, p_template_parameter_count);
      const String& cache_file_path = _get_attribute_cache_file_path(mangled_name, p_attribute_name);

      if (auto itr = _cache.find(cache_file_path); itr != _cache.end()) {
        return itr->second->as<Attribute>();
      }

      const String& godot_cache_file_path = _get_attribute_cache_file_path("godot::" + mangled_name, p_attribute_name);

      if (file_exists(cache_file_path)) {
        Ref<Node> root = reader.read_from_file(cache_file_path);
        if (!root) {
          return nullptr;
        }
        _cache[cache_file_path] = root->clone();
        return root->as<Attribute>();
      } else if (file_exists(godot_cache_file_path)) {
        Ref<Node> root = reader.read_from_file(godot_cache_file_path);
        if (!root) {
          return nullptr;
        }
        _cache[cache_file_path] = root->clone();
        return root->as<Attribute>();
      }
    }
    return nullptr;
  }

  Ref<Node> TypeDB::get_type_data(const Ref<Type>& type, const Ref<Namespace>& from_namespace) {
    return get_type_data(type->qualified_name(), type->template_argument_count(), from_namespace);
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

}
