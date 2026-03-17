/**************************************************************************/
/* generate_bindings.cpp                                                  */
/*                        ___  ___  ___   ___ _____                       */
/*                       / __|/ _ \|   \ / _ \_   _|                      */
/*                      | (_ | (_) | |) | (_) || |                        */
/*                       \___|\___/|___/ \___/ |_|                        */
/*   ___  ___    _ ___ ___ _____    ___ ___  __  __ ___ ___ _    ___ ___  */
/*  / _ \| _ )_ | | __/ __|_   _|  / __/ _ \|  \/  | _ \_ _| |  | __| _ \ */
/* | (_) | _ \ || | _| (__  | |   | (_| (_) | |\/| |  _/| || |__| _||   / */
/*  \___/|___/\__/|___\___| |_|    \___\___/|_|  |_|_| |___|____|___|_|_\ */
/*                                                                        */
/*              This file is part of Godot Object Compiler                */
/*                  Copyright (c) 2026 Luca Ian Tuerk                     */
/**************************************************************************/
/*                            MIT LICENCE                                 */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/
#include "generate_bindings.h"

#include "library/attribute_db.h"
#include "library/core/file_system_utilities.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"
#include "library/execution_context.h"
#include "library/generator/generator.h"
#include "library/parser/parser.h"
#include "library/tree/output/output_transformator.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/namespace.h"
#include "library_godot/assumptions.h"
#include "library_godot/attributes/godot_attributes.h"
#include "library_godot/generators/godot_class_generator.h"
#include "library_godot/generators/godot_generator_utils.h"
#include "library_godot/generators/godot_macro_include_generator.h"

