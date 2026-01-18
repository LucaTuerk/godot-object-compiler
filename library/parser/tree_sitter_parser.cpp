#include "library/parser/tree_sitter_parser.h"
#include "library/core/core.h"
#include "library/tree/modifiers.h"
#include "tree_sitter_parser/handlers/access_specifier_handler.h"
#include "tree_sitter_parser/handlers/class_handler.h"
#include "tree_sitter_parser/handlers/generic_step_into.h"
#include "tree_sitter_parser/handlers/namespace_handler.h"
#include "tree_sitter_parser/handlers/storage_class_handler.h"
#include "tree_sitter_parser/handlers/struct_handler.h"
#include "tree_sitter_parser/handlers/virtual_handler.h"
#include "tree_sitter_parser/node_handler.h"

#include <fcntl.h>
#include <tree_sitter/api.h>
#include <tree_sitter/tree-sitter-cpp.h>

namespace GodotObjectCompiler {

    void debug_print_node(const TSNode& node, int depth = 0) {
        for (int i = 0; i < depth; i++) {
            printf("  ");
        }
        printf("%s\n", ts_node_type(node));
    }

	Node * TreeSitterParser::parse(const String& input) {
		using NodeID = const void*;

    	Dictionary<NodeID, Context*> before_node;

    	context = ParserContext(input);
        if (!context.is_valid()) {
            return NodeDB::get_instance()->create<Namespace>();
        }

    	while (true) {
    		bool do_continue = true;

    		do {
    			context.node = ts_tree_cursor_current_node(&context.cursor);
				if (ts_node_is_null(context.node)) {
					print_err("Cursor is pointing at null node.");
					break;
				}

				debug_print_node(context.node, ts_tree_cursor_current_depth(&context.cursor));

    			do_continue = true;
    			NextStep step = UNDECIDED;
    			String type = ts_node_type(context.node);

    			for ( INodeHandler* handler : _handlers) {
    				if (handler->handles_node(context.node, type)) {
						Context * tmp = context.current_node;

    					step = handler->handle(context);

						if (tmp != context.current_node) {
							before_node[context.node.id] = tmp;
    					}

    					break;
    				}
    			}

    			switch (step) {
    				case UNDECIDED:
						if (ts_node_child_count(context.node) > 0) {
							do_continue = ts_tree_cursor_goto_first_child(&context.cursor);
						} else {
							do_continue = ts_tree_cursor_goto_next_sibling(&context.cursor);
						}
    					break;
    				case STEP_INTO:
    					do_continue = ts_tree_cursor_goto_first_child(&context.cursor);
    					break;
    				case STEP_OVER:
    					do_continue = ts_tree_cursor_goto_next_sibling(&context.cursor);
    					break;
    				case STEP_OUT:
    					do_continue = ts_tree_cursor_goto_parent(&context.cursor);
    					break;
    			}

    		} while (do_continue);

    		bool has_reached_root = false;

    		do {
    			if (!ts_tree_cursor_goto_parent(&context.cursor)) {
    				has_reached_root = true;
    			}

    			if (auto itr = before_node.find(context.node.id); itr != before_node.end()) {
    				context.current_node = itr->second;
    			}
    		} while (!has_reached_root && !ts_tree_cursor_goto_next_sibling(&context.cursor));

			if (context.current_node == nullptr) {
				print_err("Reached topmost node early.");
				break;
			}

    		if (has_reached_root) {
    			break;
    		}
    	}

        return context.global_namespace;
    }
}
