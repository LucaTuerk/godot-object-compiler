/**************************************************************************/
/* application.cpp                                                        */
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

#include "application.h"

#include "application_context.h"
#include "build_number.h"
#include "compiled_resources/res.gen.h"
#include "library/core/file_system_utilities.h"
#include "library/core/resources.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"
#include "library/execution_context.h"
#include "library/type_db.h"
#include "programs/clear.h"
#include "programs/help.h"
#include "programs/init_local_resources.h"
#include "programs/program.h"

namespace GodotObjectCompiler
{

    bool Application::was_last_exit_graceful(const ApplicationContext& context)
    {
        String lock_path = path_concat(context.paths_goc, ".goc_graceful_lock");
        if (file_exists(lock_path)) {
            return false;
        }
        write_initial_file_content(
            lock_path, "This file is used by the godot object compiler to check "
                       "if "
                       "the last program exit was graceful.\n"
                       "Removing this file may lead to unexpected behaviour.");
        return true;
    }

    int Application::exit_gracefully(const ApplicationContext& context, int p_return_code)
    {
        if (context.program == nullptr || !context.program->requires_project()) {
            return p_return_code;
        }

        String lock_path = path_concat(context.paths_goc, ".goc_graceful_lock");

        if (file_exists(lock_path) && remove_file(lock_path)) {
            PRINT_INFO("GOC: Graceful exit.");
            return p_return_code;
        }

        APP_ERR("GOC: Tried to exit gracefully but lock file no longer "
                "exists. "
                "This indicates a corrupted GOC directory.\n"
                "You may want to clear your cache to ensure smooth "
                "operations.")
    }

    int Application::run(Vector<String> p_arguments)
    {
        ApplicationContext application_context;
        APP_TOP_LEVEL_ERR_COND(
            setup_context(p_arguments, application_context) != 0,
            "Failed to setup application context.");
        APP_TOP_LEVEL_ERR_COND(
            run_program(application_context) != 0, "Failed to run the provided program.");
        return cleanup_context(application_context);
    }

    int Application::setup_context(Vector<String> p_arguments, ApplicationContext& r_context)
    {
        Resources::instance()->load_pack(&GOC_Resources::Pack);

        r_context.application_arguments = p_arguments;
        r_context.program = Programs::instance()->find_program(
            r_context.application_arguments, r_context.program_arguments);

        if (!r_context.program) {
            Help help;
            ApplicationContext help_context = r_context;
            if (r_context.application_arguments.empty()) {
                print_err("Please specify a program to run.");
                APP_ERR_COND(help.run(help_context) != ProgramError::OK, "Failed to run help");
            } else {
                print_err(format(
                    "Could not find program \"%s\".", r_context.application_arguments[0].c_str()));
                help_context.program_arguments =
                    string_split(r_context.application_arguments[0], "/");
                APP_ERR_COND(
                    help.run(help_context) != ProgramError::OK,
                    "Failed to get help info for program %s",
                    r_context.application_arguments[0].c_str());
                return 1;
            }
            return 1;
        }

        if (r_context.program->requires_project()) {
            Clear clear;
            InitLocalResources init_local_resources;
            Vector<String> cwd_files = directory_files(path_cwd());
            auto project_path_itr =
                std::find_if(cwd_files.begin(), cwd_files.end(), [](const String& path) {
                    return string_suffix(path, ".goc_project");
                });

            if (Project project;
                project_path_itr != cwd_files.end() && project.read_from_file(*project_path_itr)) {
                if (!r_context.set_from_project(project)) {
                    return 1;
                }
            } else if (!r_context.set_from_application_arguments(r_context.application_arguments)) {
                return 1;
            }

            if (!was_last_exit_graceful(r_context)) {
                PRINT_INFO("GOC: Last exit was ungraceful. "
                           "Clearing context and files.");
                APP_ERR_COND(
                    clear.run(r_context) != ProgramError::OK,
                    "Failed to clear the goc directories after "
                    "an "
                    "ungraceful exit was detected.\n"
                    "Please delete your goc folders manually "
                    "to ensure "
                    "smooth operations.")
            }

            APP_ERR_COND(
                !r_context.paths_root.has_value(),
                "No project root path specified, but is needed for "
                "selected program.");
            APP_ERR_COND(
                !r_context.paths_include.has_value(),
                "No include paths were specified, but are needed "
                "for "
                "selected program.");

            r_context.paths_include->push_back(*r_context.paths_root);

            ExecutionContext::instance()->get_type_db()->set_cache_directory(r_context.paths_cache);
            ExecutionContext::instance()->set_include_paths(*r_context.paths_include);

            init_local_resources.run(r_context);

            ExecutionContext::instance()->set_remove_macros(
                read_lines(path_concat(r_context.paths_goc, "macros/macro_remove.txt")));

            ExecutionContext::instance()->load_last_modified_times_file(
                path_concat_ext(r_context.paths_goc, "last_modified", "gocdb"));

            ExecutionContext::instance()->load_generated_from_file(
                path_concat_ext(r_context.paths_goc, "generated_from", "gocdb"));

            ExecutionContext::instance()->clean_generated_files();

            if (r_context.project_target == TARGET_GDEXTENSION) {
                ExecutionContext::instance()->add_using("godot");
            }

            auto build_num_file = path_concat(r_context.paths_goc, "last_goc_build_number.txt");
            String build_num = GOC_BUILD_NUMBER;
            if (file_exists(build_num_file)) {
                if (String last_build_num = read_file(build_num_file);
                    last_build_num != build_num) {
                    APP_ERR_COND(
                        clear.run(r_context) != ProgramError::OK, "Failed to clear the goc "
                                                                  "directories after a "
                                                                  "change in goc version was "
                                                                  "detected.\n"
                                                                  "Please delete your goc folders "
                                                                  "manually to "
                                                                  "ensure smooth operations.")
                }
            }

            FileWriter writer(build_num_file);
            writer.write(build_num);
        }
        return 0;
    }

    int Application::run_program(ApplicationContext& p_context)
    {
        if (!p_context.program->validate_arguments(p_context)) {
            fmt_print_err(
                "Invalid argument(s) for program %s", p_context.program->program_name().c_str());
            ApplicationContext help_context = p_context;
            help_context.program_arguments = string_split(p_context.program->program_name(), "/");
            Help help;
            APP_ERR_COND(
                help.run(help_context), "Failed to get help info for program %s",
                p_context.program->program_name().c_str());
            return 1;
        }

        APP_ERR_COND(
            p_context.program->run(p_context) != ProgramError::OK,
            "Error occurred while executing program.");
        return 0;
    }

    int Application::cleanup_context(const ApplicationContext& p_context)
    {
        if (p_context.program->requires_project()) {
            ExecutionContext::instance()->save_last_modified_times_file(
                path_concat_ext(p_context.paths_goc, "last_modified", "gocdb"));
            ExecutionContext::instance()->save_generated_from_file(
                path_concat_ext(p_context.paths_goc, "generated_from", "gocdb"));
        }
        return exit_gracefully(p_context, 0);
    }

} // namespace GodotObjectCompiler
