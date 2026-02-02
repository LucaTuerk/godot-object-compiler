#include "main.h"

#include "application_context.h"
#include "compiled_resources/res.gen.h"
#include "library/core/core.h"
#include "library/core/helpers.h"
#include "library/core/resources.h"
#include "library/core/string_writer.h"
#include "library/parser/parser.h"
#include "library/tree/output/output_transformator.h"
#include "library/type_db.h"
#include "library_godot/assumptions.h"
#include "library_godot/generated_assumptions/parameter_types.h"
#include "library_godot/generators/godot_macro_include_generator.h"
#include "programs/generate_assumptions.h"
#include "programs/generate_bindings.h"
#include "programs/generate_type_db.h"
#include "programs/init.h"
#include "project.h"

using namespace GodotObjectCompiler;

int main(int argc, char* argv[]) {
  ApplicationContext context;
  Resources::instance()->load_pack(&GOC_Resources::Pack);
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

  if (program->program_name() == "init") {
    Ref<ProgramError> init_error = program->run(context);
    if (init_error != ProgramError::OK) {
      return 1;
    }
    return 0;
  }

  Project project;
  if (!project.read_from_file("./goc_project.conf")) {
    print_err("Could not find project file.");
    return 1;
  }

  context.paths_root = path_absolute(project.paths_root);
  context.paths_generated = path_absolute(project.paths_generated);
  context.paths_cache = path_absolute(project.paths_cache);

  context.paths_include = project.paths_include;
  if (std::find(context.paths_include.begin(), context.paths_include.end(), project.paths_root) ==
      context.paths_include.end()) {
    context.paths_include.push_back(project.paths_root);
  }
  context.files_input = directory_files_recursive(project.paths_root);

  std::transform(
      context.paths_include.begin(), context.paths_include.end(), context.paths_include.begin(), &path_absolute);
  std::transform(context.files_input.begin(), context.files_input.end(), context.files_input.begin(), &path_absolute);

  if (!context.valid()) {
    return 1;
  }

  TypeDB::instance()->set_cache_directory(context.paths_cache);
  ExecutionContext::instance()->set_error_level(ERROR, FULL);
  ExecutionContext::instance()->set_remove_macros(read_lines(path_absolute(".goc/macros/macro_remove.txt")));
  ExecutionContext::instance()->set_include_paths(context.paths_include);

  Ref<ProgramError> error = program->run(context);
  if (error != ProgramError::OK) {
    return 1;
  }

  return 0;
};
