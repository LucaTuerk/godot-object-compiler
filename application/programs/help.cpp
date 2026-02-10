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

#include "application/version.h"
#include "library/core/file_system_utilities.h"
#include "library/core/resources.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"

namespace GodotObjectCompiler {

  String Help::get_help_text(const ProgramPath& p_path) {
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

  Ref<ProgramError> Help::run(ApplicationContext& p_context) {
    Dictionary<ProgramPath, Ref<IProgram>> programs = Programs::instance()->get_programs();

    auto cmp = [](const Pair<ProgramPath, Ref<IProgram>>& a, const Pair<ProgramPath, Ref<IProgram>>& b) {
      return string_vector_combine(a.first, "") < string_vector_combine(b.first, "");
    };
    Vector<Pair<ProgramPath, Ref<IProgram>>> programs_sorted;
    std::copy(programs.begin(), programs.end(), std::back_inserter(programs_sorted));
    std::sort(programs_sorted.begin(), programs_sorted.end(), cmp);

    fmt_print_ln("Godot Object Compiler v%d.%d \"%s\"", GOC_MAJOR_VERSION, GOC_MAJOR_VERSION, GOC_VERSION_NAME);

    print_ln("Usage: goc [PROGRAM PATH...] [OPTIONS...]");
    print_ln("");
    print_ln("Available Programs:");
    print_ln(string_pad_right("", '-', 71));

    for (const auto& [path, program] : programs_sorted) {
      if (path.empty()) {
        continue;
      }

      StreamWriter identifier_writer;
      for (Size i = 0; i < path.size() - 1; ++i) {
        identifier_writer.write("  ");
      }
      identifier_writer.write(path.back());

      print_ln("");
      print_help_columns({20, identifier_writer.get_string()}, {50, get_help_text(path)});
    }

    return ProgramError::OK;
  }

  void Help::print_help_columns(Column column1, Column column2) {
    Vector<String> rows1;
    Vector<String> rows2;

    Vector<String> lines1 = string_split(column1.second, "\n");
    Vector<String> lines2 = string_split(column2.second, "\n");

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

}