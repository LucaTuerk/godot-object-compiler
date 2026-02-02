#include "main.h"

#include "application_context.h"
#include "compiled_resources/res.gen.h"
#include "library/core/core.h"
#include "library/core/helpers.h"
#include "library/core/resources.h"
#include "library/core/string_writer.h"
#include "library/parser/parser.h"
#include "library/tree/output/output_transformator.h"
#include "library/type_db.h"
#include "library_godot/assumptions.h"
#include "library_godot/generated_assumptions/parameter_types.h"
#include "library_godot/generators/godot_macro_include_generator.h"
#include "programs/generate_assumptions.h"
#include "programs/generate_bindings.h"
#include "programs/generate_type_db.h"

using namespace GodotObjectCompiler;

int main() {
  Resources::instance()->load_pack(&GOC_Resources::Pack);
  TypeDB::instance()->set_cache_directory(".goc/cache");

  Vector<String> paths = {
      "/home/luca/Repositories/godot-object-compiler/test_files/"
      "simple_class_header.h"};

  Dictionary<Size, String> parameters;
  String res = Resources::instance()->load_text_resource("res://doc/GOC_PropertyUsageFlags/UsageNilIsVariant.txt");

  ApplicationContext context;
  context.input_files = {"/home/luca/Repositories/godot-object-compiler/test_files/simple_class_header.h",
      "/home/luca/Repositories/godot-object-compiler/test_files/exported_fields.h"};
  context.generated_root = "/home/luca/Repositories/godot-object-compiler/.generated";
  context.include_paths = read_lines("/home/luca/Repositories/godot-object-compiler/.goc/include_paths.txt");
  context.cache_root = ".goc/cache";

  ExecutionContext::instance()->set_error_level(ERROR, FULL);
  ExecutionContext::instance()->set_remove_macros(
      read_lines("/home/luca/Repositories/godot-object-compiler/.goc/macro_remove.txt"));
  ExecutionContext::instance()->set_include_paths(
      read_lines("/home/luca/Repositories/godot-object-compiler/.goc/include_paths.txt"));
  //
  // TreeSitterParser parser;
  // Ref<Context> ns = node_new<Namespace>();
  // parser.parse_file("/home/luca/Repositories/godot-object-compiler/test_files/exported_fields.h", ns);
  // print_ln(ns->pretty_print());
  // return 0;
  //
  // return 0;
  //
  // Ref<Enum> variant_type = TypeDB::instance()->get_type_data<Enum>("Variant::Type");
  // if (variant_type) {
  //   for (const auto& value : variant_type->value_names()) {
  //     FileWriter writer(
  //         path_concat_ext("./resources/variant_types", "VariantType" + macro_case_to_pascal_case(value), "txt"));
  //
  //     writer.write(read_file(path_concat_ext("./resources/variant_types2", value, "txt")));
  //   }
  // }
  // print_ln(variant_type->pretty_print());

  GenerateTypeDB generate_type_db;
  generate_type_db.run(context);

  // if (!AssumedGodotTypes::validate_assumptions()) {
  //   return 1;
  // }
  //
  // if (!AssumedParameterValues::validate_assumptions()) {
  //   return 1;
  // }
  //
  // GenerateAssumptions generate_assumptions;
  // generate_assumptions.run(context);
  // return 0;

  GodotMacroIncludeGenerator macro_include_generator;
  Ref<Context> macro_include_content = node_new<Context>();
  macro_include_generator.generate(nullptr, macro_include_content);

  FileWriter marco_writer{path_concat(context.generated_root, "macros.h")};
  OutputTransformator transformator;
  Ref<Writer::IOutputNode> macro_output = transformator.transform(macro_include_content);
  macro_output->get_output(&marco_writer);

  GenerateBindings generate;
  generate.run(context);

  return 0;
};
