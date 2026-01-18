#include "library/parser/tree_sitter_parser/parser_context.h"

namespace GodotObjectCompiler {

    String ParserContext::get_child_content(const TSNode& p_node, String p_child_name) {
        TSNode child = get_child_node(p_node, p_child_name);
        return copy_node_content(child);
    }

    TSNode ParserContext::get_child_node(const TSNode& p_node, String p_field_name) {
        return ts_node_child_by_field_name(p_node, p_field_name.c_str(), p_field_name.length());
    }

    String ParserContext::copy_node_content(const TSNode& p_node) {
        return buffer.substr(
            ts_node_start_byte(p_node),
            ts_node_end_byte(p_node) - ts_node_start_byte(p_node)
        );
    }

    Namespace* ParserContext::create_namespace() {
        Namespace* result = current_node->create_child<Namespace>();
        Identifier* name = result->create_child<Identifier>();
        name->name = get_child_content(node, "name");
        return result;
    }

    Class* ParserContext::create_class() {
        Class* result = current_node->create_child<Class>();
        Identifier* name = result->create_child<Identifier>();
        name->name = get_child_content(node, "name");
        return result;
    }

    Struct* ParserContext::create_struct() {
        Struct* result = current_node->create_child<Struct>();
        Identifier* name = result->create_child<Identifier>();
        name->name = get_child_content(node, "name");
        return result;
    }

	bool ParserContext::is_valid() const {
		uint32_t node_child_count = ts_node_child_count(node);
		return !_invalid && node_child_count > 0;
	}

	ParserContext::ParserContext(const String &input) {
	    buffer = input;
    	global_namespace = NodeDB::get_instance()->create<Namespace>();
    	current_node = global_namespace;

    	parser = ts_parser_new();
    	ts_parser_set_language(parser, tree_sitter_cpp());
    	tree = ts_parser_parse_string(parser, nullptr, buffer.c_str(), buffer.length());
    	node = ts_tree_root_node(tree);
    	cursor = ts_tree_cursor_new(node);
    	_invalid = ts_node_is_null(node);
    }

	ParserContext::~ParserContext() {
    	if (!_invalid) {
    		// ts_tree_cursor_delete(&cursor);
    		// ts_tree_delete(tree);
    	}
    }

} //namespace GodotObjectCompiler
