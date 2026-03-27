/**************************************************************************/
/* import_type_db.cpp                                                     */
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
#include "application/programs/import_type_db.h"

#include "application/programs/program.h"
#include "library/core/core.h"
#include "library/core/file_system_utilities.h"

namespace GodotObjectCompiler
{

bool ImportTypeDB::validate_arguments(ApplicationContext& p_context)
{
  return p_context.program_arguments.size() == 1 &&
         could_be_dir_path(p_context.program_arguments[0]);
}

Ref<ProgramError> ImportTypeDB::run(ApplicationContext& p_context)
{
  PROG_ERR_COND(
      p_context.program_arguments.size() != 1,
      "Invalid argument count. Expected path to the type_db "
      "directory to import.");

  const String& import_path = p_context.program_arguments[0];

  PROG_ERR_COND(
      !directory_exits(import_path),
      "Invalid argument. Could not find director at provided path \"%s\".",
      p_context.program_arguments[0].c_str());

  for (const String& file : directory_files_recursive(import_path)) {
    String relative_path = path_relative(file, import_path);
    String destination_path =
        path_concat(p_context.paths_readonly_cache, relative_path);
    String base = path_base(destination_path);

    PROG_ERR_COND(
        !directory_exits(base) && !create_dir_recursive(base),
        "Import failed. Could not create target directory at \"%s\".",
        base.c_str());
    PROG_ERR_COND(
        !copy_file(file, destination_path),
        "Failed to copy file \"%s\" to \"%s\"", file.c_str(),
        destination_path.c_str());
  }

  return ProgramError::OK;
}

} // namespace GodotObjectCompiler
