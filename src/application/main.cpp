/**************************************************************************/
/* main.cpp                                                               */
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

#include "main.h"

#include "application_context.h"
#include "build_number.h"
#include "compiled_resources/res.gen.h"
#include "library/core/core.h"
#include "library/core/file_system_utilities.h"
#include "library/core/permissions.h"
#include "library/core/resources.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"
#include "library/execution_context.h"
#include "library/parser/parser.h"
#include "library/type_db.h"
#include "programs/all.h"
#include "project.h"

using namespace GodotObjectCompiler;

int main(int argc, char* argv[]) {
  ExecutionContext::instance()->set_error_level(ERROR, FULL);
  Resources::instance()->load_pack(&GOC_Resources::Pack);

  ApplicationContext context;

  for (int i = 1; i < argc; i++) {
    context.application_arguments.emplace_back(argv[i]);
  }

  Ref<IProgram> program = Programs::instance()->find_program(context.application_arguments, context.program_arguments);

  if (!program) {
    Help help;
    ApplicationContext help_context = context;
    if (context.application_arguments.empty()) {
      print_err("Please specify a program to run.");
      APP_ERR_COND(help.run(help_context) != ProgramError::OK, "Failed to run help");
    } else {
      print_err(format("Could not find program \"%s\".", context.application_arguments[0].c_str()));
      help_context.program_arguments = string_split(context.application_arguments[0], "/");
      APP_ERR_COND(help.run(help_context) != ProgramError::OK, "Failed to get help info for program %s",
          context.application_arguments[0].c_str());
      return 1;
    }
    return 1;
  }

  if (program->requires_project()) {
    if (Project project; file_exists(".goc_project") && project.read_from_file(".goc_project")) {
      if (!context.set_from_project(project)) {
        return 1;
      }
    } else if (!context.set_from_application_arguments(context.application_arguments)) {
      return 1;
    }

    APP_ERR_COND(
        !context.paths_root.has_value(), "No project root path specified, but is needed for selected program.");
    APP_ERR_COND(
        !context.paths_include.has_value(), "No include paths were specified, but are needed for selected program.");

    context.paths_include->push_back(*context.paths_root);

    ExecutionContext::instance()->get_type_db()->set_cache_directory(context.paths_cache);
    ExecutionContext::instance()->set_include_paths(*context.paths_include);

    InitLocalResources init_local_resources;
    init_local_resources.run(context);

    ExecutionContext::instance()->set_remove_macros(read_lines(path_absolute(".goc/macros/macro_remove.txt")));
    ExecutionContext::instance()->load_last_modified_times_file(
        path_concat_ext(context.paths_goc, "last_modified", "gocdb"));
    ExecutionContext::instance()->load_generated_from_file(
        path_concat_ext(context.paths_goc, "generated_from", "gocdb"));
    ExecutionContext::instance()->clean_orphan_generated_files();

    if (context.project_target == TARGET_GDEXTENSION) {
      ExecutionContext::instance()->add_using("godot");
    }

    auto build_num_file = path_concat(context.paths_goc, "last_goc_build_number.txt");
    String build_num = GOC_BUILD_NUMBER;
    if (file_exists(build_num_file)) {
      if (String last_build_num = read_file(build_num_file); last_build_num != build_num) {
        Clear clear;
        Ref<ProgramError> error = clear.run(context);
        APP_ERR_COND(error != ProgramError::OK, "goc file clear due to changed build number failed.")
      }
    }

    FileWriter writer(build_num_file);
    writer.write(build_num);
  }

  if (!program->validate_arguments(context)) {
    fmt_print_err("Invalid argument(s) for program %s", program->program_name().c_str());
    ApplicationContext help_context = context;
    help_context.program_arguments = string_split(program->program_name(), "/");
    Help help;
    APP_ERR_COND(help.run(help_context), "Failed to get help info for program %s", program->program_name().c_str());
    return 1;
  }

  APP_ERR_COND(program->run(context) != ProgramError::OK, "Error occurred while executing program.");

  if (program->requires_project()) {
    ExecutionContext::instance()->save_last_modified_times_file(
        path_concat_ext(context.paths_goc, "last_modified", "gocdb"));
    ExecutionContext::instance()->save_generated_from_file(
        path_concat_ext(context.paths_goc, "generated_from", "gocdb"));
  }
  return 0;
};
