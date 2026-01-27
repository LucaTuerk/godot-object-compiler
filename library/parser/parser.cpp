#include "parser.h"

#include "../core/string_writer.h"
#include "../tree/syntax/all.h"
#include "handlers/all.h"
#include "helpers.h"
#include "library/attribute_db.h"
#include "node_handler.h"

namespace GodotObjectCompiler {

  void debug_print_node(const TSNode& node, int depth = 0) {
    for (int i = 0; i < depth; i++) {
      printf("  ");
    }
    printf("%s\n", ts_node_type(node));
  }

  void debug_print_tree(TSNode root, int depth = 0) {
    TSTreeCursor cursor = ts_tree_cursor_new(root);

    TSNode node = ts_tree_cursor_current_node(&cursor);
    if (!ts_node_is_null(node)) {
      debug_print_node(node, depth);
    }

    ts_tree_cursor_goto_first_child(&cursor);
    for (uint32_t i = 0; i < ts_node_child_count(node); i++) {
      debug_print_tree(ts_tree_cursor_current_node(&cursor), depth + 1);
      ts_tree_cursor_goto_next_sibling(&cursor);
    }

    ts_tree_cursor_delete(&cursor);
  }

  Ref<Node> TreeSitterParser::parse(const String& input) {
    using NodeID = const void*;

    Dictionary<NodeID, Ref<Context>> before_node;
    Dictionary<Size, String> stripped_parameters;

    String local_input = Parser::Helpers::remove_macros(input);
    local_input = strip_known_macro_contents(local_input, stripped_parameters);

    context = ParserContext(local_input);
    context.stripped_parameters = stripped_parameters;

    if (!context.is_valid()) {
      return ExecutionContext::instance()->get_node_db()->create<Namespace>();
    }

    debug_print_tree(context.node);

    while (true) {
      bool do_continue = true;

      do {
        context.node = ts_tree_cursor_current_node(&context.cursor);
        if (ts_node_is_null(context.node)) {
          print_err("Cursor is pointing at null node.");
          break;
        }

        do_continue = true;
        NextStep step = UNDECIDED;
        String type = ts_node_type(context.node);

        for (INodeHandler* handler : _handlers) {
          if (handler->handles_node(context.node, type)) {
            Ref<Context> tmp = context.current_node;

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
          case STEP_OVER_SPECIFIC:
            while (ts_tree_cursor_current_node(&context.cursor).id != context.specific_step_id) {
              do_continue = ts_tree_cursor_goto_parent(&context.cursor);
            }
            ts_tree_cursor_goto_next_sibling(&context.cursor);
        }

      } while (do_continue);

      bool has_reached_root = false;

      do {
        if (!ts_tree_cursor_goto_parent(&context.cursor)) {
          has_reached_root = true;
        }

        if (auto itr = before_node.find(ts_tree_cursor_current_node(&context.cursor).id); itr != before_node.end()) {
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

  Ref<Node> TreeSitterParser::parse(const String& input, std::vector<INodeHandler*> handlers) {
    using NodeID = const void*;

    Dictionary<NodeID, Ref<Context>> before_node;
    Dictionary<Size, String> stripped_parameters;

    String local_input = strip_known_macro_contents(input, stripped_parameters);

    context = ParserContext(local_input);
    context.stripped_parameters = stripped_parameters;

    if (!context.is_valid()) {
      return ExecutionContext::instance()->get_node_db()->create<Namespace>();
    }

    debug_print_tree(context.node);

    while (true) {
      bool do_continue = true;

      do {
        context.node = ts_tree_cursor_current_node(&context.cursor);
        if (ts_node_is_null(context.node)) {
          print_err("Cursor is pointing at null node.");
          break;
        }

        do_continue = true;
        NextStep step = UNDECIDED;
        String type = ts_node_type(context.node);

        for (INodeHandler* handler : handlers) {
          if (handler->handles_node(context.node, type)) {
            Ref<Context> tmp = context.current_node;

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
          case STEP_OVER_SPECIFIC:
            while (ts_tree_cursor_current_node(&context.cursor).id != context.specific_step_id) {
              do_continue = ts_tree_cursor_goto_parent(&context.cursor);
            }
            ts_tree_cursor_goto_next_sibling(&context.cursor);
        }

      } while (do_continue);

      bool has_reached_root = false;

      do {
        if (!ts_tree_cursor_goto_parent(&context.cursor)) {
          has_reached_root = true;
        }

        if (auto itr = before_node.find(ts_tree_cursor_current_node(&context.cursor).id); itr != before_node.end()) {
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

  // TreeSitter does not handle macro parameters well
  // but it works if the parameters are empty, so strip them before processing
  String TreeSitterParser::strip_known_macro_contents(const String& input, Dictionary<Size, String>& parameters) {
    String local_input = input;
    Vector<String> macros = AttributeDB::instance()->get_all_macros();

    for (const String& macro : macros) {
      Size index = 0;
      Vector<Size> positions;
      {
        Size position = local_input.find(macro);
        while (position != String::npos) {
          positions.push_back(position);
          position = local_input.find(macro, position + 1);
        }
      }

      StreamWriter writer;
      for (Size position : positions) {
        // TODO FIX THIS
        Size bracket_open_pos = local_input.find('(', position);
        Size bracket_closed_pos = local_input.find(')', position);
        if (bracket_open_pos > bracket_closed_pos) {
          continue;
        }
        if (bracket_open_pos == String::npos || bracket_closed_pos == String::npos) {
          continue;
        }

        parameters.insert({position, input.substr(bracket_open_pos + 1, bracket_closed_pos - bracket_open_pos - 1)});

        writer.write(local_input.substr(index, bracket_open_pos - index + 1));
        for (char c : local_input.substr(bracket_open_pos + 1, bracket_closed_pos - bracket_open_pos - 1)) {
          if (c == ' ') {
            writer.write(" ");
          } else if (c == '\t') {
            writer.write("\t");
          } else if (c == '\n') {
            writer.write("\n");
          } else {
            writer.write(" ");
          }
        }
        index = bracket_closed_pos;
      }
      writer.write(local_input.substr(index));
      local_input = writer.get_string();
    }
    return local_input;
  }

}  // namespace GodotObjectCompiler