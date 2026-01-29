
#pragma once
#include "library/attribute_db.h"
#include "library/generator/attribute_parameter_type.h"
#include "library/parser/attribute_argument_parser.h"

namespace GodotObjectCompiler {

  class GodotAttributeArgumentParser : public IAttributeArgumentParser {
   public:

    explicit GodotAttributeArgumentParser(const Ref<Attribute>& attribute) : weak_attribute(attribute) {}

    Ref<ParserError> parse_attribute_arguments(const String& content, Ref<Context> target) override;
    Ref<ParserError> parse_attribute_argument(const String& content, Ref<Context> target);
    Ref<ParserError> parse_inner_arguments(
        const String& content, Ref<Context> target, const IAttributeParameterType::Argument& parameter);

   private:

    WeakRef<Attribute> weak_attribute;
  };

}  // namespace GodotObjectCompiler
