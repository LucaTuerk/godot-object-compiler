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
#include "project.h"

using namespace GodotObjectCompiler;

int main() {
  Resources::instance()->load_pack(&GOC_Resources::Pack);
  TypeDB::instance()->set_cache_directory(".goc/cache");

  Vector<String> paths = {
      "/home/luca/Repositories/godot-object-compiler/test_files/"
      "simple_class_header.h"};

  Project project;
  if (!project.read_from_file("./goc_project.conf")) {
    print_err("Could not find project file.");
  }

  ApplicationContext context;
  context.paths_root = path_absolute(project.paths_root);
  context.paths_generated = path_absolute(project.paths_generated);
  context.paths_cache = path_absolute(project.paths_cache);

  context.paths_include = project.paths_include;
  if (std::find(context.paths_include.begin(), context.paths_include.end(),project.paths_root) == context.paths_include.end()) {
    context.paths_include.push_back(project.paths_root);
  }
  context.files_input = directory_files_recursive(project.paths_root);

  std::transform(context.paths_include.begin(), context.paths_include.end(), context.paths_include.begin(), &path_absolute);
  std::transform(context.files_input.begin(), context.files_input.end(), context.files_input.begin(), &path_absolute);

  if (!context.valid()) {
    return 1;
  }

  ExecutionContext::instance()->set_error_level(ERROR, FULL);
  ExecutionContext::instance()->set_remove_macros(
      read_lines("/home/luca/Repositories/godot-object-compiler/.goc/macro_remove.txt"));
  ExecutionContext::instance()->set_include_paths(context.paths_include);

  // project.project_name = "test_files";
  //
  // project.godot_target_major_version = 4;
  // project.godot_target_minor_version = 6;
  //
  // project.paths_root = "/home/luca/Repositories/godot-object-compiler/test_files";
  // project.paths_generated = "/home/luca/Repositories/godot-object-compiler/.generated";
  // project.paths_cache = context.cache_root;
  // project.paths_include = context.include_paths;
  //
  // Config config;
  // project.write_to(&config);
  // config.write_to_file("./goc_project.conf");

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

  FileWriter marco_writer{path_concat(context.paths_generated, "macros.h")};
  OutputTransformator transformator;
  Ref<Writer::IOutputNode> macro_output = transformator.transform(macro_include_content);
  macro_output->get_output(&marco_writer);

  GenerateBindings generate;
  generate.run(context);

  return 0;
};
