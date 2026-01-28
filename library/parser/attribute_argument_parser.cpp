
#include "attribute_argument_parser.h"

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

    result.push_back(strstr.str());
    return result;
  }

  String IAttributeArgumentParser::get_inner_arguments(const String& content) {
    std::stringstream strstr;
    bool open = false;
    bool escaped = false;
    for (char c : content) {
      if (!escaped) {
        if (c == '(') {
          open = true;
          continue;
        } else if (c == ')') {
          return strstr.str();
        } else if (c == '\\') {
          escaped = true;
          strstr << c;
          continue;
        }
      }

      if (open) {
        strstr << c;
        escaped = false;
      }
    }
    return "";
  }

}  // namespace GodotObjectCompiler