namespace GodotObjectCompiler {

String GenerateBindings::file_id(const String& p_file_name) {
  Hasher<String> hasher;
  return hash_string(hasher(p_file_name));
}

String GenerateBindings::generated_macro_name(const String& p_header,
                                              Size p_line) {
  StreamWriter stream;
  stream.write("GOC_GENERATED_");
  stream.write_generic(p_line);
  stream.write("_");
  stream.write(file_id(p_header));
  return stream.get_string();
}

Ref<ProgramError> GenerateBindings::run(ApplicationContext& p_context) {
  PROG_ERR_COND(!(AssumedGodotTypes::validate_assumptions() &&
                  AssumedParameterValues::validate_assumptions()),
                "Failed to validate some assumptions on available Godot "
                "types and macros, probably because the TypeDB "
                "generator has not found the relevant files.\n"
                "Ensure godot-cpp include path are known to goc via the -I= "
                "flag or in the .goc_project file.");
  PROG_ERR_COND(!p_context.paths_root.has_value(),
                "No project root path specified. Cannot generate bindings.");
  PROG_ERR_COND(!p_context.files_input.has_value(),
                "No input files specified. Cannot generate bindings.");

  OutputTransformator transformator;

  GodotMacroIncludeGenerator macro_include_generator;
  Ref<Context> macro_include_content = node_new<Context>();
  macro_include_generator.generate(nullptr, macro_include_content);

  FileWriter marco_writer = FileWriter::generated(
      path_concat(p_context.paths_generated, "godot_object_compiler/macros.h"),
      "");
  Ref<Output::OutputNode> macro_output =
      transformator.transform(macro_include_content);
  macro_output->get_output(&marco_writer);

  Ref<Context> register_types_header = node_new<Context>();
  Ref<Context> register_types_source = node_new<Context>();
  Ref<Context> register_class_includes = node_new<Context>();

  String register_method_name = "generated_register_module";
  String unregister_method_name = "generated_unregister_module";
  String register_file_name = "generated_register_types";
  Vector<String> registered_classes_headers;

  register_types_header->add_child(Output::PragmaOnce());

  switch (p_context.project_target) {
    case TARGET_GDEXTENSION:
      register_types_header->add_children({
          Output::Include("godot_cpp/godot.hpp"),
          Output::Text("using namespace godot;"),
      });
      register_types_source->add_children(
          {Output::Include("gdextension_interface.h"),
           Output::Include("godot_cpp/core/class_db.hpp"),
           Output::Include("godot_cpp/core/defs.hpp")});
      break;
    case TARGET_MODULE:
      // This should not be reachable as project target can currently not be
      // changed. If implemented the above godot-cpp includes need to be
      // changed to godot internal includes for this case
      PANIC("UNIMPLEMENTED");
      break;
  }

  register_types_header->add_children(
      {Output::NewLine(),
       build<Function>()
           .with_children(
               {build<Type>().with_child<Identifier>("void"),
                build<Identifier>(register_method_name),
                build<Parameters>().with_child(build<Parameter>().with_children(
                    {build<Type>().with_child<Identifier>(
                         AssumedGodotTypes::ModuleInitializationLevel()
                             .type->qualified_name()),
                     build<Identifier>("p_level")}))})
           .with_child(Output::Semicolon()),
       build<Function>()
           .with_children(
               {build<Type>().with_child<Identifier>("void"),
                build<Identifier>(unregister_method_name),
                build<Parameters>().with_child(build<Parameter>().with_children(
                    {build<Type>().with_child<Identifier>(
                         AssumedGodotTypes::ModuleInitializationLevel()
                             .type->qualified_name()),
                     build<Identifier>("p_level")}))})
           .with_child(Output::Semicolon())});

  Ref<Body> register_body;
  Ref<Body> unregister_body;

  register_types_source->add_children(
      {Output::Include(format("%s.h", register_file_name.c_str())),
       register_class_includes, Output::NewLine(),
       build<Function>().with_children(
           {build<Type>().with_child<Identifier>("void"),
            build<Identifier>(register_method_name),
            build<Parameters>().with_child(build<Parameter>().with_children(
                {build<Type>().with_child<Identifier>(
                     AssumedGodotTypes::ModuleInitializationLevel()
                         .type->qualified_name()),
                 build<Identifier>("p_level")})),
            build_ref<Body>(&register_body)}),
       build<Function>().with_children(
           {build<Type>().with_child<Identifier>("void"),
            build<Identifier>(unregister_method_name),
            build<Parameters>().with_child(build<Parameter>().with_children(
                {build<Type>().with_child<Identifier>(
                     AssumedGodotTypes::ModuleInitializationLevel()
                         .type->qualified_name()),
                 build<Identifier>("p_level")})),
            build_ref<Body>(&unregister_body)})});

  HashSet<String> processed;
  HashSet<String> register_includes;

  for (String input_file : *p_context.files_input) {
    if (!path_is_descendant(*p_context.paths_root, input_file)) {
      PRINT_INFO("Input file \"%s\" is not in the root path. Skipping.",
                 input_file.c_str())
      continue;
    }

    if (string_suffix(input_file, ".cpp")) {
      String h_file = input_file.substr(0, input_file.size() - 3) + "h";
      String hpp_file = input_file.substr(0, input_file.size() - 3) + "hpp";
      bool h_exists = file_exists(h_file);
      bool hpp_exists = file_exists(hpp_file);
      if (!h_exists && !hpp_exists) {
        PRINT_VERBOSE("No header found for input file \"%s\". Skipping",
                      input_file.c_str());
        continue;
      }
      if (h_exists) {
        input_file = h_file;
      } else {
        input_file = hpp_file;
      }
    }

    if (!string_suffix(input_file, ".h") &&
        !string_suffix(input_file, ".hpp")) {
      continue;
    }

    if (processed.find(input_file) != processed.end()) {
      continue;
    }
    processed.insert(input_file);

    PRINT_VERBOSE("Generating bindings for \"%s\"", input_file.c_str());
    TreeSitterParser parser;
    Ref<Namespace> global_namespace = node_new<Namespace>();
    Ref<ParserError> error = parser.parse_file(input_file, global_namespace);
    PROG_ERR_COND(error != ParserError::OK, "Failed to parse input file \"%s\"",
                  input_file.c_str());

    String relative_path = path_relative(input_file, *p_context.paths_root);
    String in_generated_path =
        path_concat(p_context.paths_generated, relative_path);
    String in_generated_base = path_base(in_generated_path);
    String in_generated_stem = path_stem(in_generated_path);
    String gen_source_path =
        path_concat_ext(in_generated_base, in_generated_stem, "generated.cpp");
    String gen_header_path =
        path_concat_ext(in_generated_base, in_generated_stem, "generated.h");
    String gen_header_include_path =
        header_path(p_context.paths_generated, gen_header_path);

    if (!directory_exits(in_generated_base)) {
      create_dir_recursive(in_generated_base);
    }

    Vector<Ref<GeneratedGlobalAttribute>> generated_global_attributes =
        global_namespace->body()->find_children<GeneratedGlobalAttribute>();
    PROG_ERR_COND(generated_global_attributes.size() > 1,
                  "Multiple GODOT_GENERATED_GLOBAL attributes found in "
                  "file, only on is required and allowed.");
    Ref<GeneratedGlobalAttribute> generated_global_attribute =
        generated_global_attributes.empty() ? nullptr
                                            : generated_global_attributes[0];

    Vector<Ref<Class>> classes = global_namespace->classes_recursive();
    Vector<Pair<Ref<GeneratedBodyAttribute>, Ref<Context>>> generated_bodies;

    Ref<Context> initialize = node_new<Context>();
    Ref<Context> uninitialize = node_new<Context>();
    Ref<Context> startup = node_new<Context>();
    Ref<Context> shutdown = node_new<Context>();
    Ref<Context> global_generated = node_new<Context>();
    HashSet<String> header_includes;
    HashSet<String> source_includes;

    Vector<ClassGeneratorResult> generate_results;

    for (const Ref<Class>& target_class : classes) {
      PRINT_VERBOSE("Processing class \"%s\"",
                    target_class->qualified_name().c_str());
      ClassGeneratorResult result{input_file, target_class, header_includes,
                                  source_includes, register_includes};
      result.initialize = register_body;
      result.uninitialize = unregister_body;
      result.generated_global = global_generated;
      result.generated_header_include_path = gen_header_include_path;

      for (const Ref<Type>& type : target_class->find_children<Type>(true)) {
        String header;
        if (GodotGeneratorUtils::get_type_header(type, target_class, header)) {
          source_includes.insert(header);
        }
      }

      Ref<Node> previous = target_class->get_previous_sibling();
      if (!previous) {
        PRINT_VERBOSE(
            "No previous sibling found, class cannot have a "
            "GodotClassAttribute applied. Skipping class.");
        continue;
      }

      Ref<GodotClassAttribute> class_attribute =
          previous->as<GodotClassAttribute>();
      if (!class_attribute) {
        PRINT_VERBOSE(
            "Class does not have a GodotClassAttribute "
            "applied. Skipping class.");
        continue;
      }

      auto generated_body_attribute =
          target_class->body()->find_child<GeneratedBodyAttribute>();
      PROG_ERR_COND(!generated_body_attribute ||
                        generated_body_attribute->get_index() != 0,
                    "Generated class requires a GODOT_GENERATED_BODY "
                    "attribute as first entry in the class body.");

      result.generated_body_line = generated_body_attribute->line;
      result.generated_sources->add_child(Output::NewLine());

      PROG_ERR_COND(!generated_global_attribute,
                    "File must contain a GODOT_GENERATED_GLOBAL "
                    "attribute in the global namespace.");

      GodotClassGenerator class_generator;
      Ref<Context> class_default_values = node_new<Context>();
      Ref<GeneratorError> class_def_gen_error =
          class_generator.generate_default_attribute_arguments(
              target_class, class_attribute, class_default_values);
      PROG_ERR_COND(class_def_gen_error != GeneratorError::OK,
                    "Failed to generate default attribute arguments.");

      ClassGenerator::merge_default_attribute_arguments(class_attribute,
                                                        class_default_values);

      Ref<GeneratorError> class_gen_error =
          class_generator.generate(target_class, class_attribute, result);

      PROG_ERR_COND(class_gen_error != GeneratorError::OK,
                    "Failed to generate class.");

      Ref<GeneratorError> init_gen_error =
          class_generator.generate_initialization(target_class, class_attribute,
                                                  result.initialize,
                                                  result.uninitialize);

      PROG_ERR_COND(init_gen_error != GeneratorError::OK,
                    "Failed to generate class initialization code.")

      if (result.initialize->get_child_count() > 0 ||
          result.uninitialize->get_child_count() > 0) {
        result.register_includes.insert(
            header_path(*p_context.paths_root, input_file));
      }

      if (!ExecutionContext::instance()->file_modified(input_file)) {
        PRINT_VERBOSE(
            "Input file \"%s\" was not modified since last "
            "read. Skipping.",
            input_file.c_str());
        continue;
      }

      for (const Ref<Node>& child : *target_class->body()) {
        if (Ref<Attribute> attribute = child->as<Attribute>()) {
          for (const Ref<ClassGenerator>& generator :
               ExecutionContext::instance()
                   ->get_attribute_db()
                   ->class_generators()) {
            if (generator->handles(target_class, attribute)) {
              Ref<Context> default_values = node_new<Context>();
              Ref<GeneratorError> attr_def_error =
                  generator->generate_default_attribute_arguments(
                      target_class, attribute, default_values);

              PROG_ERR_COND(attr_def_error != GeneratorError::OK,
                            "Failed to generate default "
                            "attribute arguments.");

              ClassGenerator::merge_default_attribute_arguments(attribute,
                                                                default_values);

              Ref<GeneratorError> attr_error =
                  generator->generate(target_class, attribute, result);

              PROG_ERR_COND(attr_error, "Failed to generate attribute code.");
            }
          }
        }
      }
      generate_results.push_back(result);
    }

    if (!ExecutionContext::instance()->file_modified(input_file)) {
      PRINT_VERBOSE(
          "Input file \"%s\" was not modified since last read. Skipping.",
          input_file.c_str());
      continue;
    }

    FileWriter source_writer =
        FileWriter::generated(gen_source_path, input_file);
    FileWriter header_writer =
        FileWriter::generated(gen_header_path, input_file);

    auto target_header = header_path(*p_context.paths_root, input_file);
    Output::Lines({Output::PragmaOnce(), Output::Text("#undef GOC_FILE_ID"),
                   Output::Define("GOC_FILE_ID", {}, file_id(target_header)),
                   Output::Include("godot_object_compiler/macros.h"),
                   Output::NewLine()})
        ->get_output(&header_writer);

    Output::Lines({Output::Include(target_header), Output::NewLine()})
        ->get_output(&source_writer);

    for (const String& include : header_includes) {
      Output::Include(include)->get_output(&header_writer);
      header_writer.write("\n");
    }

    for (const String& include : source_includes) {
      Output::Include(include)->get_output(&source_writer);
      source_writer.write("\n");
    }

    for (ClassGeneratorResult& result : generate_results) {
      Ref<Output::OutputNode> source_output =
          transformator.transform(result.generated_sources);

      Ref<Output::OutputNode> body_output = Output::Define(
          generated_macro_name(target_header, result.generated_body_line), {},
          {result.generated_body});

      source_output->get_output(&source_writer);
      body_output->get_output(&header_writer);
    }

    Ref<Output::OutputNode> global_output = Output::Define(
        generated_macro_name(
            target_header,
            generated_global_attribute ? generated_global_attribute->line : 0),
        {}, {global_generated});

    header_writer.write("\n");
    global_output->get_output(&header_writer);
  }

  for (const String& register_include : register_includes) {
    register_class_includes->add_child(Output::Include(register_include));
  }

  Ref<Output::OutputNode> register_header_output =
      transformator.transform(register_types_header);
  Ref<Output::OutputNode> register_source_output =
      transformator.transform(register_types_source);

  FileWriter register_header_writer =
      FileWriter::generated(path_concat_ext(p_context.paths_generated,
                                            "generated_register_types", "h"),
                            "");
  FileWriter register_source_writer =
      FileWriter::generated(path_concat_ext(p_context.paths_generated,
                                            "generated_register_types", "cpp"),
                            "");

  register_header_output->get_output(&register_header_writer);
  register_source_output->get_output(&register_source_writer);

  return ProgramError::OK;
}

}  // namespace GodotObjectCompiler
