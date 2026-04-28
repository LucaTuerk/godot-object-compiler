/**************************************************************************/
/* generate_resources.cpp                                                 */
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

#ifdef DEV_BUILD
#include "generate_resources.h"

#include "application/application.h"
#include "generate.h"
#include "generate_bindings.h"
#include "generate_type_db.h"
#include "library/attribute_db.h"
#include "library/core/file_system_utilities.h"
#include "library/core/permissions.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"
#include "library/tree/output/output.h"
#include "library/tree/output/output_transformator.h"
#include "library/tree/syntax/identifier.h"
#include "library/tree/syntax/namespace.h"
#include "library_godot/assumptions.h"

namespace GodotObjectCompiler
{

    String resource_variable_name(const String& p_path)
    {
        String result = p_path;
        result = string_replace(result, "/", "_");
        result = string_replace(result, ".", "_");
        result = string_replace(result, "\\", "_");
        result = string_replace(result, ":", "_");
        return result;
    }

    String GenerateResources::rst_header(const String& p_text, char p_punctuation)
    {
        StreamWriter writer;
        writer.write(p_text);
        writer.write("\n");
        writer.write(char_times_n(p_punctuation, p_text.size()));
        writer.write("\n");
        return writer.get_string();
    }

    String GenerateResources::rst_table(const Table& table)
    {
        Vector<Size> per_column_length;
        Vector<Size> per_row_height;

        for (Size y = 0; y < table.size(); ++y) {
            for (Size x = 0; x < table[y].size(); ++x) {
                const String& cell = table[y][x];
                Size size_x, size_y;
                string_get_2d_size(cell, size_x, size_y);

                if (x >= per_column_length.size()) {
                    per_column_length.push_back(size_x);
                } else {
                    per_column_length[x] = std::max(per_column_length[x], size_x);
                }

                if (y >= per_row_height.size()) {
                    per_row_height.push_back(size_y);
                } else {
                    per_row_height[y] = std::max(per_row_height[y], size_y);
                }
            }
        }

        StreamWriter writer;
        for (Size y = 0; y < table.size(); ++y) {
            const TableRow& row = table[y];
            Size row_height = per_row_height[y];

            Vector<Vector<String>> row_cell_lines;
            for (const String& cell : row) {
                row_cell_lines.push_back(string_split(cell, "\n"));
            }

            for (Size x = 0; x < row.size(); ++x) {
                Size col_width = per_column_length[x];
                writer.write("+");
                writer.write(char_times_n(y == 1 ? '=' : '-', col_width + 2));
                if (x == row.size() - 1) {
                    writer.write("+\n");
                }
            }

            for (Size row_y = 0; row_y < row_height; ++row_y) {
                for (Size x = 0; x < row.size(); ++x) {
                    Size col_width = per_column_length[x];
                    writer.write("| ");

                    if (x < row_cell_lines.size() && row_y < row_cell_lines[x].size()) {
                        writer.write(
                            string_pad_right(row_cell_lines[x][row_y], ' ', col_width + 1));
                    } else {
                        writer.write(char_times_n(' ', col_width + 1));
                    }

                    if (x == row.size() - 1) {
                        writer.write("|\n");
                    }
                }
            }
        }

        for (Size x = 0; x < per_column_length.size(); ++x) {
            Size col_width = per_column_length[x];
            writer.write("+");
            writer.write(char_times_n('-', col_width + 2));
        }
        writer.write("+\n");

        return writer.get_string();
    }

