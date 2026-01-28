#include "godot_macro_include_generator.h"

#include "../../library/type_db.h"
#include "library/attribute_db.h"
#include "library/core/helpers.h"
#include "library/tree/output/output.h"
#include "library/tree/output/output_file.h"
#include "library/tree/predicates.h"
#include "library/tree/syntax/enum.h"
#include "library_godot/attributes/godot_property_hint_parameter_type.h"
#include "library_godot/attributes/godot_property_usage_flags_parameter_type.h"

namespace GodotObjectCompiler {

  Vector<Vector<Size>> find_all_subsets(Size n) {
    if (n == 0) {
      return {{}};
    }

    Vector<Vector<Size>> prev = find_all_subsets(n - 1);
    Vector<Vector<Size>> result;
    std::copy(prev.begin(), prev.end(), std::back_inserter(result));

    for (Vector<Size>& val : result) {
      val.push_back(n - 1);
    }
    std::copy(prev.begin(), prev.end(), std::back_inserter(result));

    return result;
  }

  bool GodotMacroIncludeGenerator::generate_macros(Ref<Context> write_to) {
    write_to->add_child(Writer::Define("GOC_BODY_COMBINE_INNER",
        {Writer::Text("A"), Writer::Text("B"), Writer::Text("C"), Writer::Text("D")}, "A##B##C##D"));
    write_to->add_child(
        Writer::Define("GOC_BODY_COMBINE", {Writer::Text("A"), Writer::Text("B"), Writer::Text("C"), Writer::Text("D")},
            "GOC_BODY_COMBINE_INNER(A, B, C, D)"));
    String generated_content = "GOC_BODY_COMBINE(GOC_GENERATED_, __LINE__, _, GOC_FILE_ID())()";

    for (const String& macro : AttributeDB::instance()->get_all_macros()) {
      Ref<Attribute> attr = AttributeDB::instance()->create_for_macro(macro);
      if (attr->is<GeneratedBodyAttribute>() || attr->is<GeneratedGlobalAttribute>()) {
        write_to->add_child(Writer::Define(macro, {Writer::Text("...")},
             generated_content + "; static_assert(" + macro + "_prototype(__VA_ARGS__),\"\");"));
      } else {
        write_to->add_child(
            Writer::Define(macro, {Writer::Text("...")}, "static_assert(" + macro + "_prototype(__VA_ARGS__),\"\")"));
      }
    }
    // write_to->add_child(Writer::Define("GODOT_GENERATED_BODY", {Writer::Text("...")}, generated_content));
    // write_to->add_child(Writer::Define("GODOT_GENERATED_GLOBAL", {Writer::Text("...")}, generated_content));
    // write_to->add_child(Writer::Define(
    //     "GODOT_PROPERTY", {Writer::Text("...")}, "static_assert(_godot_prop_prototype(__VA_ARGS__),\"\")"));
    // write_to->add_child(Writer::Define("GODOT_FUNCTION", {Writer::Text("...")}, ""));
    // write_to->add_child(Writer::Define("GODOT_CLASS", {Writer::Text("...")}, ""));
    // write_to->add_child(Writer::Define("GODOT_ENUM", {Writer::Text("...")}, ""));
    return true;
  }

  bool GodotMacroIncludeGenerator::generate_attribute_type(Ref<IAttributeParameterType> type, Ref<Context> write_to) {
    String type_name = type->get_type_name();

    write_to->build_child<Writer::SnippetNode>("class " + type_name + " {};");

    if ((type->get_features() & IAttributeParameterType::FEATURE_FLAG)) {
      // clang-format off
      write_to->build_child<Function>().with_children({
        build<ConstExpression>(),
        build<Type>().with_child<Identifier>(type_name),
        build<Identifier>("operator |"),
        build<Parameters>().with_children({
          build<Parameter>().with_children({
            build<Type>().with_child<Identifier>(type_name),
            build<Identifier>(" a")
          }),
            build<Parameter>().with_children({
            build<Type>().with_child<Identifier>(type_name),
            build<Identifier>(" b")
          })
        }),
        build<Body>().with_child(Writer::Return("{}"))
      });
      // clang-format on
    }

    const Vector<IAttributeParameterType::Argument> arguments = type->get_arguments();
    for (const String& value_name : type->get_value_names()) {
      if (arguments.empty()) {
        // clang-format off
        write_to->build_child<Field>().with_children({
          build<ConstExpression>(),
          build<Type>().with_child<Identifier>(type_name),
          build<Identifier>(value_name),
          build<Literal>("= {}")
        }).with_child(Writer::Semicolon());
        // clang-format on
      } else {
        Ref<Parameters> parameters;
        // clang-format off
        write_to->build_child<Function>().with_children({
        build<ConstExpression>(),
        build<Type>().with_child<Identifier>(type_name),
        build<Identifier>(value_name),
        build_ref<Parameters>(&parameters),
        build<Body>().with_child(Writer::Return("{}"))});
        // clang-format on

        for (const auto& argument : arguments) {
          if (argument.type == IAttributeParameterType::ARG_STRING) {
            // clang-format off
              Ref<Parameter> parameter;
              parameters->add_child(build_ref<Parameter>(&parameter).with_children({
              build<Type>().with_children({
                build<Const>(),
                build<Identifier>("char"),
                build<Pointer>()
               }),
              build<Identifier>("hint_string"),
              }));
            // clang-format on
            if (argument.optional) {
              parameter->build_child<Literal>("\"\"");
            }
          } else {
            print_err("UNIMPLEMENTED");
            return false;
          }
        }
      }
    }
    return true;
  }

  bool GodotMacroIncludeGenerator::generate_prototype_methods(
      const Ref<Context>& write_to, const String& macro, const Vector<Ref<IAttributeParameterType>>& params) {
    Vector<Vector<Size>> subsets = find_all_subsets(params.size());
    for (Vector<Size>& subset : subsets) {
      do {
        Ref<Parameters> parameters;
        // clang-format off
        write_to->build_child<Function>().with_children({
          build<ConstExpression>(),
          build<Type>().with_child<Identifier>("bool"),
          build<Identifier>(macro+"_prototype"),
          build_ref<Parameters>(&parameters),
          build<Body>().with_child(Writer::Return("true"))
        });
        // clang-format on

        for (Size index : subset) {
          parameters->build_child<Parameter>().with_child(Writer::Text(params[index]->get_type_name()));
        }
      } while (std::next_permutation(subset.begin(), subset.end()));
    }
    return true;
  }

  Ref<Context> GodotMacroIncludeGenerator::generate(Ref<Context> tree, Ref<Node> entry_point) {
    Ref<Context> entry = entry_point->as<Context>();
    if (!entry) {
      return nullptr;
    }
    entry->add_child(Writer::PragmaOnce());

    generate_macros(entry);

    HashSet<String> generated_param_types;
    for (const String& macro : AttributeDB::instance()->get_all_macros()) {
      Ref<Attribute> attribute = AttributeDB::instance()->create_for_macro(macro);
      Vector<Ref<IAttributeParameterType>> params = AttributeDB::instance()->get_parameters_for_macro(macro);

      for (const auto& param : params) {
        if (generated_param_types.find(param->get_type_name()) == generated_param_types.end()) {
          generated_param_types.insert(param->get_type_name());
          generate_attribute_type(param, entry);
        }
      }

      generate_prototype_methods(entry, macro, params);
    }

    return entry;
  }

}  // namespace GodotObjectCompiler