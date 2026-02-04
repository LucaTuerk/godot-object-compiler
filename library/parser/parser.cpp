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

  Ref<ParserError> TreeSitterParser::parse_file(const String& path, Ref<Context> target) {
    input_is_path = true;
    return parse(path, std::move(target));
  }

  Ref<ParserError> TreeSitterParser::parse(const String& input, Ref<Context> target) {
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
          for (const Ref<INodeHandler>& node_handler : _handlers) {
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