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
  // Vector<String> split_vec =
  //     IAttributeArgumentParser::split_arguments("Hint(\"abc,\"), Property(1,2,3,6,,asdiaidkasd),");
  // for (const String& split : split_vec) {
  //   print_ln(split);
  //   auto inner = IAttributeArgumentParser::get_inner_arguments(split);
  //   print_ln(inner);
  //
  //   for (const String& split2 : IAttributeArgumentParser::split_arguments(inner)) {
  //     print_ln(split2);
  //   }
  // }
  //
  // return 0;
  Vector<String> paths = {
      "/home/luca/Repositories/godot-object-compiler/test_files/"
      "simple_class_header.h"};

  Dictionary<Size, String> parameters;
  print_ln(TreeSitterParser::strip_known_macro_contents(
      read_file("/home/luca/Repositories/godot-object-compiler/test_files/simple_class_header.h"), parameters));

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

  //
  // Ref<Context> ns = node_new<Namespace>();
  // ns->create_child<Identifier>("A");
  // Ref<Body> body = ns->create_child<Body>();
  // Ref<Function> func = body->create_child<Function>();
  // Ref<Type> type = func->create_child<Type>();
  // type->create_child<Identifier>("void");
  // func->create_child<Identifier>("Class::set_value");
  // Ref<Parameters> params = func->create_child<Parameters>();
  // Ref<Parameter> param = params->create_child<Parameter>();
  // Ref<Type> param_type = param->create_child<Type>();
  // param_type->create_child<Const>();
  // param_type->create_child<Identifier>("Ref<Some>");
  // param_type->create_child<Reference>();
  // param->create_child<Identifier>("p_value");
  // Ref<Body> func_body = func->create_child<Body>();
  // func_body->add_child(Writer::Text("value = p_value;\n"));
  //
  // Ref<Function> get = body->create_child<Function>();
  // Ref<Type> type_get = get->create_child<Type>();
  // type_get->create_child<Identifier>("Ref<Some>");
  // get->create_child<Identifier>("Class::get_value");
  // Ref<Parameters> params_get = get->create_child<Parameters>();
  // Ref<Body> get_body = get->create_child<Body>();
  // get_body->add_child(Writer::Return("value"));
  //
  // OutputTransformator transformator;
  // Ref<Writer::IOutputNode> output = transformator.transform(ns);
  // StreamWriter writer;
  // output->get_output(&writer);
  // print_ln(writer.get_string());
  //
  // return 0;

  for (const String& path : paths) {
    // Timer timer{"Handling " + path};
    // ExecutionContext* context = ExecutionContext::instance();
    // context->init();
    // context->set_remove_macros(read_lines(".goc/macro_remove.txt"));
    // context->set_include_paths(read_lines(".goc/include_paths.txt"));
    //
    // TreeSitterParser* parser = new TreeSitterParser();
    //
    // Ref<Namespace> ns = nullptr;
    // {
    //   Timer parse_timer{"Parse \"" + path + "\""};
    //   ns = parser->parse(read_file(path))->as<Namespace>();
    // }
    //
    // Ref<Namespace> target = default_construct<Namespace>()->as<Namespace>();
    // {
    //   Timer merge_timer{"Merge Includes \"" + path + "\""};
    //   // ns->merge_includes(target);
    // }
    //
    // // GodotGeneratedClassGenerator generator;
    // //
    // // for (Ref<Class> _class : BranchIterator<Class>(ns, BFS)) {
    // //   print_ln(_class->name());
    // // }
    //
    // Vector<Ref<Class>> classes = ns->classes_recursive();
    // for (Ref<Class> cl : classes) {
    //   Ref<Context> generated = generator.generate(ns, cl);
    //   for (Ref<Node> child : *generated) {
    //     if (OutputFile* output = child->as<OutputFile>()) {
    //       output->print();
    //     }
    //   }
    // }
  }

  return 0;
};

//
// {
//   Timer search_timer{"Find A"};
//   Ref<Class> A = target->find_descendant<Class>(BFS, NamedContextPredicates::name<Class>("A"));
//   if (A) {
//     auto bases = A->base_classes();
//     for (auto base : bases) {
//       print(base->qualified_name() + " ");
//     }
//     print_ln("");
//   }
// }
//
// {
//   Timer search_timer{"Find C"};
//   Ref<Class> C = target->find_descendant<Class>(BFS, NamedContextPredicates::name<Class>("C"));
//   if (C) {
//     auto bases = C->base_classes();
//     for (auto base : bases) {
//       print(base->qualified_name() + " ");
//     }
//     print_ln("");
//   }
// }
