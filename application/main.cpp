#include "main.h"

#include "../library/core/config.h"
#include "../library/type_db.h"
#include "../library_godot/generators/godot_macro_include_generator.h"
#include "application_context.h"
#include "library/core/core.h"
#include "library/core/helpers.h"
#include "library/core/string_writer.h"
#include "library/parser/attribute_argument_parser.h"
#include "library/parser/parser.h"
#include "library/tree/output/output_transformator.h"
#include "programs/generate.h"
#include "programs/generate_type_db.h"

using namespace GodotObjectCompiler;

int main() {
  Vector<String> paths = {
      "/home/luca/Repositories/godot-object-compiler/test_files/"
      "simple_class_header.h"};

  Dictionary<Size, String> parameters;

  ApplicationContext context;
  context.input_files = {"/home/luca/Repositories/godot-object-compiler/test_files/simple_class_header.h"};
  context.generated_root = "/home/luca/Repositories/godot-object-compiler/.generated";
  context.include_paths = read_lines("/home/luca/Repositories/godot-object-compiler/.goc/include_paths.txt");
  context.cache_root = ".goc/cache";

  ExecutionContext::instance()->set_error_level(ERROR, FULL);
  ExecutionContext::instance()->set_remove_macros(
      read_lines("/home/luca/Repositories/godot-object-compiler/.goc/macro_remove.txt"));
  ExecutionContext::instance()->set_include_paths(
      read_lines("/home/luca/Repositories/godot-object-compiler/.goc/include_paths.txt"));

  GenerateTypeDB generate_type_db;
  generate_type_db.run(context);

  GodotMacroIncludeGenerator macro_include_generator;
  Ref<Context> macro_include_content = node_new<Context>();
  macro_include_generator.generate(nullptr, macro_include_content);

  FileWriter marco_writer{path_concat(context.generated_root, "macros.h")};
  OutputTransformator transformator;
  Ref<Writer::IOutputNode> macro_output = transformator.transform(macro_include_content);
  macro_output->get_output(&marco_writer);

  Generate generate;
  generate.run(context);

  return 0;
};
