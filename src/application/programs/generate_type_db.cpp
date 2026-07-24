/**************************************************************************/
/* generate_type_db.cpp                                                   */
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

#include "generate_type_db.h"

#include "application/application_context.h"
#include "application/arguments/argument_lists.h"
#include "application/arguments/argument_parsers.h"
#include "library/core/config.h"
#include "library/core/file_system_utilities.h"
#include "library/core/string_utilities.h"
#include "library/library_context.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/define.h"
#include "library/tree/syntax/enum.h"
#include "library/tree/syntax/namespace.h"
#include "library/tree/syntax/struct.h"
#include "library/type_db.h"
#include "library_godot/parsers/extension_api_parser.h"

namespace GodotObjectCompiler
{

    CommandLineArgumentParseResult
    GenerateTypeDB::register_required_arguments(ApplicationContext& p_context) const
    {
        return p_context.register_argument_lists<GeneratorArguments, GDExtensionProjectArguments>();
    }

    void GenerateTypeDB::generate_from_file(
        const File& p_file, const ApplicationContext& p_context, IParser* p_parser)
    {
        const auto project_args = p_context.get_argument_list<GDExtensionProjectArguments>();

        auto& [path, include_path] = p_file;

        if (path.extension() != ".h" && path.extension() != ".hpp" &&
            path.extension() != ".gen.inc" && path.extension() != ".json") {
            return;
        }

        auto sources = project_args->sources->get<Vector<Path>>();

        bool is_input_file = std::find(sources.begin(), sources.end(), path) != sources.end();

        if (!is_input_file && !LibraryContext::instance()->file_modified(path)) {
            PRINT_VERBOSE("Skipping \"%s\". Not modified.", path.c_str());
            return;
        }

        PRINT_VERBOSE("Scanning \"%s\"", path.c_str());
        file_count++;

        const Ref<Namespace> global_namespace = node_new<Namespace>();

        if (Ref<ParserError> error = p_parser->parse_file(path, global_namespace);
            error != ParserError::OK) {
            return;
        }

        if (global_namespace) {
            auto is_valid_type_target = [](const Ref<NamedContext>& node) {
                return node->is<Class>() || node->is<Struct>() || node->is<Enum>() ||
                       node->is<Define>();
            };

            Vector<Ref<NamedContext>> found = global_namespace->find_children<NamedContext>(
                true, [is_valid_type_target](const Ref<NamedContext>& node) {
                    return is_valid_type_target(node) || node->is<Attribute>();
                });

            for (const Ref<NamedContext>& node : found) {
                if (Ref<Attribute> attr = node->as<Attribute>(); attr) {
                    if (!attr->resolve_target()) {
                        continue;
                    }

                    if (Ref<NamedContext> type = attr->resolve_target()->as<NamedContext>();
                        type && is_valid_type_target(type)) {
                        PRINT_VERBOSE("Discovered attribute %s", node->get_type().c_str());
                        LibraryContext::instance()->get_type_db()->save_type_attribute(
                            type, attr, path);
                    }
                    continue;
                }

                PRINT_VERBOSE("Discovered type %s", node->qualified_name().c_str());
                type_count++;

                if (include_path.has_value()) {
                    node->header = header_path(Path(include_path.value()), path);
                }
                LibraryContext::instance()->get_type_db()->save_type_data(node, path);
            }
        }
    }

    Ref<ProgramError> GenerateTypeDB::execute(ApplicationContext& p_context)
    {
        const auto project_args = p_context.get_argument_list<GDExtensionProjectArguments>();
        const auto generator_args = p_context.get_argument_list<GeneratorArguments>();

        LibraryContext::instance()->add_include_paths(project_args->godot_cpp->get<Vector<Path>>());

        file_count = 0;
        type_count = 0;

        const Ref<IParser> parser =
            LibraryContext::instance()->get_default_parser(IParser::SOURCE_PARSER);
        parser->config(IParser::CONFIG_SKIP_ATTRIBUTES);

        ExtensionAPIParser extension_api_parser;
        extension_api_parser.setup_include_paths(project_args->godot_cpp->get<Vector<Path>>());

        Vector<File> files = {{project_args->extension_api->get<Path>(), std::nullopt}};

        generate_from_file(
            {project_args->extension_api->get<Path>(), std::nullopt}, p_context,
            &extension_api_parser);

        auto includes = generator_args->include_paths->get<Vector<Path>>();
        includes.push_back(generator_args->root_path->get<Path>());

        for (const Path& path : includes) {
            Path include_path = path_absolute(path);
            for (const Path& file : directory_files_recursive(include_path)) {
                generate_from_file(
                    {path_absolute(file), include_path.string()}, p_context, parser.get());
            }
        }

        if (file_count > 0) {
            PRINT_INFO(
                "Generate TypeDB: Scanned %d file(s) discovering %d type(s).", file_count,
                type_count);
        } else {
            PRINT_VERBOSE("Generate TypeDB: No files scanned.");
        }

        return ProgramError::OK;
    }

} // namespace GodotObjectCompiler
