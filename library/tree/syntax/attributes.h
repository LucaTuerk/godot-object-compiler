
#pragma once
#include "attribute.h"
#include "class.h"
#include "field.h"
#include "function.h"

namespace GodotObjectCompiler {

  // This attribute applies to the class it is contained in
  class ClassAttribute : public Attribute {
    ATTRIBUTE_TYPE(ClassAttribute, Class, CONTAINING);
  };

  // This attribute provides the generated body include location hook
  class GeneratedBodyAttribute : public Attribute {
    ATTRIBUTE_TYPE(GeneratedBodyAttribute, Class, CONTAINING);
  };

  // This attributes provides the location hook for generators operating in the global namespace
  class GeneratedGlobalAttribute : public Attribute {
    ATTRIBUTE_TYPE(GeneratedGlobalAttribute, Namespace, CONTAINING);
  };

  // This attribute applies to the next sibling function
  class FunctionAttribute : public Attribute {
    ATTRIBUTE_TYPE(FunctionAttribute, Function, NEXT)
  };

  // This attribute applies to the next sibling field
  class FieldAttribute : public Attribute {
    ATTRIBUTE_TYPE(FieldAttribute, Field, NEXT)
  };

  // This attribute does not apply to any specific node in the syntax tree but
  // can provide a hint to the generator.
  class HintAttribute : public Attribute {
    ATTRIBUTE_TYPE(HintAttribute, Attribute, NONE)
  };

}  // namespace GodotObjectCompiler
