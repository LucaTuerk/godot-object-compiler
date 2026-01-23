#pragma once
#include "../tree/syntax/attribute.h"
#include "library/core/core.h"
#include "parser.h"

namespace GodotObjectCompiler {
  class Node;

  class AttributeArgumentParser : public IParser {
   public:
    void setup_for_macro(const String& macro_name);
    Node* parse(const String& input) override;

   private:
    Dictionary<String, Vector<AttributeParameterType>> const* parameters = nullptr;
  };

}  // namespace GodotObjectCompiler
