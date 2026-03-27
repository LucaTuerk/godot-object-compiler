/**************************************************************************/
/* init_tools.cpp                                                         */
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

#include "init_tools.h"

#include "library/core/file_system_utilities.h"
#include "library/core/permissions.h"
#include "library/core/resources.h"
#include "library/core/string_writer.h"

namespace GodotObjectCompiler
{

    bool InitTools::validate_arguments(ApplicationContext& p_context)
    {
        return p_context.program_arguments.size() == 1 &&
               could_be_dir_path(p_context.program_arguments[0]);
    }

    Ref<ProgramError> InitTools::run(ApplicationContext& p_context)
    {
        PROG_ERR_COND(
            p_context.program_arguments.size() != 1,
            "Invalid argument count for init tools program. Expected "
            "single path argument specifying target directory.");

        const String path = p_context.program_arguments[0];
        Permissions::instance()->add_write_path(path);

        PROG_ERR_COND(
            !directory_exits(path) && !create_dir_recursive(path),
            "Target directory does not exists and could not be "
            "created.");

        for (const String& res_file : Resources::instance()->resources_recursive("res://tools")) {
            const String content = Resources::instance()->load_text_resource(res_file);
            const String relative = path_relative(res_file, "res://tools");
            FileWriter writer(path_concat(path, relative));
            writer.write(content);
        }

        return ProgramError::OK;
    }

} // namespace GodotObjectCompiler