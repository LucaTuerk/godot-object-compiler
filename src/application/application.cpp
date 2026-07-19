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

#include <utility>

#include "application_context.h"
#include "build_info.h"
#include "compiled_resources/res.gen.h"
#include "library/core/file_system_utilities.h"
#include "library/core/permissions.h"
#include "library/core/resources.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"
#include "library/library_context.h"
#include "library/parser.h"
#include "library/type_db.h"
#include "programs/clear.h"
#include "programs/help.h"
#include "programs/program.h"

namespace GodotObjectCompiler
{
    bool Application::was_last_exit_graceful() const
    {
        auto arguments = context.get_argument_list<CLIArgs::ApplicationArguments>();

        const String lock_path =
            path_concat(arguments->goc_path->get<Path>(), ".goc_graceful_lock");
        if (file_exists(lock_path)) {
            return false;
        }
        write_initial_file_content(
            lock_path,
            "This file is used by the godot object compiler to check if the last program exit was "
            "graceful.\nRemoving this file may lead to unexpected behaviour.");
        return true;
    }

    int Application::exit_gracefully(int p_return_code) const
    {

        if (context.program == nullptr || context.program->is_readonly()) {
            return p_return_code;
        }

        auto arguments = context.get_argument_list<CLIArgs::ApplicationArguments>();
        if (const String lock_path =
                path_concat(arguments->goc_path->get<Path>(), ".goc_graceful_lock");
            file_exists(lock_path) && remove_file(lock_path)) {
            PRINT_VERBOSE("Graceful exit.");
            return p_return_code;
        }

        APP_ERR(
            "Tried to exit gracefully but the lock file no longer exists. This indicates a "
            "corrupted cache directory.\nPlease delete the \"%s\" directory to ensure proper "
            "operations.",
            arguments->goc_path->get<Path>().c_str());
    }

    bool Application::init_local_resources() const
    {
        auto arguments = context.get_argument_list<CLIArgs::ApplicationArguments>();

        if (!Resources::instance()->copy_resources_to_folder(
                {
                    "res://variant_types",
                    "res://macros",
                },
                arguments->goc_path->get<Path>())) {
            return false;
        }
        return true;
    }

    int Application::run(const Vector<String>& p_arguments)
    {
        PRINT_VERBOSE("Application: %s", string_vector_combine(p_arguments, " ").c_str());
        APP_TOP_LEVEL_ERR_COND(
            setup_context(p_arguments) != 0, "Failed to setup application context.");
        APP_TOP_LEVEL_ERR_COND(
            run_program(context.program) != 0, "Failed to run the %s program.",
            context.program->program_name().c_str());
        return cleanup();
    }

    Application::Application()
    {
        PANIC_COND(
            has_application,
            "Invalid Application init. Another Application instance is still active.");
        has_application = true;
        LibraryContext::instance()->init();
    }

    Application::~Application()
    {
        has_application = false;
    }

    int Application::setup_context(Vector<String> p_arguments)
    {
        Resources::instance()->load_pack(&GOC_Resources::Pack);
        CLI_PARS_ERR_V(context.register_argument_lists<CLIArgs::ApplicationArguments>(), 1);

        context.arguments = std::move(p_arguments);

        Vector<String> program_arguments;

        context.program = Programs::instance()->find_program(context.arguments, program_arguments);

        if (!context.program) {
            Help help;
            ApplicationContext help_context = context;
            APP_ERR_COND(help.run(help_context) != ProgramError::OK, "Failed to run help");
            return 1;
        }

        context.arguments = program_arguments;
        const auto arguments = context.get_argument_list<CLIArgs::ApplicationArguments>();

        if (!context.program->is_readonly()) {
            CLI_PARS_ERR_V(context.register_argument_lists<CLIArgs::GeneratorArguments>(), 1);
            const auto generator_arguments =
                context.get_argument_list<CLIArgs::GeneratorArguments>();

            Permissions::instance()->add_write_path(arguments->goc_path->get<Path>());

            Clear clear;

            if (!was_last_exit_graceful()) {
                PRINT_INFO("GOC: Last exit was ungraceful. Clearing context and files.");
                APP_ERR_COND(
                    clear.run(context) != ProgramError::OK,
                    "Failed to clear the cache directory after an ungraceful exit was "
                    "detected.\nPlease delete the \"%s\" directory to ensure proper operations.",
                    arguments->goc_path->get<Path>().c_str());
            }

            Vector<Path> combined_include_paths;
            auto include_paths = generator_arguments->include_paths->get<Vector<Path>>();

            combined_include_paths.push_back(generator_arguments->root_path->get<Path>());
            combined_include_paths.insert(
                combined_include_paths.end(), include_paths.begin(), include_paths.end());

            auto goc_path = arguments->goc_path->get<Path>();
            auto cache_path = generator_arguments->type_db_path->get<Path>();
            auto generated_path = generator_arguments->generated_path->get<Path>();

            LibraryContext::instance()->set_temporary_path(goc_path);
            LibraryContext::instance()->get_type_db()->set_cache_directory(cache_path);
            LibraryContext::instance()->set_include_paths(combined_include_paths);

            APP_ERR_COND(!init_local_resources(), "Failed to initialize local resources.");

            LibraryContext::instance()->set_remove_macros(
                read_lines(path_concat(goc_path, "macros/macro_remove.txt")));

            LibraryContext::instance()->load_last_modified_times_file(
                path_concat_ext(goc_path, "last_modified", "gocdb"));

            LibraryContext::instance()->load_generated_from_file(
                path_concat_ext(goc_path, "generated_from", "gocdb"));

            LibraryContext::instance()->clean_generated_files();

            LibraryContext::instance()->add_using("godot");

            auto build_num_file = path_concat(goc_path, "last_goc_build_number.txt");
            String build_num = BuildInfo::commit_hash;
            if (file_exists(build_num_file)) {
                if (String last_build_num = read_file(build_num_file);
                    last_build_num != build_num) {
                    APP_ERR_COND(
                        clear.run(context) != ProgramError::OK,
                        "Failed to clear the cache after a change in goc version was "
                        "detected.\nPlease delete the \"%s\" directory to ensure proper "
                        "operations.",
                        goc_path.c_str());
                }
            }

            FileWriter writer(build_num_file);
            writer.write(build_num);
        }
        return 0;
    }

    int Application::run_program(const Ref<IProgram>& p_program)
    {
        APP_ERR_COND(p_program == nullptr, "No program was provided.");
        context.program = p_program;
        APP_ERR_COND(
            context.program->run(context) != ProgramError::OK,
            "Error occurred while executing program \"%s\"",
            context.program->program_name().c_str());
        return 0;
    }

    int Application::cleanup()
    {
        auto arguments = context.get_argument_list<CLIArgs::ApplicationArguments>();

        if (!context.program->is_readonly()) {
            LibraryContext::instance()->save_last_modified_times_file(
                path_concat_ext(arguments->goc_path->get<Path>(), "last_modified", "gocdb"));
            LibraryContext::instance()->save_generated_from_file(
                path_concat_ext(arguments->goc_path->get<Path>(), "generated_from", "gocdb"));
        }

        const int return_code = exit_gracefully(0);
        context = {};
        return return_code;
    }
} // namespace GodotObjectCompiler
