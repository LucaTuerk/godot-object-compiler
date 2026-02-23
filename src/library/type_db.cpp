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
#include "library/core/core.h"
#include "library/execution_context.h"
#include "library/tree/syntax/attribute.h"
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

    if (const Ref<Context> context = node->as<Context>(); context && !context->is<Include>()) {
      for (const Ref<Node>& child : context->get_children()) {
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

      const Ref<Node> self = self_itr->second;

      if (Ref<Context> parent = parent_itr->second->as<Context>(); parent && self) {
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

  void TypeDB::set_cache_directory(const String& path) {
    _cache_directory = path;
    _readonly_cache_directory = path_concat(path, ".readonly");
  }

  String TypeDB::_get_cache_file_path(
      const String& p_qualified_name, CacheType p_cache_type, Size p_template_argument_count) const {
    switch (p_cache_type) {
      case CacheType::READONLY_CACHE: {
        return path_concat(
            _readonly_cache_directory, mangle_name(p_qualified_name, p_template_argument_count) + ".gocdb");
      } break;
      case CacheType::READWRITE_CACHE: {
        return path_concat(_cache_directory, mangle_name(p_qualified_name, p_template_argument_count) + ".gocdb");
      } break;
    }

    PANIC("Unhandled cache type.");
  }

  String TypeDB::_get_attribute_cache_file_path(const String& p_qualified_name, const String& p_attribute_name,
      CacheType p_cache_type, Size p_template_argument_count) const {
    switch (p_cache_type) {
      case CacheType::READONLY_CACHE: {
        const String base =
            path_concat(_readonly_cache_directory, mangle_name(p_qualified_name, p_template_argument_count));
        return path_concat(base, format("attr_%s.gocdb", p_attribute_name.c_str()));
      }
      case CacheType::READWRITE_CACHE: {
        const String base = path_concat(_cache_directory, mangle_name(p_qualified_name, p_template_argument_count));
        return path_concat(base, format("attr_%s.gocdb", p_attribute_name.c_str()));
      } break;
    }
    PANIC("Unhandled cache type.");
  }

  void TypeDB::save_type_data(const Ref<NamedContext>& p_type, const String& p_generated_from) const {
    String path;
    if (Ref<TemplateParameters> parameters = p_type->find_child<TemplateParameters>(); parameters != nullptr) {
      path = _get_cache_file_path(p_type->qualified_name(), CacheType::READWRITE_CACHE, parameters->get_child_count());
    } else {
      path = _get_cache_file_path(p_type->qualified_name(), CacheType::READWRITE_CACHE);
    }

    if (string_contains(path, INVALID_NAME)) {
      PRINT_ERROR("Failed to get cache path for type \"%s\"", p_type->qualified_name().c_str());
      return;
    }

    if (const auto base = path_base(path); !directory_exits(base) && !create_dir_recursive(base)) {
      return;
    }

    if (Writer writer; writer.write_to_file(p_type, path)) {
      ExecutionContext::instance()->register_generated_file(path, p_generated_from);
    }
  }

  void TypeDB::save_type_attribute(
      const Ref<NamedContext>& p_type, const Ref<Attribute>& p_attribute, const String& p_generated_from) const {
    String path;
    if (Ref<TemplateParameters> parameters = p_type->find_child<TemplateParameters>(); parameters != nullptr) {
      path = _get_attribute_cache_file_path(p_type->qualified_name(), p_attribute->get_type(), CacheType::READWRITE_CACHE, parameters->get_child_count());
    } else {
      path = _get_attribute_cache_file_path(p_type->qualified_name(), p_attribute->get_type(), CacheType::READWRITE_CACHE);
    }

    if (string_contains(path, INVALID_NAME)) {
      PRINT_ERROR("Failed to get cache path for attribute \"%s\" on type \"%s\"", p_attribute->get_type().c_str(), p_attribute->qualified_name().c_str());
      return;
    }

    if (const String base = path_base(path); !directory_exits(base) && !create_dir_recursive(base)) {
      return;
    }

    if (Writer writer; writer.write_to_file(p_attribute, path)) {
      ExecutionContext::instance()->register_generated_file(path, p_generated_from);
    }
  }

  Ref<Node> TypeDB::_get_type_data(const String& p_qualified_name, Size p_template_argument_count,
      const Ref<Namespace>& p_from_namespace, CacheType p_cache_type) {
    Reader reader;

    for (const String& name : resolve_possible_namespaces(p_qualified_name, p_from_namespace)) {
      const String& cache_file_path = _get_cache_file_path(name, p_cache_type, p_template_argument_count);

      if (auto itr = _cache.find(cache_file_path); itr != _cache.end()) {
        return itr->second->clone();
      }

      if (file_exists(cache_file_path)) {
        Ref<Node> root = reader.read_from_file(cache_file_path);
        _cache[cache_file_path] = root->clone();
        return root;
      }
      for (const String& using_ : ExecutionContext::instance()->get_usings()) {
        if (String using_path = _get_cache_file_path(
                format("%s::%s", using_.c_str(), name.c_str()), p_cache_type, p_template_argument_count);
            file_exists(using_path)) {
          Ref<Node> root = reader.read_from_file(using_path);
          _cache[cache_file_path] = root->clone();
          return root;
        }
      }
    }

    return nullptr;
  }

  Ref<Attribute> TypeDB::_get_type_attribute(const String& p_qualified_name, const String& p_attribute_name,
      Size p_template_parameter_count, const Ref<Namespace>& p_from_namespace, CacheType cache_type) {
    Reader reader;

    for (const String& name : resolve_possible_namespaces(p_qualified_name, p_from_namespace)) {
      const String& cache_file_path =
          _get_attribute_cache_file_path(name, p_attribute_name, cache_type, p_template_parameter_count);

      if (auto itr = _cache.find(cache_file_path); itr != _cache.end()) {
        return itr->second->as<Attribute>();
      }

      if (file_exists(cache_file_path)) {
        const Ref<Node> root = reader.read_from_file(cache_file_path);
        if (!root) {
          return nullptr;
        }
        _cache[cache_file_path] = root->clone();
        return root->as<Attribute>();
      }
      for (const String& using_ : ExecutionContext::instance()->get_usings()) {
        if (String using_path = _get_attribute_cache_file_path(
                format("%s::%s", using_.c_str(), name.c_str()), p_attribute_name, cache_type);
            file_exists(using_path)) {
          const Ref<Node> root = reader.read_from_file(using_path);
          _cache[cache_file_path] = root->clone();
          return root->as<Attribute>();
        }
      }
    }
    return nullptr;
  }

  Ref<Node> TypeDB::get_type_data(
      const String& qualified_name, Size template_argument_count, const Ref<Namespace>& from_namespace) {
    Ref<Node> found =
        _get_type_data(qualified_name, template_argument_count, from_namespace, CacheType::READWRITE_CACHE);
    if (!found) {
      found = _get_type_data(qualified_name, template_argument_count, from_namespace, CacheType::READONLY_CACHE);
    }
    return found;
  }

  Ref<Attribute> TypeDB::get_type_attribute(const String& p_qualified_name, const String& p_attribute_name,
      Size p_template_parameter_count, const Ref<Namespace>& p_from_namespace) {
    Ref<Attribute> found = _get_type_attribute(
        p_qualified_name, p_attribute_name, p_template_parameter_count, p_from_namespace, CacheType::READWRITE_CACHE);
    if (!found) {
      found = _get_type_attribute(
          p_qualified_name, p_attribute_name, p_template_parameter_count, p_from_namespace, CacheType::READONLY_CACHE);
    }
    return found;
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
    Vector<String> parts = string_split(qualified_name, "::");

    auto itr = std::find_if(parts.begin(), parts.end(), [](const String& part) { return !part.empty(); });
    if (itr == parts.end()) {
      return INVALID_NAME;
    }
    parts = {itr, parts.end()};

    if (parts.size() > 1) {
      for (Size i = 0; i < parts.size(); ++i) {
        if (i != parts.size() - 1) {
          parts[i] = mangle_name(parts[i], INVALID_SIZE);
        } else {
          if (parts[i].empty()) {
            return INVALID_NAME;
          }
          parts[i] = mangle_name(parts[i], template_parameter_count);
        }

        if (parts[i] == INVALID_NAME) {
          return INVALID_NAME;
        }
      }
      return string_vector_combine(parts, "/");
    }

    StreamWriter name_writer;

    Size count = 0;
    Size open = 0;
    Size closed = 0;

    for (char c : qualified_name) {
      if (!(is_whitespace(c) || c == '<' || c == '>' || c == '_' || isalnum(c) || c == ',')) {
        return INVALID_NAME;
      }

      if (open > 0 && open == closed && !is_whitespace(c)) {
        return INVALID_NAME;
      }

      if (c == '<') {
        open++;
      }
      if (c == '>') {
        closed++;
      }
      if (c == ',' && open - closed == 1) {
        count++;
      }
      if (open == 0) {
        name_writer.write_generic(c);
      }
    }

    if (open == closed) {
      if (open > 0) {
        count++;
      }
    } else {
      return "___INVALID___";
    }

    if (template_parameter_count == INVALID_SIZE) {
      template_parameter_count = count;
    }

    if (template_parameter_count == 0) {
      return qualified_name;
    }

    return format("%s_T_ARGS_%d_", name_writer.get_string().c_str(), template_parameter_count);
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
