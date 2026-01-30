
#include "generate.h"

#include "library/attribute_db.h"
#include "library/core/helpers.h"
#include "library/core/string_writer.h"
#include "library/execution_context.h"
#include "library/generator/generator.h"
#include "library/parser/parser.h"
#include "library/tree/output/output_transformator.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/namespace.h"
#include "library_godot/attributes/godot_attributes.h"
#include "library_godot/generators/godot_class_generator.h"

namespace GodotObjectCompiler {

  String Generate::file_id(const String& file_name) {
    Hasher<String> hasher;
    return hash_string(hasher(file_name));
  }

  String Generate::generated_macro_name(const String& file, Size line) {
    StreamWriter stream;
    stream.write("GOC_GENERATED_");
    stream.write_generic(line);
    stream.write("_");
    stream.write(file_id(file));
    return stream.get_string();
  }

  int Generate::run(ApplicationContext& context) {
    ExecutionContext* execution_context = ExecutionContext::instance();
    execution_context->init();
    execution_context->set_remove_macros(read_lines(".goc/macro_remove.txt"));
    execution_context->set_include_paths(read_lines(".goc/include_paths.txt"));
    OutputTransformator transformator;

    for (const String& input_file : context.input_files) {
      TreeSitterParser parser;
      Ref<Namespace> global_namespace = node_new<Namespace>();
      Ref<ParserError> error = parser.parse_file(input_file, global_namespace);

      if (error != ParserError::OK) {
        continue;
      }

      String source_path = input_file + ".generated.cpp";
      String generated_path = input_file + ".generated.h";

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
      };

      Ref<Context> global_generated = node_new<Context>();

      Vector<Results> generate_results;

      Writer::PragmaOnce()->get_output(&generated_writer);
      Writer::Define("GOC_FILE_ID", {}, file_id(input_file))->get_output(&generated_writer);
      Writer::Include(input_file)->get_output(&source_writer);

      for (Ref<Class> target_class : classes) {
        Results results;
        results.file_path = input_file;
        results.target_class = target_class;
        results.generated_body = node_new<Context>();
        results.generated_source = node_new<Context>();
        results.generated_global = global_generated;

        auto generated_body_attribute = target_class->body()->find_child<GeneratedBodyAttribute>();
        if (!generated_body_attribute) {
          continue;
        }

        results.generated_body_line = generated_body_attribute->line;
        results.generated_source->add_child(Writer::NewLine());

        Ref<Node> previous = target_class->get_previous_sibling();
        if (!previous || !previous->is<GodotClassAttribute>()) {
          continue;
        }

        GodotClassGenerator class_generator;

        Ref<GeneratorError> error = class_generator.generate(target_class, previous->as<GodotClassAttribute>(),
            results.generated_body, results.generated_source, results.generated_global);

        if (error != GeneratorError::OK) {
          continue;
        }

        for (Ref<Node> child : *target_class->body()) {
          Ref<Attribute> attribute = child->as<Attribute>();

          if (attribute) {
            for (Ref<ClassGenerator> generator : AttributeDB::instance()->class_generators()) {
              if (generator->handles(target_class, attribute)) {
                Ref<GeneratorError> attr_error = generator->generate(target_class, attribute, results.generated_body,
                    results.generated_source, results.generated_global);
              }
            }
          }
        }
        generate_results.push_back(results);
      }

      for (Results& result : generate_results) {
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
    return 0;
  }

}  // namespace GodotObjectCompiler
