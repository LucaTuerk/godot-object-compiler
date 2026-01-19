#pragma once

#include <utility>

#include "library/core/core.h"
#include "library/tree/all.h"

namespace GodotObjectCompiler {
  namespace Writer {
    class IOutputNode {
     public:
      virtual String get_output() = 0;
    };

    class Indent : public Context, public IOutputNode {
      NODE_TYPE(Indent)
      LAZY(Indent, Size, total_amount);

      explicit Indent(Size amount) : amount(amount) {}
      String get_output() override;
      Size amount = 0;
    };

    class Enclosing : public Context {
      NODE_TYPE(Enclosing)
      Enclosing(String before, String after)
          : before(std::move(before)), after(std::move(after)) {}

      String before;
      String after;
    };

    Enclosing* Brackets();
    Enclosing* SquareBrackets();
    Enclosing* Braces();
    Enclosing* Chevrons();

    class List : public Context {
      NODE_TYPE(List)
      List(String delimiter, bool before_first, bool after_last)
          : delimiter(std::move(delimiter)),
            before_first(before_first),
            after_last(after_last) {}

      String delimiter;
      bool before_first = false;
      bool after_last = true;
    };

    List* Lines();
    List* Parameters();

  }  // namespace Writer
}  // namespace GodotObjectCompiler
