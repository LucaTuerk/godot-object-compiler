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

#include "application/arguments/argument_lists.h"
#include "application/arguments/argument_parsers.h"
#include "library/attribute_db.h"
#include "library/core/file_system_utilities.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"
#include "library/generator/generator.h"
#include "library/library_context.h"
#include "library/parser.h"
#include "library/tree/output/output_transformator.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/namespace.h"
#include "library_godot/assumptions.h"
#include "library_godot/attributes/godot_attributes.h"
#include "library_godot/generators/godot_class_generator.h"
#include "library_godot/generators/godot_generator_utils.h"
#include "library_godot/generators/godot_macro_include_generator.h"

namespace GodotObjectCompiler
{
    CommandLineArgumentParseResult
    GenerateBindings::register_required_arguments(ApplicationContext& p_context) const
    {
        return p_context.register_argument_lists<
            ApplicationArguments, GeneratorArguments, GDExtensionProjectArguments>();
    }

    String GenerateBindings::file_id(const String& p_file_name)
    {
        constexpr Hasher<String> hasher;
        return hash_string(hasher(String(p_file_name)));
    }

    Path GenerateBindings::cache_path(const Path& goc_path, const String& p_file_name)
    {
        const Path input_cache = goc_path / "input_cache";
        Path cache_path = Path(input_cache) / format("%s.gocdb", file_id(p_file_name).c_str());
        if (!directory_exits(input_cache)) {
            create_dir_recursive(input_cache);
        }
        return cache_path;
    }

    String GenerateBindings::generated_macro_name(const String& p_header, Size p_line)
    {
        StreamWriter stream;
        stream.write("GOC_GENERATED_");
        stream.write_generic(p_line);
        stream.write("_");
        stream.write(file_id(p_header));
        return stream.get_string();
    }

