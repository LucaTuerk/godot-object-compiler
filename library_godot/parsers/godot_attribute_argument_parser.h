
#pragma once
#include "library/attribute_db.h"
#include "library/generator/attribute_parameter_type.h"
#include "library/parser/attribute_argument_parser.h"

namespace GodotObjectCompiler {

  class GodotAttributeArgumentParser : public IAttributeArgumentParser {
   public:

    explicit GodotAttributeArgumentParser(const Ref<Attribute>& attribute) : weak_attribute(attribute) {}

    Ref<ParserError> parse_attribute_arguments(const String& p_content, Ref<Context> p_target) override;

    Ref<ParserError> parse_attribute_argument(const String& p_content, Ref<Context> p_target);

    Ref<ParserError> parse_inner_arguments(
        const String& p_content, Ref<Context> p_target, const IAttributeParameterType::Argument& p_parameter);

   private:

    WeakRef<Attribute> weak_attribute;
  };

}  // namespace GodotObjectCompiler
