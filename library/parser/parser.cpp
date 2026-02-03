#include "parser.h"

#include <utility>

#include "../core/string_writer.h"
#include "../tree/syntax/all.h"
#include "handlers/all.h"
#include "helpers.h"
#include "library/attribute_db.h"
#include "node_handler.h"
#include "tree_sitter_node.h"

namespace GodotObjectCompiler {

  // void debug_print_node(const TSNode& node, int depth = 0) {
  //   for (int i = 0; i < depth; i++) {
  //     printf("  ");
  //   }
  //   printf("%s\n", ts_node_type(node));
  // }
  //
  // void debug_print_tree(TSNode root, int depth = 0) {
  //   TSTreeCursor cursor = ts_tree_cursor_new(root);
  //
  //   TSNode node = ts_tree_cursor_current_node(&cursor);
  //   if (!ts_node_is_null(node)) {
  //     debug_print_node(node, depth);
  //   }
  //
  //   ts_tree_cursor_goto_first_child(&cursor);
  //   for (uint32_t i = 0; i < ts_node_child_count(node); i++) {
  //     debug_print_tree(ts_tree_cursor_current_node(&cursor), depth + 1);
  //     ts_tree_cursor_goto_next_sibling(&cursor);
  //   }
  //
  //   ts_tree_cursor_delete(&cursor);
  // }

  Ref<ParserError> TreeSitterParser::parse_file(const String& path, Ref<Context> target) {
    input_is_path = true;
    return parse(path, std::move(target));
  }

