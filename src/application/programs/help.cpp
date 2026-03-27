/**************************************************************************/
/* help.cpp                                                               */
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

#include "help.h"

#include <functional>
#include <unordered_set>

#include "application/programs/program.h"
#include "application/version.h"
#include "library/core/core.h"
#include "library/core/file_system_utilities.h"
#include "library/core/resources.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"

namespace GodotObjectCompiler
{

    bool Help::validate_arguments(ApplicationContext& p_context)
    {
        UNUSED(p_context);
        return true;
    }

    Ref<ProgramError> Help::run(ApplicationContext& p_context)
    {
        UNUSED(p_context);

        Dictionary<ProgramPath, Ref<IProgram>> programs = Programs::instance()->get_programs();

        auto cmp = [](const Pair<ProgramPath, Ref<IProgram>>& a,
                      const Pair<ProgramPath, Ref<IProgram>>& b) {
            return string_vector_combine(a.first, "") < string_vector_combine(b.first, "");
        };

        struct ProgramPathHash {
            size_t operator()(const ProgramPath& path) const
            {
                return std::hash<String>()(string_vector_combine(path, ""));
            }
        };

        Vector<Pair<ProgramPath, Ref<IProgram>>> programs_sorted;
        std::copy(programs.begin(), programs.end(), std::back_inserter(programs_sorted));
        std::sort(programs_sorted.begin(), programs_sorted.end(), cmp);

        if (p_context.program_arguments.empty()) {
            print_title(
                format(
                    "Godot Object Compiler v%d.%d \"%s\"", GOC_MAJOR_VERSION, GOC_MINOR_VERSION,
                    GOC_VERSION_NAME),
                100);
            print_ln("");

            if (Resources::instance()->has_resource("res://help/header_content.txt")) {
                print_ln(
                    Resources::instance()->load_text_resource("res://help/header_content.txt"));
            }

            print_title("PROGRAMS", 100);
        }

        std::unordered_set<ProgramPath, ProgramPathHash> written;

        for (const auto& [path, program] : programs_sorted) {
            if (path.empty()) {
                continue;
            }

            bool skip = false;
            for (Size i = 0; i < path.size() && i < p_context.program_arguments.size(); i++) {
                if (path[i] != p_context.program_arguments[i]) {
                    skip = true;
                }
            }

            if (skip) {
                continue;
            }

            if (path.size() != 1) {
                for (auto itr = path.begin(); itr != path.end() - 1; ++itr) {
                    if (Vector<String> sub{path.begin(), itr + 1};
                        written.find(sub) == written.end()) {
                        StreamWriter sub_writer;
                        for (Size i = 0; i < sub.size() - 1; ++i) {
                            sub_writer.write("  ");
                        }
                        sub_writer.write(sub.back());
                        written.insert(sub);

                        print_ln("");
                        print_help_columns({30, sub_writer.get_string()}, {70, ""});
                    }
                }
            }

            StreamWriter identifier_writer;
            for (Size i = 0; i < path.size() - 1; ++i) {
                identifier_writer.write("  ");
            }
            identifier_writer.write(path.back());

            if (!program->requires_project()) {
                identifier_writer.write(" [!p]");
            }

            print_ln("");
            print_help_columns({30, identifier_writer.get_string()}, {70, get_help_text(path)});

            written.insert(path);
        }

        return ProgramError::OK;
    }

    void Help::print_title(const String& p_title, Size width)
    {
        if (p_title.size() + 4 >= width) {
            print_ln(p_title);
        }

        Size available_width = width - 4 - p_title.size();
        StreamWriter writer;

        for (Size i = 0; i < available_width / 2; i++) {
            writer.write("=");
        }

        writer.write("[ ");
        writer.write(p_title);
        writer.write(" ]");

        for (Size i = 0; i < available_width / 2 + available_width % 2; i++) {
            writer.write("=");
        }
        print_ln(writer.get_string());
    }

    String Help::get_help_text(const ProgramPath& p_path)
    {
        if (p_path.empty()) {
            return "";
        }
        String file_stem = string_vector_combine(p_path, "_");
        String res_path = "res://" + path_concat_ext("help", file_stem, "txt");
        if (!Resources::instance()->has_resource(res_path)) {
            return "-";
        }

        return Resources::instance()->load_text_resource(res_path);
    }

    void Help::print_help_columns(const Column& column1, const Column& column2)
    {
        Vector<String> rows1;
        Vector<String> rows2;

        const Vector<String> lines1 = string_split(column1.second, "\n");
        const Vector<String> lines2 = string_split(column2.second, "\n");

        for (const String& line1 : lines1) {
            Vector<String> line_split = string_split_length(line1, column1.first);
            rows1.insert(rows1.end(), line_split.begin(), line_split.end());
        }

        for (const String& line2 : lines2) {
            Vector<String> line_split = string_split_length(line2, column2.first);
            rows2.insert(rows2.end(), line_split.begin(), line_split.end());
        }

        for (Size i = 0; i < std::max(rows1.size(), rows2.size()); ++i) {
            String row1 = i < rows1.size() ? rows1[i] : "";
            String row2 = i < rows2.size() ? rows2[i] : "";
            row1 = string_pad_right(row1, ' ', column1.first);
            row2 = string_pad_right(row2, ' ', column2.first);
            fmt_print_ln("%s %s", row1.c_str(), row2.c_str());
        }
    }

} // namespace GodotObjectCompiler
