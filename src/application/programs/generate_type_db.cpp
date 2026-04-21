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
#include "library/core/config.h"
#include "library/core/file_system_utilities.h"
#include "library/core/string_utilities.h"
#include "library/library_context.h"
#include "library/parser/parser.h"
#include "library/tree/syntax/class.h"
#include "library/tree/syntax/define.h"
#include "library/tree/syntax/enum.h"
#include "library/tree/syntax/namespace.h"
#include "library/tree/syntax/struct.h"
#include "library/type_db.h"
#include "library_godot/parsers/extension_api_parser.h"

namespace GodotObjectCompiler
{

    struct File {
        String path;
        Opt<String> include_path;
    };

    void
    generate_from_file(const File& p_file, const ApplicationContext& p_context, IParser* p_parser)
    {
        auto& [path, include_path] = p_file;

        if (!string_suffix(path, ".h") && !string_suffix(path, ".hpp") &&
            !string_suffix(path, ".gen.inc") && !string_suffix(path, ".json")) {
            return;
        }

        bool is_input_file =
            p_context.files_input.has_value() &&
            std::find(p_context.files_input->begin(), p_context.files_input->end(), path) !=
                p_context.files_input->end();

        if (!is_input_file && !LibraryContext::instance()->file_modified(path)) {
            PRINT_VERBOSE("TypeDB:\tSkipping \"%s\". Not modified.", path.c_str());
            return;
        }

        PRINT_VERBOSE("TypeDB:\tProcessing \"%s\"", path.c_str());

        Ref<Namespace> global_namespace = node_new<Namespace>();
        TreeSitterParser* tree_sitter_parser = dynamic_cast<TreeSitterParser*>(p_parser);
        Ref<ParserError> error = tree_sitter_parser
                                     ? tree_sitter_parser->parse_file(path, global_namespace)
                                     : p_parser->parse(path, global_namespace);

        if (error != ParserError::OK) {
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
                        PRINT_VERBOSE(
                            "TypeDB:\tSaving attribute \"%s\"", node->qualified_name().c_str());
                        LibraryContext::instance()->get_type_db()->save_type_attribute(
                            type, attr, path);
                    }
                    continue;
                }

                PRINT_VERBOSE("TypeDB:\tSaving type \"%s\"", node->qualified_name().c_str());

                if (include_path.has_value()) {
                    node->header = header_path(include_path.value(), path);
                }
                LibraryContext::instance()->get_type_db()->save_type_data(node, path);
            }
        }
    }

    Ref<ProgramError> GenerateTypeDB::run(ApplicationContext& p_context)
    {
        PROG_ERR_COND(
            !p_context.paths_include.has_value(),
            "No include path specified. Cannot generate the TypeDB.");
        PROG_ERR_COND(
            !p_context.path_extension_api.has_value(),
            "No extension api path specified. Cannot generate the TypeDB.");
        PROG_ERR_COND(
            !p_context.paths_godot_cpp.has_value(),
            "No godot-cpp path specified. Cannot generate the TypeDB");

        TreeSitterParser tree_sitter_parser;
        tree_sitter_parser.set_parse_attributes(false);

        ExtensionAPIParser extension_api_parser;
        extension_api_parser.setup_include_paths(p_context.paths_godot_cpp.value());

        Vector<File> files = {{p_context.path_extension_api.value(), std::nullopt}};

        generate_from_file(
            {p_context.path_extension_api.value(), std::nullopt}, p_context, &extension_api_parser);

        for (String include_path : *p_context.paths_include) {
            include_path = path_absolute(include_path);
            for (const String& file : directory_files_recursive(include_path)) {
                generate_from_file(
                    {path_absolute(file), include_path}, p_context, &tree_sitter_parser);
            }
        }

        return ProgramError::OK;
    }

} // namespace GodotObjectCompiler
