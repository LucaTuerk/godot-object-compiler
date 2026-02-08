#pragma once
#include "library/generator/attribute_parameter_type.h"
#include "library/generator/generator.h"

namespace GodotObjectCompiler {

  class GodotMacroIncludeGenerator : public IGenerator {
    GENERATOR(GodotMacroIncludeGenerator);

   public:

    bool generate_macros(Ref<Context> p_write_to);
    bool generate_attribute_parameter_type(Ref<IAttributeParameterType> p_type, Ref<Context> p_write_to);
    bool generate_prototype_methods(
        const Ref<Context>& p_write_to, const String& p_macro, const Vector<Ref<IAttributeParameterType>>& p_params);

    Ref<Context> generate(Ref<Context> p_tree, Ref<Node> p_entry_point) override;
  };

}  // namespace GodotObjectCompiler
