#include "generate_bindings.h"

#include "library/attribute_db.h"
#include "library/core/helpers.h"
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
#include "library_godot/generators/godot_macro_include_generator.h"

namespace GodotObjectCompiler {

  String GenerateBindings::file_id(const String& file_name) {
    Hasher<String> hasher;
    return hash_string(hasher(file_name));
  }

  String GenerateBindings::generated_macro_name(const String& file, Size line) {
    StreamWriter stream;
    stream.write("GOC_GENERATED_");
    stream.write_generic(line);
    stream.write("_");
    stream.write(file_id(file));
    return stream.get_string();
  }

  Ref<ProgramError> GenerateBindings::run(ApplicationContext& context) {
    OutputTransformator transformator;

    GodotMacroIncludeGenerator macro_include_generator;
    Ref<Context> macro_include_content = node_new<Context>();
    macro_include_generator.generate(nullptr, macro_include_content);

    FileWriter marco_writer{path_concat(context.paths_generated, "macros.h")};
    Ref<Writer::IOutputNode> macro_output = transformator.transform(macro_include_content);
    macro_output->get_output(&marco_writer);

    Ref<Context> register_types_header = node_new<Context>();
    Ref<Context> register_types_source = node_new<Context>();
    Ref<Context> register_class_includes = node_new<Context>();

    String register_method_name = "generated_register_module";
    String unregister_method_name = "generated_unregister_module";
    String register_file_name = "generated_register_types";
    Vector<String> registered_classes_headers;

    // #include <gdextension_interface.h>
    // #include <godot_cpp/core/class_db.hpp>
    // #include <godot_cpp/core/defs.hpp>
    // #include <godot_cpp/godot.hpp>

    // clang-format off
    register_types_header->add_children({
      Writer::PragmaOnce(),
      // Writer::Include("modules/register_module_types.h"),
      // Writer::Include("core/object/class_db.h"),
      Writer::SystemInclude("godot_cpp/godot.hpp"),
      Writer::Text("using namespace godot;"),
      Writer::NewLine(),
      build<Function>().with_children({
        build<Type>().with_child<Identifier>("void"),
        build<Identifier>(register_method_name),
          build<Parameters>()
        .with_child(
            build<Parameter>().with_children({
              build<Type>().with_child<Identifier>(AssumedGodotTypes::ModuleInitializationLevel().type_name),
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
              build<Type>().with_child<Identifier>(AssumedGodotTypes::ModuleInitializationLevel().type_name),
              build<Identifier>("p_level")
            })
          )
      }).with_child(Writer::Semicolon())
    });

    Ref<Body> register_body;
    Ref<Body> unregister_body;

    register_types_source->add_children({
      Writer::SystemInclude("gdextension_interface.h"),
      Writer::SystemInclude("godot_cpp/core/class_db.hpp"),
      Writer::SystemInclude("godot_cpp/core/defs.hpp"),
      Writer::Include(path_concat_ext(context.paths_generated, register_file_name, "h")),
      register_class_includes,
      Writer::NewLine(),
      build<Function>().with_children({
        build<Type>().with_child<Identifier>("void"),
        build<Identifier>(register_method_name),
          build<Parameters>().with_child(
            build<Parameter>().with_children({
              build<Type>().with_child<Identifier>(AssumedGodotTypes::ModuleInitializationLevel().type_name),
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
              build<Type>().with_child<Identifier>(AssumedGodotTypes::ModuleInitializationLevel().type_name),
              build<Identifier>("p_level")
            })
          ),
        build_ref<Body>(&unregister_body)
      })
    });
    // clang-format on

    for (const String& input_file : context.files_input) {
      if (!string_suffix(input_file, ".h") && !string_suffix(input_file, ".hpp")) {
        continue;
      }

      TreeSitterParser parser;
      Ref<Namespace> global_namespace = node_new<Namespace>();
      Ref<ParserError> error = parser.parse_file(input_file, global_namespace);

      if (error != ParserError::OK) {
        continue;
      }

      String relative_path = path_relative(input_file, context.paths_root);

      String in_generated_path = path_concat(context.paths_generated, relative_path);
      String in_generated_base = path_base(in_generated_path);
      String in_generated_stem = path_stem(in_generated_path);
      String source_path = path_concat_ext(in_generated_base, in_generated_stem, "generated.cpp");
      String generated_path = path_concat_ext(in_generated_base, in_generated_stem, "generated.h");

      if (!dir_exists(in_generated_base)) {
        create_dir_recursive(in_generated_base);
      }

      FileWriter source_writer{source_path};
      FileWriter generated_writer{generated_path};

      Ref<GeneratedGlobalAttribute> generated_global_attribute =
          global_namespace->find_child<GodotGeneratedGlobalAttribute>();

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

      Writer::PragmaOnce()->get_output(&generated_writer);
      Writer::Text("#undef GOC_FILE_ID\n")->get_output(&generated_writer);
      Writer::Define("GOC_FILE_ID", {}, file_id(input_file))->get_output(&generated_writer);
      Writer::Include(input_file)->get_output(&source_writer);

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
        }

        Ref<GeneratorError> start_gen_error =
            class_generator.generate_startup(target_class, class_attribute, results.startup, results.shutdown);
        if (start_gen_error != GeneratorError::OK) {
          continue;
        }

        for (const Ref<Node>& child : *target_class->body()) {
          if (Ref<Attribute> attribute = child->as<Attribute>()) {
            for (const Ref<ClassGenerator>& generator : AttributeDB::instance()->class_generators()) {
              if (generator->handles(target_class, attribute)) {
                Ref<Context> default_values = node_new<Context>();
                Ref<GeneratorError> attr_def_error =
                    generator->generate_default_attribute_arguments(target_class, attribute, default_values);

                if (attr_def_error) {
                  continue;
                }

                ClassGenerator::merge_default_attribute_arguments(attribute, default_values);

                // print_ln(attribute->pretty_print());

                Ref<GeneratorError> attr_error = generator->generate(target_class, attribute, results.generated_body,
                    results.generated_source, results.generated_global);
              }
            }
          }
        }
        generate_results.push_back(results);
      }

      for (Results& result : generate_results) {
        register_body->add_child(result.initialize);
        unregister_body->add_child(result.uninitialize);

        Ref<Writer::IOutputNode> source_output = transformator.transform(result.generated_source);

        Ref<Writer::IOutputNode> body_output =
            Writer::Define(generated_macro_name(result.file_path, result.generated_body_line), {},
                {transformator.transform(result.generated_body)});

        source_output->get_output(&source_writer);
        body_output->get_output(&generated_writer);

        Ref<Writer::IOutputNode> global_output = Writer::Define(
            generated_macro_name(input_file, generated_global_attribute ? generated_global_attribute->line : 0), {},
            {transformator.transform(global_generated)});

        generated_writer.write("\n");
        global_output->get_output(&generated_writer);
      }
    }

    Ref<Writer::IOutputNode> register_header_output = transformator.transform(register_types_header);
    Ref<Writer::IOutputNode> register_source_output = transformator.transform(register_types_source);

    FileWriter register_header_writer(path_concat_ext(context.paths_generated, "generated_register_types", "h"));
    FileWriter register_source_writer(path_concat_ext(context.paths_generated, "generated_register_types", "cpp"));

    register_header_output->get_output(&register_header_writer);
    register_source_output->get_output(&register_source_writer);

    return ProgramError::OK;
  }

}  // namespace GodotObjectCompiler
