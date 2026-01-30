#pragma once
#include "library/generator/attribute_parameter_type.h"
#include "library/generator/generator.h"

namespace GodotObjectCompiler {

  class GodotMacroIncludeGenerator : public IGenerator {
    GENERATOR(GodotMacroIncludeGenerator);

   public:

    bool generate_macros(Ref<Context> write_to);
    bool generate_attribute_parameter_type(Ref<IAttributeParameterType> type, Ref<Context> write_to);
    bool generate_prototype_methods(
        const Ref<Context>& write_to, const String& macro, const Vector<Ref<IAttributeParameterType>>& params);

    Ref<Context> generate(Ref<Context> tree, Ref<Node> entry_point) override;
  };

}  // namespace GodotObjectCompiler
