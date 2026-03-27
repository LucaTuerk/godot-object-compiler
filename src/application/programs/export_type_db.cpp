/**************************************************************************/
/* export_type_db.cpp                                                     */
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

#include "export_type_db.h"

#include "application/programs/clear.h"
#include "application/programs/generate_type_db.h"
#include "application/programs/program.h"
#include "library/core/core.h"
#include "library/core/file_system_utilities.h"
#include "library/core/permissions.h"
#include "library/core/string_utilities.h"
#include "library/execution_context.h"

namespace GodotObjectCompiler
{

    bool ExportTypeDB::validate_arguments(ApplicationContext& p_context)
    {
        return p_context.program_arguments.size() == 1 &&
               could_be_dir_path(p_context.program_arguments[0]);
    }

    Ref<ProgramError> ExportTypeDB::run(ApplicationContext& p_context)
    {
        PROG_ERR_COND(
            p_context.program_arguments.size() != 1,
            "Invalid argument count. Expected directory path to export "
            "TypeDB to.");

        String export_dir = p_context.program_arguments[0];

        if (directory_exits(export_dir) &&
            (!directory_dirs(export_dir).empty() || !directory_files(export_dir).empty())) {
            PROG_ERR("A non empty directory exists at the export "
                     "path. Path must "
                     "point to an empty or non existing directory");
        }

        Permissions::instance()->add_write_path(export_dir);

        PROG_ERR_COND(
            !directory_exits(export_dir) && !create_dir_recursive(export_dir),
            "Failed to create export directory.");

        ClearCache clear_cache;
        Ref<ProgramError> clear_error = clear_cache.run(p_context);
        if (clear_error != ProgramError::OK) {
            return clear_error;
        }

        GenerateTypeDB generate_type_db;
        Ref<ProgramError> generate_error = generate_type_db.run(p_context);
        if (generate_error != ProgramError::OK) {
            return generate_error;
        }

        for (const String& file : directory_files_recursive(p_context.paths_cache)) {
            if (string_contains(file, ".readonly")) {
                continue;
            }

            String relative_path = path_relative(file, p_context.paths_cache);
            String destination_path = path_concat(export_dir, relative_path);

            PROG_ERR_COND(
                !copy_file(file, destination_path), "Failed to copy file \"%s\" to \"%s\"",
                file.c_str(), destination_path.c_str());
        }

        return ProgramError::OK;
    }

} // namespace GodotObjectCompiler
