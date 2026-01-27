#include "macro_include_generator.h"

#include "library/core/helpers.h"
#include "library/tree/output/output.h"
#include "library/tree/output/output_file.h"
#include "library/tree/predicates.h"
#include "library/tree/syntax/enum.h"

namespace GodotObjectCompiler {

  const Predicate<Enum> VariantTypePredicate = NamedContextPredicates::qualified_name<Enum>("Variant::Type");

  const Predicate<Enum> PropertyHintPredicate = NamedContextPredicates::qualified_name<Enum>("PropertyHint");

  const Predicate<Enum> PropertyUsageFlagsPredicate =
      NamedContextPredicates::qualified_name<Enum>("PropertyUsageFlags");

  const Vector<String> godot_property_prototype_args = {
      "Hint hint",
      "GetSet get_set",
      "int flags",
  };

  Vector<Vector<Size>> find_all_subsets(Size n) {
    if (n == 1) {
      return {{}, {0}};
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

  Context* MacroIncludeGenerator::generate(Context* tree, Node* entry_point) {
    Context* entry = entry_point->as<Context>();

    entry->add_child(Writer::PragmaOnce());
    entry->add_child(Writer::Define("GOC_BODY_COMBINE_INNER",
        {Writer::Text("A"), Writer::Text("B"), Writer::Text("C"), Writer::Text("D")}, "A##B##C##D"));
    entry->add_child(
        Writer::Define("GOC_BODY_COMBINE", {Writer::Text("A"), Writer::Text("B"), Writer::Text("C"), Writer::Text("D")},
            "GOC_BODY_COMBINE_INNER(A, B, C, D)"));
    auto generated_content = "GOC_BODY_COMBINE(GOC_GENERATED_, __LINE__, _, GOC_FILE_ID())()";
    entry->add_child(Writer::Define("GODOT_GENERATED_BODY", {Writer::Text("...")}, generated_content));
    entry->add_child(Writer::Define("GODOT_GENERATED_GLOBAL", {Writer::Text("...")}, generated_content));
    entry->add_child(Writer::Define("GODOT_PROPERTY", {Writer::Text("...")}, ""));
    entry->add_child(Writer::Define("GODOT_FUNCTION", {Writer::Text("...")}, ""));
    entry->add_child(Writer::Define("GODOT_CLASS", {Writer::Text("...")}, ""));
    entry->add_child(Writer::Define("GODOT_ENUM", {Writer::Text("...")}, ""));
    return entry;
    //
    // Context* result = node_new<OutputFile>();
    //
    // Enum* variant_type = tree->find_descendant<Enum>(BFS, VariantTypePredicate);
    // Enum* property_hint = tree->find_descendant<Enum>(BFS, PropertyHintPredicate);
    // Enum* property_usage_flags = tree->find_descendant<Enum>(BFS, PropertyUsageFlagsPredicate);
    //
    // bool valid = true;
    // if (!variant_type) {
    //   print_err("Failed to find variant type");
    //   valid = false;
    // }
    //
    // if (!property_hint) {
    //   print_err("Failed to find property hind");
    //   valid = false;
    // }
    //
    // if (!property_usage_flags) {
    //   print_err("Failed to find property usage");
    //   valid = false;
    // }
    //
    // if (!valid) {
    //   return result;
    // }
    //
    // const Vector<String>& variant_type_names = variant_type->value_names();
    // const Vector<String>& property_hint_names = property_hint->value_names();
    // const Vector<String>& property_usage_names = property_usage_flags->value_names();
    //
    // Writer::ListNode* lines = Writer::Lines({});
    //
    // lines->add_child(Writer::Define("GOC_BODY_COMBINE_INNER",
    //     {Writer::Text("A"), Writer::Text("B"), Writer::Text("C"), Writer::Text("D")}, "A##B##C##D"));
    // lines->add_child(
    //     Writer::Define("GOC_BODY_COMBINE", {Writer::Text("A"), Writer::Text("B"), Writer::Text("C"),
    //     Writer::Text("D")},
    //         "GOC_BODY_COMBINE_INNER(A, B, C, D)"));
    // auto generated_content = "GOC_BODY_COMBINE(GOC_GENERATED_, __LINE__, _, GOC_FILE_ID)";
    // lines->add_child(Writer::Define("GODOT_GENERATED_BODY", {Writer::Text("...")}, generated_content));
    // lines->add_child(Writer::Define("GODOT_GENERATED_GLOBAL", {Writer::Text("...")}, generated_content));
    //
    // lines->add_child(Writer::Enum("Hint", Writer::Text("_VAL")));
    // lines->add_child(Writer::Enum("GetSet", Writer::Text("_VAL")));
    //
    // for (const String& hint_name : property_hint_names) {
    //   String function_name = macro_case_to_pascal_case(string_replace(hint_name, "PROPERTY", ""));
    //
    //   lines->add_child(Writer::FuncImpl("constexpr", "Hint", function_name,
    //       {Writer::Param("const char*", "hint_string", Writer::StringLiteral(""))}, "",
    //       {Writer::Return("Hint::_VAL")}));
    // }
    //
    // for (const String& usage_name : property_usage_names) {
    //   String short_name = macro_case_to_pascal_case(string_replace(usage_name, "PROPERTY_", ""));
    //   lines->add_child(Writer::DeclAssign("constexpr PropertyUsageFlags", short_name, Writer::Text(usage_name)));
    // }
    //
    // Vector<Vector<Size>> subsets = find_all_subsets(godot_property_prototype_args.size());
    //
    // for (Vector<Size>& indices : subsets) {
    //   do {
    //     Writer::ListNode* params = Writer::Params({});
    //     for (Size& index : indices) {
    //       params->add_child(Writer::Text(godot_property_prototype_args[index]));
    //     }
    //     lines->add_child(
    //         Writer::FuncImpl("constexpr", "bool", "godot_property_prototype", {params}, "",
    //         {Writer::Return("true")}));
    //   } while (next_permutation(indices.begin(), indices.end()));
    // }
    //
    // result->add_child(Writer::Namespace("GodotObjectCompiler", Writer::Namespace("Generated", lines)));
    //
    // result->add_child(Writer::NewLine());
    // result->add_child(Writer::MacroFunctionDefine("GODOT_PROPERTY", {Writer::Text("...")},
    //     {Writer::LineOfCode({Writer::Text("static_assert(_godot_property_prototype(__VA_ARGS__))")})}));
    //
    // return result;
  }

}  // namespace GodotObjectCompiler