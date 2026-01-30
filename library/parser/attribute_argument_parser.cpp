
#include "attribute_argument_parser.h"

#include <cstring>

#include "../tree/syntax/all.h"
#include "../tree/syntax/function.h"
#include "../tree/syntax/identifier.h"
#include "library/core/helpers.h"

namespace GodotObjectCompiler {

  Vector<String> IAttributeArgumentParser::split_arguments(const String& content) {
    std::stringstream strstr;
    Vector<String> result;

    Size brackets_open = 0;
    Size quotes_open = 0;
    bool escaped = false;

    if (string_trim(content).empty()) {
      return {};
    }

    for (char c : content) {
      if (!escaped) {
        if (c == '(') {
          brackets_open++;
        } else if (c == ')') {
          if (brackets_open == 0) {
            return {};
          }
          brackets_open--;
        } else if (c == '"') {
          if (quotes_open) {
            quotes_open--;
          } else {
            quotes_open++;
          }
        } else if (c == '\\') {
          escaped = true;
          strstr << c;
          continue;
        } else if (c == ',') {
          if (!quotes_open && !brackets_open) {
            result.push_back(strstr.str());
            strstr = {};
            continue;
          }
        }
      }

      strstr << c;
      escaped = false;
    }

    result.push_back(string_trim(strstr.str()));
    return result;
  }

  void IAttributeArgumentParser::split_outer_inner(const String& content, String& outer, String& inner) {
    std::stringstream inner_strm;
    std::stringstream outer_strm;
    outer = "";
    inner = "";

    bool open = false;
    bool escaped = false;
    for (char c : content) {
      if (!escaped) {
        if (c == '(') {
          open = true;
          continue;
        } else if (c == ')') {
          break;
        } else if (c == '\\') {
          escaped = true;
          inner_strm << c;
          continue;
        }
      }

      if (open) {
        inner_strm << c;
        escaped = false;
      } else {
        outer_strm << c;
      }
    }

    outer = string_trim(outer_strm.str());
    inner = string_trim(inner_strm.str());
  }

  Vector<String> IAttributeArgumentParser::split_flags(const String& content) {
    Vector<String> split = string_split(content, "|");
    std::transform(split.cbegin(), split.cend(), split.begin(), &string_trim);
    return split;
  }

}  // namespace GodotObjectCompiler