    Ref<ProgramError> GenerateBindings::execute(ApplicationContext& p_context)
    {
        auto application_args = p_context.get_argument_list<ApplicationArguments>();
        auto generator_args = p_context.get_argument_list<GeneratorArguments>();
        auto project_args = p_context.get_argument_list<GDExtensionProjectArguments>();

        PROG_ERR_COND(
            !(AssumedGodotTypes::validate_assumptions() &&
              AssumedParameterValues::validate_assumptions()),
            "Failed to validate some assumptions on available Godot types and macros. Supplied "
            "extension api files or godot-cpp include paths might be invalid.");

        OutputTransformator transformator;

        LibraryContext::instance()->add_include_paths(project_args->godot_cpp->get<Vector<Path>>());

        if (LibraryContext::instance()->file_modified(project_args->extension_api->get<Path>())) {
            GodotMacroIncludeGenerator macro_include_generator;
            Ref<Context> macro_include_content = node_new<Context>();
            Ref<Context> core_include_content = node_new<Context>();

            macro_include_generator.generate(nullptr, macro_include_content);
            macro_include_generator.generate_core_include(
                project_args->godot_cpp->get<Vector<Path>>(), core_include_content);

            FileWriter marco_writer = FileWriter::generated(

                generator_args->generated_path->get<Path>() / "godot_object_compiler/macros.h", "");
            Ref<Output::OutputNode> macro_output = transformator.transform(macro_include_content);
            macro_output->get_output(&marco_writer);

            FileWriter core_include_writer = FileWriter::generated(
                generator_args->generated_path->get<Path>() /
                    "godot_object_compiler/core_includes.h",
                "");
            Ref<Output::OutputNode> core_include_output =
                transformator.transform(core_include_content);
            core_include_output->get_output(&core_include_writer);
        }

        Ref<Context> register_types_header = node_new<Context>();
        Ref<Context> register_types_source = node_new<Context>();
        Ref<Context> register_class_includes = node_new<Context>();

        String register_method_name = "generated_register_module";
        String unregister_method_name = "generated_unregister_module";
        String register_file_name = "generated_register_types";
        Vector<String> registered_classes_headers;

        register_types_header->add_child(Output::PragmaOnce());
        register_types_header->add_children({
            Output::PragmaOnce(),
            Output::Include("godot_object_compiler/core_includes.h"),
            Output::Text("using namespace godot;"),
        });
        register_types_source->add_children(
            {Output::Include("godot_object_compiler/core_includes.h")});

        register_types_header->add_children(
            {Output::NewLine(),
             B<Function>()[{
                 B<Type>()[B<Identifier>("void")], B<Identifier>(register_method_name),
                 B<Parameters>()[B<Parameter>()[{
                     B<Type>()[B<Identifier>(
                         AssumedGodotTypes::ModuleInitializationLevel().type->qualified_name())],
                     B<Identifier>("p_level")}]]}][Output::Semicolon()],
             B<Function>()[{
                 B<Type>()[B<Identifier>("void")], B<Identifier>(unregister_method_name),
                 B<Parameters>()[B<Parameter>()[{
                     B<Type>()[B<Identifier>(
                         AssumedGodotTypes::ModuleInitializationLevel().type->qualified_name())],
                     B<Identifier>("p_level")}]]}][Output::Semicolon()]});

        Ref<Body> register_body;
        Ref<Body> unregister_body;

        register_types_source->add_children(
            {Output::Include(format("%s.h", register_file_name.c_str())), register_class_includes,
             Output::NewLine(),
             B<Function>()[{
                 B<Type>()[B<Identifier>("void")], B<Identifier>(register_method_name),
                 B<Parameters>()[B<Parameter>()[{
                     B<Type>()[B<Identifier>(
                         AssumedGodotTypes::ModuleInitializationLevel().type->qualified_name())],
                     B<Identifier>("p_level")}]],
                 R<Body>(&register_body)}],
             B<Function>()[{
                 B<Type>()[B<Identifier>("void")], B<Identifier>(unregister_method_name),
                 B<Parameters>()[B<Parameter>()[{
                     B<Type>()[B<Identifier>(
                         AssumedGodotTypes::ModuleInitializationLevel().type->qualified_name())],
                     B<Identifier>("p_level")}]],
                 R<Body>(&unregister_body)}]});

        HashSet<Path> processed;
        HashSet<String> register_includes;

        for (Path input_file : project_args->sources->get<Vector<Path>>()) {
            if (!path_is_descendant(generator_args->root_path->get<Path>(), input_file)) {
                PRINT_INFO(
                    "Input file \"%s\" is not in the root path. Skipping.", input_file.c_str())
                continue;
            }

            if (input_file.extension() == ".cpp") {
                Path h_file = input_file;
                h_file.replace_extension(".h");

                Path hpp_file = input_file;
                hpp_file.replace_extension(".hpp");

                bool h_exists = file_exists(h_file);
                bool hpp_exists = file_exists(hpp_file);
                if (!h_exists && !hpp_exists) {
                    PRINT_VERBOSE(
                        "No header found for input file \"%s\". Skipping", input_file.c_str());
                    continue;
                }
                if (h_exists) {
                    input_file = h_file;
                } else {
                    input_file = hpp_file;
                }
            }

            if (input_file.extension() != ".h" && input_file.extension() != ".hpp") {
                continue;
            }

            if (processed.find(input_file) != processed.end()) {
                continue;
            }

            processed.insert(input_file);
            Path relative_path = path_relative(input_file, generator_args->root_path->get<Path>());

            Ref<Namespace> global_namespace = nullptr;
            Path cached_path = cache_path(application_args->goc_path->get<Path>(), input_file);

            ConfigNodeReaderWriter reader_writer;
            if (!LibraryContext::instance()->file_modified(input_file) &&
                file_exists(cached_path)) {
                if (Result<Node> parsed = reader_writer.read_from_file(cached_path);
                    parsed.has_error()) {
                    parsed.get_error()->set_handled();
                    remove_file(cached_path);
                    fmt_print_err(
                        "Failed to get cached input file for \"%s\": %s", input_file.c_str(),
                        parsed.get_error()->message.c_str());
                } else if (!parsed.get_result()->is<Namespace>()) {
                    remove_file(cached_path);
                    fmt_print_err(
                        "Invalid node read from cached file \"%s\" for input file \"%s\"",
                        cached_path.c_str(), input_file.c_str());
                } else {
                    global_namespace = parsed.get_result()->as<Namespace>();
                }
            }

            if (global_namespace == nullptr) {
                Ref<IParser> parser =
                    LibraryContext::instance()->get_default_parser(IParser::SOURCE_PARSER);
                parser->config(IParser::CONFIG_PARSE_ATTRIBUTES);
                global_namespace = node_new<Namespace>();
                Ref<ParserError> error = parser->parse_file(input_file, global_namespace);
                PROG_ERR_COND(
                    error != ParserError::OK, "Failed to parse input file \"%s\"",
                    input_file.c_str());
                reader_writer.write_to_file(global_namespace, cached_path);
            }

            Path in_generated_path = generator_args->generated_path->get<Path>() / relative_path;
            Path in_generated_base = in_generated_path.parent_path();
            String in_generated_stem = in_generated_path.stem().generic_string();

            Path gen_source_path =
                in_generated_base / format("%s.generated.cpp", in_generated_stem.c_str());
            Path gen_header_path =
                in_generated_base / format("%s.generated.h", in_generated_stem.c_str());
            String gen_header_include_path =
                header_path(generator_args->generated_path->get<Path>(), gen_header_path);

            if (!directory_exits(in_generated_base)) {
                create_dir_recursive(in_generated_base);
            }

            Vector<Ref<GeneratedGlobalAttribute>> generated_global_attributes =
                global_namespace->body()->find_children<GeneratedGlobalAttribute>();
            PROG_ERR_COND(
                generated_global_attributes.size() > 1,
                "Multiple GODOT_GENERATED_GLOBAL attributes found in file, only on is required and "
                "allowed.");
            Ref<GeneratedGlobalAttribute> generated_global_attribute =
                generated_global_attributes.empty() ? nullptr : generated_global_attributes[0];

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
                PRINT_VERBOSE("Preprocessing class \"%s\"", target_class->qualified_name().c_str());
                ClassGeneratorResult result{
                    input_file, target_class, header_includes, source_includes, register_includes};
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
                    PRINT_VERBOSE("No previous sibling found, class cannot have a "
                                  "GodotClassAttribute applied. Skipping class.");
                    continue;
                }

                Ref<GodotClassAttribute> class_attribute = previous->as<GodotClassAttribute>();
                if (!class_attribute) {
                    PRINT_VERBOSE(
                        "Class does not have a GodotClassAttribute applied. Skipping class.");
                    continue;
                }

                auto generated_body_attribute =
                    target_class->body()->find_child<GeneratedBodyAttribute>();
                PROG_ERR_COND(
                    !generated_body_attribute || generated_body_attribute->get_index() != 0,
                    "Generated class requires a GODOT_GENERATED_BODY attribute as first entry in "
                    "the class body.");

                result.generated_body_line = generated_body_attribute->line;
                result.generated_sources->add_child(Output::NewLine());

                PROG_ERR_COND(
                    !generated_global_attribute, "File must contain a GODOT_GENERATED_GLOBAL "
                                                 "attribute in the global namespace.");

                GodotClassGenerator class_generator;
                Ref<Context> class_default_values = node_new<Context>();
                Ref<GeneratorError> class_def_gen_error =
                    class_generator.generate_default_attribute_arguments(
                        target_class, class_attribute, class_default_values);
                PROG_ERR_COND(
                    class_def_gen_error != GeneratorError::OK,
                    "Failed to generate default attribute arguments.");

                ClassGenerator::merge_default_attribute_arguments(
                    class_attribute, class_default_values);

                Ref<GeneratorError> class_gen_error =
                    class_generator.generate(target_class, class_attribute, result);

                PROG_ERR_COND(
                    class_gen_error != GeneratorError::OK, "Failed to generate class %s.",
                    target_class->name().c_str());

                Ref<GeneratorError> init_gen_error = class_generator.generate_initialization(
                    target_class, class_attribute, result.initialize, result.uninitialize);

                PROG_ERR_COND(
                    init_gen_error != GeneratorError::OK,
                    "Failed to generate class registration for class %s",
                    target_class->name().c_str());

                if (result.initialize->get_child_count() > 0 ||
                    result.uninitialize->get_child_count() > 0) {
                    result.add_register_include(
                        header_path(generator_args->root_path->get<Path>(), input_file));
                }

                if (!LibraryContext::instance()->file_modified(input_file)) {
                    PRINT_VERBOSE(
                        "Input file \"%s\" was not modified since last read. Skipping.",
                        input_file.c_str());
                    continue;
                }

                PRINT_INFO("Generating class \"%s\"", target_class->name().c_str());

                for (const Ref<Node>& child : *target_class->body()) {
                    if (Ref<Attribute> attribute = child->as<Attribute>()) {
                        for (const Ref<ClassGenerator>& generator :
                             LibraryContext::instance()->get_attribute_db()->class_generators()) {
                            if (generator->handles(target_class, attribute)) {
                                Ref<Context> default_values = node_new<Context>();
                                Ref<GeneratorError> attr_def_error =
                                    generator->generate_default_attribute_arguments(
                                        target_class, attribute, default_values);

                                PROG_ERR_COND(
                                    attr_def_error != GeneratorError::OK,
                                    "Failed to generate default attribute arguments.");

                                ClassGenerator::merge_default_attribute_arguments(
                                    attribute, default_values);

                                Ref<GeneratorError> attr_error =
                                    generator->generate(target_class, attribute, result);

                                PROG_ERR_COND(
                                    attr_error,
                                    "Failed to generate attribute %s in target class %s",
                                    attribute->get_type().c_str(), target_class->name().c_str());
                            }
                        }
                    }
                }
                generate_results.push_back(result);
            }

            if (!LibraryContext::instance()->file_modified(input_file)) {
                PRINT_VERBOSE(
                    "Input file \"%s\" was not modified since last read. Skipping.",
                    input_file.c_str());
                continue;
            }

            FileWriter source_writer = FileWriter::generated(gen_source_path, input_file);
            FileWriter header_writer = FileWriter::generated(gen_header_path, input_file);

            auto target_header = header_path(generator_args->root_path->get<Path>(), input_file);
            Output::Lines({Output::PragmaOnce(), Output::Text("#undef GOC_FILE_ID"),
                           Output::Define("GOC_FILE_ID", {}, file_id(target_header)),
                           Output::Include("godot_object_compiler/macros.h"), Output::NewLine()})
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

        FileWriter register_header_writer = FileWriter::generated(
            generator_args->generated_path->get<Path>() / "generated_register_types.h", "");
        FileWriter register_source_writer = FileWriter::generated(
            generator_args->generated_path->get<Path>() / "generated_register_types.cpp", "");

        register_header_output->get_output(&register_header_writer);
        register_source_output->get_output(&register_source_writer);

        PRINT_VERBOSE("Done generating bindings.");
        return ProgramError::OK;
    }
} // namespace GodotObjectCompiler