    Ref<ProgramError> GenerateResources::run(ApplicationContext& p_context)
    {
        GenerateTypeDB generate_type_db;
        PROG_ERR_PASS_ON(generate_type_db.run(p_context));
        PROG_ERR_COND(
            !(AssumedGodotTypes::validate_assumptions() &&
              AssumedParameterValues::validate_assumptions()),
            "Failed to validate some assumptions on available Godot types and macros, probably "
            "because the TypeDB generator has not found the relevant files.\nEnsure godot-cpp "
            "include path are known to goc via the -I= flag or in the .goc_project file.");

        Permissions::instance()->add_write_path("resources");
        Permissions::instance()->add_write_path("docs");

        // Generate Program help resources
        Dictionary<ProgramPath, Ref<IProgram>> programs = Programs::instance()->get_programs();
        for (const auto& [path, program] : programs) {
            String file_stem = string_vector_combine(path, "_");
            String file_path = path_concat_ext("resources/help", file_stem, "txt");
            if (!file_exists(file_path)) {
                FileWriter writer(file_path);
                writer.write("No description available");
            }
            String description = read_file(file_path);
            if (string_contains(description, "DEV_PROGRAM")) {
                // dev specific programs should not be included in the docs as they
                // are not meant for the common user. cli help texts are still available.
                continue;
            }

            String doc_path = path_concat("docs", "cli");
            String desc_path = path_concat(doc_path, "descriptions");
            create_dir_recursive(desc_path);

            String doc_file_path = path_concat_ext(doc_path, file_stem, "rst");
            String desc_file_path = path_concat_ext(desc_path, file_stem, "rst");

            StreamWriter documentation_writer;
            documentation_writer.write(rst_header(string_vector_combine(path, " "), '='));
            documentation_writer.write("\n");
            documentation_writer.write(
                format(".. include:: descriptions/%s.rst", file_stem.c_str()));
            documentation_writer.write("\n");

            write_initial_file_content(doc_file_path, documentation_writer.get_string());

            FileWriter description_writer(desc_file_path);
            description_writer.write(description);
        }

        // Generate macro help docs
        for (const String& macro :
             LibraryContext::instance()->get_attribute_db()->get_all_macros()) {
            String internal_doc_path = path_concat_ext("resources/doc", macro, "txt");
            write_initial_file_content(internal_doc_path, "No documentation available");

            String doc_path = path_concat_ext("docs/macros/", macro, "rst");
            write_initial_file_content(doc_path, "No documentation available");

            String doc_desc_path = path_concat_ext("docs/macros/descriptions/", macro, "rst");
            create_dir_recursive(path_base(doc_desc_path));
            write_file(doc_desc_path, read_file(internal_doc_path));

            Vector<Ref<IAttributeParameterType>> params =
                LibraryContext::instance()->get_attribute_db()->get_parameters_for_macro(macro);

            for (const auto& param : params) {
                if (param->is_builtin()) {
                    continue;
                }
                Table table;
                table.push_back({"Value", "Description"});

                auto param_res_doc_dir = path_concat("resources/doc", param->get_return_type());
                auto param_res_doc_path =
                    path_concat_ext("resources/doc", param->get_return_type(), "txt");

                create_dir_recursive(param_res_doc_dir);
                write_initial_file_content(param_res_doc_path, "No documentation available");

                for (const auto& value_name : param->get_value_names()) {
                    auto value_doc_path = path_concat_ext(param_res_doc_dir, value_name, "txt");
                    write_initial_file_content(value_doc_path, "No documentation available");

                    table.push_back({value_name, read_file(value_doc_path)});
                }

                String param_doc_path =
                    path_concat_ext("docs/macros/parameters/", param->get_return_type(), "rst");
                create_dir_recursive(path_base(param_doc_path));

                StreamWriter writer;
                writer.write(rst_header(param->get_return_type(), '^'));
                writer.write("\n");
                writer.write(read_file(param_res_doc_path));
                writer.write("\n");
                writer.write("\n");
                writer.write("The following values are available:\n\n");
                writer.write(rst_table(table));
                write_file(param_doc_path, writer.get_string());
            }
        }

        Ref<Namespace> global_namespace = make_ref<Namespace>();
        Ref<Body> body;
        global_namespace->add_children({B<Body>()[{
            Output::PragmaOnce(), Output::Include("library/core/resources.h"),
            B<Namespace>()[{B<Identifier>("GOC_Resources"), R<Body>(&body)}]}]});

        auto files = directory_files_recursive("resources");
        std::sort(files.begin(), files.end());
        // Compile Resources
        for (const String& file : files) {
            String content = read_file(file);
            String relative = path_relative(file, path_cwd());

            Ref<Output::ListNode> values;
            body->add_children(
                {Output::FmtText(
                     "constexpr char %s[] = ", resource_variable_name(relative).c_str()),
                 B<Body>()[R<Output::ListNode>(&values, ", ", false, false)], Output::Semicolon()});

            Size i = 0;
            for (char c : content) {
                if ((++i % 15) == 0) {
                    values->add_child(Output::FmtText("\n0x%x", c));
                } else {
                    values->add_child(Output::FmtText("0x%x", c));
                }
            }
            values->add_child(Output::FmtText("0x%x", '\0'));
        }
        {
            Ref<Output::ListNode> values;
            body->add_children(
                {Output::FmtText(
                     "static inline GodotObjectCompiler::Resources::ResourcePack Pack = "),
                 B<Body>()[R<Output::ListNode>(&values, ",\n", false, false)],
                 Output::Semicolon()});

            for (const String& file : files) {
                String relative = path_relative(file, path_cwd());
                String res_path = "res://" + path_relative(relative, "resources");
                values->add_child(Output::FmtText(
                    "{\"%s\", &%s[0]}", res_path.c_str(),
                    resource_variable_name(relative).c_str()));
            }
        }

        Permissions::instance()->add_write_path("src/application/compiled_resources");
        FileWriter writer =
            FileWriter::generated("src/application/compiled_resources/res.gen.h", "");
        OutputTransformator transformator;

        transformator.transform(global_namespace)->get_output(&writer);

        GenerateBindings generate_example;
        p_context.paths_root = path_absolute("docs/files");
        p_context.paths_include->push_back(*p_context.paths_root);
        p_context.paths_generated = path_absolute("docs/generated_files");
        PROG_ERR_PASS_ON(generate_example.run(p_context));

        return ProgramError::OK;
    }

} // namespace GodotObjectCompiler
#endif
