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
#include "compiled_resources/res.gen.h"
#include "library/core/core.h"
#include "library/core/file_system_utilities.h"
#include "library/core/permissions.h"
#include "library/core/resources.h"
#include "library/execution_context.h"
#include "library/parser/parser.h"
#include "library/type_db.h"
#include "programs/init.h"
#include "project.h"

using namespace GodotObjectCompiler;

int main(int argc, char* argv[]) {
  ApplicationContext context;
  Resources::instance()->load_pack(&GOC_Resources::Pack);
  ExecutionContext::instance()->set_error_level(ERROR, FULL);

  Vector<String> application_arguments;

  for (int i = 1; i < argc; i++) {
    context.application_arguments.emplace_back(argv[i]);
  }

  Ref<IProgram> program = Programs::instance()->find_program(context.application_arguments, context.program_arguments);

  if (!program) {
    if (context.application_arguments.empty()) {
      print_err("Please specify a program to run.");
    } else {
      print_err(format("Could not find program \"%s\".", context.application_arguments[0].c_str()));
    }
    return 1;
  }

  if (program->requires_project()) {
    Project project;
    if (!project.read_from_file("goc_project.conf")) {
      print_err("Could not find project file.");
      return 1;
    }

    if (!context.set_from_project(project)) {
      return 1;
    }

    TypeDB::instance()->set_cache_directory(context.paths_cache);
    ExecutionContext::instance()->set_remove_macros(read_lines(path_absolute(".goc/macros/macro_remove.txt")));
    ExecutionContext::instance()->set_include_paths(context.paths_include);
    ExecutionContext::instance()->load_last_modified_times_file(
        path_concat_ext(context.paths_goc, "last_modified", "gocdb"));
    ExecutionContext::instance()->load_generated_from_file(
        path_concat_ext(context.paths_goc, "generated_from", "gocdb"));
    ExecutionContext::instance()->clean_orphan_generated_files();
  }

#ifdef DEV_BUILD
  Permissions::instance()->add_write_path("resources");
#endif

  Ref<ProgramError> error = program->run(context);
  if (error != ProgramError::OK) {
    return 1;
  }

  if (program->requires_project()) {
    ExecutionContext::instance()->save_last_modifed_times_file(
        path_concat_ext(context.paths_goc, "last_modified", "gocdb"));
    ExecutionContext::instance()->save_generated_from_file(
        path_concat_ext(context.paths_goc, "generated_from", "gocdb"));
  }
  return 0;
};
