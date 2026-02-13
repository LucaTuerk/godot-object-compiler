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

  String GenerateBindings::generated_macro_name(const String& p_file, Size p_line) {
    StreamWriter stream;
    stream.write("GOC_GENERATED_");
    stream.write_generic(p_line);
    stream.write("_");
    stream.write(file_id(p_file));
    return stream.get_string();
  }

  Ref<ProgramError> GenerateBindings::run(ApplicationContext& p_context) {
    if (!AssumedGodotTypes::validate_assumptions() || !AssumedParameterValues::validate_assumptions()) {
      return node_new<ProgramError>(ERROR,
          "Failed to validate some assumptions on available Godot types and macros, probably because the TypeDB "
          "generator has not found the relevant files.\n\n"
          "For modules please add the godot source to the GOC projects godot include paths.\n"
          "For gdextensions please add the godot-cpp generated includes to the GOC projects godot include paths.\n");
    }
    OutputTransformator transformator;

    GodotMacroIncludeGenerator macro_include_generator;
    Ref<Context> macro_include_content = node_new<Context>();
    macro_include_generator.generate(nullptr, macro_include_content);

    FileWriter marco_writer = FileWriter::generated(path_concat(p_context.paths_generated, "macros.h"), "");
    Ref<Writer::IOutputNode> macro_output = transformator.transform(macro_include_content);
    macro_output->get_output(&marco_writer);

    Ref<Context> register_types_header = node_new<Context>();
    Ref<Context> register_types_source = node_new<Context>();
    Ref<Context> register_class_includes = node_new<Context>();

    String register_method_name = "generated_register_module";
    String unregister_method_name = "generated_unregister_module";
    String register_file_name = "generated_register_types";
    Vector<String> registered_classes_headers;

    register_types_header->add_child(Writer::PragmaOnce());

    switch (p_context.project_target) {
      case TARGET_MODULE:
        return node_new<ProgramError>(ERROR, "Unimplemented");
        break;
      case TARGET_GDEXTENSION:
        register_types_header->add_children({
            Writer::SystemInclude("godot_cpp/godot.hpp"),
            Writer::Text("using namespace godot;"),
        });
        register_types_source->add_children({Writer::SystemInclude("gdextension_interface.h"),
            Writer::SystemInclude("godot_cpp/core/class_db.hpp"), Writer::SystemInclude("godot_cpp/core/defs.hpp")});
        break;
    }

    // clang-format off
    register_types_header->add_children({
      Writer::NewLine(),
      build<Function>().with_children({
        build<Type>().with_child<Identifier>("void"),
        build<Identifier>(register_method_name),
          build<Parameters>()
        .with_child(
            build<Parameter>().with_children({
              build<Type>().with_child<Identifier>(AssumedGodotTypes::ModuleInitializationLevel().qualified_name),
              build<Identifier>("p_level")
            })
          )
      }).with_child(Writer::Semicolon()),
      build<Function>().with_children({
        build<Type>().with_child<Identifier>("void"),
        build<Identifier>(unregister_method_name),
          build<Parameters>()
        .with_child(
            build<Parameter>().with_children({
              build<Type>().with_child<Identifier>(AssumedGodotTypes::ModuleInitializationLevel().qualified_name),
              build<Identifier>("p_level")
            })
          )
      }).with_child(Writer::Semicolon())
    });

    Ref<Body> register_body;
    Ref<Body> unregister_body;

    register_types_source->add_children({
      Writer::Include(path_concat_ext(p_context.paths_generated, register_file_name, "h")),
      register_class_includes,
      Writer::NewLine(),
      build<Function>().with_children({
        build<Type>().with_child<Identifier>("void"),
        build<Identifier>(register_method_name),
          build<Parameters>().with_child(
            build<Parameter>().with_children({
              build<Type>().with_child<Identifier>(AssumedGodotTypes::ModuleInitializationLevel().qualified_name),
              build<Identifier>("p_level")
            })
          ),
        build_ref<Body>(&register_body)
      }),
      build<Function>().with_children({
        build<Type>().with_child<Identifier>("void"),
        build<Identifier>(unregister_method_name),
          build<Parameters>().with_child(
            build<Parameter>().with_children({
              build<Type>().with_child<Identifier>(AssumedGodotTypes::ModuleInitializationLevel().qualified_name),
              build<Identifier>("p_level")
            })
          ),
        build_ref<Body>(&unregister_body)
      })
    });
    // clang-format on

    for (const String& input_file : p_context.files_input) {
      if (!string_suffix(input_file, ".h") && !string_suffix(input_file, ".hpp")) {
        continue;
      }

      TreeSitterParser parser;
      Ref<Namespace> global_namespace = node_new<Namespace>();
      Ref<ParserError> error = parser.parse_file(input_file, global_namespace);

      if (error != ParserError::OK) {
        continue;
      }

      String relative_path = path_relative(input_file, p_context.paths_root);

      String in_generated_path = path_concat(p_context.paths_generated, relative_path);
      String in_generated_base = path_base(in_generated_path);
      String in_generated_stem = path_stem(in_generated_path);
      String source_path = path_concat_ext(in_generated_base, in_generated_stem, "generated.cpp");
      String generated_path = path_concat_ext(in_generated_base, in_generated_stem, "generated.h");

      if (!directory_exits(in_generated_base)) {
        create_dir_recursive(in_generated_base);
      }

      Ref<GeneratedGlobalAttribute> generated_global_attribute =
          global_namespace->find_descendant<GodotGeneratedGlobalAttribute>();

      Vector<Ref<Class>> classes = global_namespace->classes_recursive();
      Vector<Pair<Ref<GeneratedBodyAttribute>, Ref<Context>>> generated_bodies;

      struct Results {
        String file_path;
        Ref<Class> target_class;
        Size generated_body_line;
        Size generated_global_line;

        Ref<Context> generated_body;
        Ref<Context> generated_source;
        Ref<Context> generated_global;

        Ref<Context> initialize;
        Ref<Context> uninitialize;
        Ref<Context> startup;
        Ref<Context> shutdown;
      };

      Ref<Context> global_generated = node_new<Context>();

      Vector<Results> generate_results;

      for (const Ref<Class>& target_class : classes) {
        Results results;
        results.file_path = input_file;
        results.target_class = target_class;

        results.generated_global = global_generated;
        results.generated_body = node_new<Context>();
        results.generated_source = node_new<Context>();
        results.initialize = node_new<Context>();
        results.uninitialize = node_new<Context>();
        results.startup = node_new<Context>();
        results.shutdown = node_new<Context>();

        auto generated_body_attribute = target_class->body()->find_child<GeneratedBodyAttribute>();
        if (!generated_body_attribute) {
          continue;
        }

        results.generated_body_line = generated_body_attribute->line;
        results.generated_source->add_child(Writer::NewLine());

        Ref<Node> previous = target_class->get_previous_sibling();
        if (!previous) {
          continue;
        }

        Ref<GodotClassAttribute> class_attribute = previous->as<GodotClassAttribute>();
        if (!class_attribute) {
          continue;
        }

        GodotClassGenerator class_generator;
        Ref<Context> class_default_values = node_new<Context>();
        Ref<GeneratorError> class_def_gen_error =
            class_generator.generate_default_attribute_arguments(target_class, class_attribute, class_default_values);
        if (class_def_gen_error != GeneratorError::OK) {
          continue;
        }
        ClassGenerator::merge_default_attribute_arguments(class_attribute, class_default_values);

        Ref<GeneratorError> class_gen_error = class_generator.generate(
            target_class, class_attribute, results.generated_body, results.generated_source, results.generated_global);
        if (class_gen_error != GeneratorError::OK) {
          continue;
        }

        Ref<GeneratorError> init_gen_error = class_generator.generate_initialization(
            target_class, class_attribute, results.initialize, results.uninitialize);
        if (init_gen_error != GeneratorError::OK) {
          continue;
        }

        if (results.initialize->get_child_count() > 0 || results.uninitialize->get_child_count() > 0) {
          register_class_includes->add_child(Writer::Include(input_file));
          register_body->add_child(results.initialize);
          unregister_body->add_child(results.uninitialize);
        }

        Ref<GeneratorError> start_gen_error =
            class_generator.generate_startup(target_class, class_attribute, results.startup, results.shutdown);
        if (start_gen_error != GeneratorError::OK) {
          continue;
        }

        if (!ExecutionContext::instance()->file_modified(input_file)) {
          continue;
        }

        for (const Ref<Node>& child : *target_class->body()) {
          if (Ref<Attribute> attribute = child->as<Attribute>()) {
            for (const Ref<ClassGenerator>& generator :
                ExecutionContext::instance()->get_attribute_db()->class_generators()) {
              if (generator->handles(target_class, attribute)) {
                Ref<Context> default_values = node_new<Context>();
                Ref<GeneratorError> attr_def_error =
                    generator->generate_default_attribute_arguments(target_class, attribute, default_values);

                if (attr_def_error) {
                  continue;
                }

                ClassGenerator::merge_default_attribute_arguments(attribute, default_values);

                Ref<GeneratorError> attr_error = generator->generate(target_class, attribute, results.generated_body,
                    results.generated_source, results.generated_global);
              }
            }
          }
        }
        generate_results.push_back(results);
      }

      if (!ExecutionContext::instance()->file_modified(input_file)) {
        continue;
      }

      FileWriter source_writer = FileWriter::generated(source_path, input_file);
      FileWriter generated_writer = FileWriter::generated(generated_path, input_file);
      Writer::PragmaOnce()->get_output(&generated_writer);
      Writer::Text("#undef GOC_FILE_ID\n")->get_output(&generated_writer);
      Writer::Define("GOC_FILE_ID", {}, file_id(input_file))->get_output(&generated_writer);
      Writer::Include(input_file)->get_output(&source_writer);

      for (Results& result : generate_results) {
        Ref<Writer::IOutputNode> source_output = transformator.transform(result.generated_source);

        Ref<Writer::IOutputNode> body_output =
            Writer::Define(generated_macro_name(result.file_path, result.generated_body_line), {},
                {transformator.transform(result.generated_body)});

        source_output->get_output(&source_writer);
        body_output->get_output(&generated_writer);
      }

      Ref<Writer::IOutputNode> global_output = Writer::Define(
          generated_macro_name(input_file, generated_global_attribute ? generated_global_attribute->line : 0), {},
          {transformator.transform(global_generated)});

      generated_writer.write("\n");
      global_output->get_output(&generated_writer);
    }

    Ref<Writer::IOutputNode> register_header_output = transformator.transform(register_types_header);
    Ref<Writer::IOutputNode> register_source_output = transformator.transform(register_types_source);

    FileWriter register_header_writer =
        FileWriter::generated(path_concat_ext(p_context.paths_generated, "generated_register_types", "h"), "");
    FileWriter register_source_writer =
        FileWriter::generated(path_concat_ext(p_context.paths_generated, "generated_register_types", "cpp"), "");

    register_header_output->get_output(&register_header_writer);
    register_source_output->get_output(&register_source_writer);

    return ProgramError::OK;
  }

}