  Ref<ParserError> TreeSitterParser::parse(const String& input, Ref<Context> target) {
    /**using NodeID = const void*;

    Dictionary<NodeID, Ref<Context>> before_node;
    Dictionary<Size, String> stripped_parameters;

    String original_input =
        input_is_path ? Parser::Helpers::remove_macros(read_file(input)) : Parser::Helpers::remove_macros(input);
    String local_input = strip_known_macro_contents(original_input, stripped_parameters);

    context = ParserContext(local_input);
    context.original_buffer = original_input;
    if (input_is_path) {
      context.file_path = input;
      input_is_path = false;
    }
    context.stripped_parameters = stripped_parameters;

    if (!context.is_valid()) {
      return node_new<ParserError>(ERROR, "TreeSitterParser: Invalid parser context.");
    }

    auto global_namespace = target->as<Namespace>();
    if (!global_namespace || !global_namespace->qualified_name().empty()) {
      return node_new<ParserError>(
          ERROR, "TreeSitterParser: Invalid target node, expected to be the global namespace.");
    }
    context.global_namespace = global_namespace;
    Ref<Body> body = global_namespace->build_child<Body>();
    context.current_node = body;

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

        if (type != context.ts_node->type) {
          print_err("Mismatch");
          break;
        } else {
          print_ln("Success");
        }

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
              if (!context.ts_node->empty()) {
                context.ts_node = context.ts_node->get_child(0)->as<TreeSitterNode>();
              }
            } else {
              do_continue = ts_tree_cursor_goto_next_sibling(&context.cursor);
              if (context.ts_node->has_next_sibling()) {
                context.ts_node = context.ts_node->get_next_sibling()->as<TreeSitterNode>();
              }
            }
            break;
          case STEP_INTO:
            do_continue = ts_tree_cursor_goto_first_child(&context.cursor);
            if (!context.ts_node->empty()) {
              context.ts_node = context.ts_node->get_child(0)->as<TreeSitterNode>();
            }
            break;
          case STEP_OVER:
            do_continue = ts_tree_cursor_goto_next_sibling(&context.cursor);
            if (context.ts_node->has_next_sibling()) {
              context.ts_node = context.ts_node->get_next_sibling()->as<TreeSitterNode>();
            }
            break;
          case STEP_OUT:
            do_continue = ts_tree_cursor_goto_parent(&context.cursor);
            if (context.ts_node->has_parent()) {
              context.ts_node = context.ts_node->get_parent()->as<TreeSitterNode>();
            }
            break;
          case STEP_OVER_SPECIFIC:
            while (ts_tree_cursor_current_node(&context.cursor).id != context.specific_step_id) {
              do_continue = ts_tree_cursor_goto_parent(&context.cursor);
              context.ts_node = context.ts_node->get_parent()->as<TreeSitterNode>();
            }
            ts_tree_cursor_goto_next_sibling(&context.cursor);
            context.ts_node = context.ts_node->get_next_sibling()->as<TreeSitterNode>();
        }

      } while (do_continue);

      bool has_reached_root = false;

      do {
        if (!ts_tree_cursor_goto_parent(&context.cursor)) {
          has_reached_root = true;
        }

        if (context.ts_node->has_parent()) {
          context.ts_node = context.ts_node->get_parent()->as<TreeSitterNode>();
        }

        if (auto itr = before_node.find(ts_tree_cursor_current_node(&context.cursor).id); itr != before_node.end()) {
          context.current_node = itr->second;
        }

        if (context.ts_node->has_next_sibling()) {
          context.ts_node = context.ts_node->get_next_sibling()->as<TreeSitterNode>();
        }
      } while (!has_reached_root && !ts_tree_cursor_goto_next_sibling(&context.cursor));

      if (context.current_node == nullptr) {
        print_err("Reached topmost node early.");
        break;
      }

      if (has_reached_root) {
        break;
      }
    }*/

    Dictionary<Size, String> stripped_parameters;
    Dictionary<UID, Ref<Context>> before_node;
    HashSet<UID> handled;

    String original_input =
        input_is_path ? Parser::Helpers::remove_macros(read_file(input)) : Parser::Helpers::remove_macros(input);
    String local_input = strip_known_macro_contents(original_input, stripped_parameters);
    ParserContext context = ParserContext(local_input);
    context.stripped_parameters = stripped_parameters;

    if (!context.is_valid()) {
      return node_new<ParserError>(ERROR, "Failed to setup context.");
    }

    auto global_namespace = target->as<Namespace>();
    if (!global_namespace || !global_namespace->qualified_name().empty()) {
      return node_new<ParserError>(
          ERROR, "TreeSitterParser: Invalid target node, expected to be the global namespace.");
    }

    print_ln(context.current_src->pretty_print());

    Ref<Body> body = global_namespace->create_child<Body>();
    context.current_target = body;

    while (true) {
      bool do_continue = true;

      do {
        bool already_handled = handled.find(context.current_src->get_id()) != handled.end();
        ParserStep step = already_handled ? ParserStep::StepOver() : ParserStep::Undecided();

        if (!already_handled) {
          for (const Ref<INodeHandlerV2>& node_handler : _handlers_v2) {
            if (node_handler->handles_node(context.current_src)) {
              Ref<Context> before = context.current_target;
              step = node_handler->handle(context.current_src, context.current_target);

              if (before != context.current_target) {
                before_node[context.current_src->get_id()] = before;
              }
              break;
            }
          }
          handled.insert(context.current_src->get_id());
        }

        if (step.is_undecided()) {
          if (!context.current_src->empty()) {
            step = ParserStep::StepInto();
          } else {
            step = ParserStep::StepOver();
          }
        }

        if (step.is_step_into()) {
          do_continue = !context.current_src->empty();
          if (do_continue) {
            context.current_src = context.current_src->get_child(0)->as<TreeSitterNode>();
          }
        } else if (step.is_step_over()) {
          do_continue = context.current_src->has_next_sibling();
          if (do_continue) {
            context.current_src = context.current_src->get_next_sibling()->as<TreeSitterNode>();
          }
        } else if (step.is_step_out()) {
          do_continue = context.current_src->has_parent();
          if (do_continue) {
            context.current_src = context.current_src->get_parent()->as<TreeSitterNode>();
          }
        } else if (Ref<TreeSitterNode> go_to; step.is_go_to(go_to)) {
          do_continue = go_to != nullptr;
          if (do_continue) {
            context.current_src = go_to;

            do_continue = context.current_src->has_next_sibling();
            if (do_continue) {
              context.current_src = context.current_src->get_next_sibling()->as<TreeSitterNode>();
            }
          }
        }
      } while (do_continue);

      if (!context.current_src->has_parent()) {
        break;
      }
      context.current_src = context.current_src->get_parent()->as<TreeSitterNode>();
      if (auto itr = before_node.find(context.current_src->get_id()); itr != before_node.end()) {
        context.current_target = itr->second;
      }
    }

    print_ln(context.current_target->pretty_print());
    return ParserError::OK;
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

      std::sort(positions.begin(), positions.end());

      StreamWriter writer;
      for (Size position : positions) {
        Size open_index = position;
        bool found_whitespace = false;
        bool no_args = false;

        auto itr = std::next(local_input.begin(), position);
        while (itr != local_input.end()) {
          if (*itr == '(') {
            break;
          }

          bool whitespace = is_whitespace(*itr);
          if (whitespace && !found_whitespace) {
            found_whitespace = true;
          } else if (!whitespace && found_whitespace) {
            no_args = true;
            break;
          }

          ++itr;
          ++open_index;
        }

        if (no_args) {
          continue;
        }

        StreamWriter content;
        Size opened = 1;
        Size closed_index = open_index + 1;

        if (itr == local_input.end()) {
          // EHH Fix this
          return "";
        }

        ++itr;
        while (itr != local_input.end()) {
          if (*itr == '(') {
            opened++;
          }
          if (*itr == ')') {
            opened--;
          }

          if (opened == 0) {
            break;
          }
          content.write_generic(*itr);
          ++itr;
          ++closed_index;
        }

        parameters.insert({position, content.get_string()});

        writer.write(local_input.substr(index, open_index - index + 1));
        for (char c : content.get_string()) {
          if (is_whitespace(c)) {
            writer.write_generic(c);
          } else {
            writer.write(" ");
          }
        }
        index = closed_index;
      }
      writer.write(local_input.substr(index));
      local_input = writer.get_string();
    }
    return local_input;
  }

}  // namespace GodotObjectCompiler