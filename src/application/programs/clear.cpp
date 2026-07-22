/**************************************************************************/
/* clear.cpp                                                              */
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

#include "clear.h"

#include "application/arguments/argument_lists.h"
#include "help.h"
#include "library/core/file_system_utilities.h"
#include "library/core/string_utilities.h"

namespace GodotObjectCompiler
{

    CommandLineArgumentParseResult
    Clear::register_required_arguments(ApplicationContext& p_context) const
    {
        return p_context.register_argument_lists<GeneratorArguments>();
    }

    Ref<ProgramError> Clear::execute(ApplicationContext& p_context)
    {
        ClearTypeDB clear_cache;
        if (Ref<ProgramError> clear_cache_error = clear_cache.run(p_context);
            clear_cache_error != ProgramError::OK) {
            return clear_cache_error;
        }

        ClearGenerated clear_generated;
        if (Ref<ProgramError> clear_generated_error = clear_generated.run(p_context);
            clear_generated_error != ProgramError::OK) {
            return clear_generated_error;
        }

        LibraryContext::instance()->clear_generated_from();
        LibraryContext::instance()->clear_last_modified_times();
        return ProgramError::OK;
    }

    CommandLineArgumentParseResult
    ClearGenerated::register_required_arguments(ApplicationContext& p_context) const
    {
        return p_context.register_argument_lists<GeneratorArguments>();
    }

    Ref<ProgramError> ClearGenerated::execute(ApplicationContext& p_context)
    {
        const auto arguments = p_context.get_argument_list<GeneratorArguments>();

        if (directory_exits(arguments->generated_path->get<Path>())) {
            for (const String& entry : directory_entries(arguments->generated_path->get<Path>())) {
                PROG_ERR_COND(!remove(entry), "Failed to remove \"%s\"", entry.c_str())
            }
        }
        return ProgramError::OK;
    }

    CommandLineArgumentParseResult
    ClearTypeDB::register_required_arguments(ApplicationContext& p_context) const
    {
        return p_context.register_argument_lists<GeneratorArguments>();
    }

    Ref<ProgramError> ClearTypeDB::execute(ApplicationContext& p_context)
    {
        const auto arguments = p_context.get_argument_list<GeneratorArguments>();

        if (directory_exits(arguments->type_db_path->get<Path>())) {
            for (const String& entry : directory_entries(arguments->type_db_path->get<Path>())) {
                PROG_ERR_COND(!remove(entry), "Failed to remove \"%s\"", entry.c_str())
            }
        }
        return ProgramError::OK;
    }

} // namespace GodotObjectCompiler
