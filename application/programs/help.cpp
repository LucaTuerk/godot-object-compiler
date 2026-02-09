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

#include "library/core/file_system_utilities.h"
#include "library/core/resources.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"

namespace GodotObjectCompiler {

  String HelpEntry::get_help_text() const {
    if (path.empty()) {
      return "";
    }
    String file_stem = string_vector_combine(path, "_");
    String res_path = "res://" + path_concat_ext("help", file_stem, "txt");
    if (!Resources::instance()->has_resource(res_path)) {
      return "";
    }

    return Resources::instance()->load_text_resource(res_path);
  }

  Ref<ProgramError> Help::run(ApplicationContext& p_context) {
    Dictionary<ProgramPath, Ref<IProgram>> programs = Programs::instance()->get_programs();

    const Ref<Context> help = node_new<HelpEntry>(ProgramPath(), "Available Programs: ", false);

    for (const auto& [path, program] : programs) {
      Ref<Context> current = help;
      for (Size i = 0; i < path.size(); ++i) {
        String val = path.at(i);
        if (const Ref<HelpEntry> existing =
                current->find_child<HelpEntry>(0, [val](Ref<HelpEntry> entry) { return entry->name == val; })) {
          if (i == path.size() - 1) {
            existing->path = path;
            existing->is_program = true;
          } else {
            current = existing;
          }
        } else {
          if (i == path.size() - 1) {
            current->build_child<HelpEntry>(path, val, true);
          } else {
            const Ref<HelpEntry> new_entry = current->build_child<HelpEntry>(path, val, false);
            current = new_entry;
          }
        }
      }
    }

    print_ln(help->pretty_print());
    return ProgramError::OK;
  }

  String HelpEntry::to_string() const {
    if (!path.empty()) {
      return format("%s\n%s", name.c_str(), get_help_text().c_str());
    } else {
      return name;
    }
  }

}