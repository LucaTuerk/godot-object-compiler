/**************************************************************************/
/* program.cpp                                                            */
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

#include "program.h"

#include "library/core/string_utilities.h"

namespace GodotObjectCompiler {

bool Programs::register_program(const Ref<IProgram>& p_program) {
  if (_registered_programs.find(p_program->program_name()) !=
      _registered_programs.end()) {
    return false;
  }

  const ProgramPath path = string_split(p_program->program_name(), "/");
  _programs[path] = p_program;
  return true;
}

Ref<IProgram> Programs::find_program(
    const Vector<String>& p_application_arguments,
    Vector<String>& r_program_arguments) {
  Ref<IProgram> found_program;

  for (const auto& [path, program] : _programs) {
    Size matching = overlap(path, p_application_arguments);
    if (Size current_overlap = 0;
        matching == path.size() && matching > current_overlap) {
      found_program = program;
      r_program_arguments = {};

      for (Size i = matching; i < p_application_arguments.size(); i++) {
        if (!string_prefix(p_application_arguments[i], "-")) {
          r_program_arguments.push_back(p_application_arguments[i]);
        }
      }
    }
  }

  return found_program;
}

const Dictionary<ProgramPath, Ref<IProgram>>& Programs::get_programs() {
  return _programs;
}

Size Programs::overlap(const ProgramPath& a, const ProgramPath& b) {
  Size overlap = 0;
  for (Size i = 0; i < a.size() && i < b.size(); i++) {
    if (a[i] != b[i]) {
      break;
    }
    overlap++;
  }
  return overlap;
}

}  // namespace GodotObjectCompiler
