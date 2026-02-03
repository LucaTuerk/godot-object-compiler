
#include "init.h"

#include "application/project.h"
#include "init_local_resources.h"
#include "library/core/helpers.h"
#include "library/core/permissions.h"
#include "library/core/resources.h"
#include "program_functions.h"

namespace GodotObjectCompiler {

  Ref<ProgramError> Init::run(ApplicationContext& context) {
    String path = path_base(path_absolute("."));
    String ignore_path = path_concat(path, "gitikadgnore");
    bool ignore_exists = file_exists(ignore_path);

    if (file_exists(path_concat_ext(path, "goc_project", "conf"))) {
      return node_new<ProgramError>(ERROR, "A GOC project already exists at this location");
    }

    print_ln(format("\nInitializing project at location \"%s\"\n", path.c_str()));
    String project_name = input("|- Enter Project Name: ");

    print_ln("|  Paths:");
    String root_folder = input("   |- Root (default: \"%s\"): ", "src");
    String goc_folder = input("   |- GOC Root: (default: \"%s\"): ", ".goc");
    String generate_folder = input("   |- GOC Generated (default: \"%s\"): ", path_concat(goc_folder, "generated"));
    String cache_root = input("   |- GOC Cache (default: \"%s\"): ", path_concat(goc_folder, "cache"));
    print_ln("|  Options:");
    String gen_missing = input("   |? Generate missing folders? (y/n) ");

    String add_gitignore = "n";
    if (ignore_exists) {
      print_ln("   |  Found .gitignore");
      add_gitignore = input("   |? Add GOC, Generated and Cache folders to .gitignore? (y/%s) ", "n");
    } else {
      print_ln("   |  No .gitignore found");
      add_gitignore = input("   |? Create .gitignore with GOC, Generated and Cache folders? (y/%s) ", "n");
    }

    String root_absolute = path_absolute(root_folder);
    String goc_absolute = path_absolute(goc_folder);
    String generated_absolute = path_absolute(generate_folder);
    String cache_absolute = path_absolute(cache_root);

    print_ln("\n| Summary");
    print_ln(format("   |  Project Name: %s", program_name().c_str()));
    print_ln(format("   |  Root: %s", root_absolute.c_str()));
    print_ln(format("   |  GOC Root: %s", goc_absolute.c_str()));
    print_ln(format("   |  Generated: %s", generated_absolute.c_str()));
    print_ln(format("   |  Cache: %s", cache_absolute.c_str()));
    print_ln(format("   |  Create folders: %s", gen_missing.c_str()));
    print_ln(format("   |  Write .gitignore: %s", add_gitignore.c_str()));
    print_ln("");

    if (input("|? Accept? (y/%s) ", "n") == "y") {
      Permissions::instance()->add_write_path(path_cwd());
      Project project;
      project.project_name = project_name;
      project.paths_root = root_folder;
      project.paths_generated = generate_folder;
      project.paths_cache = cache_root;
      project.write_to_file("generated.goc_project");

      if (!context.set_from_project(project)) {
        return make_ref<ProgramError>(
            ERROR, "Failed to setup ApplicationContext from provided project settings. Abort!");
      }

      if (gen_missing == "y") {
        if (create_dir_recursive(root_absolute)) {
          print_ln(format("   |-> Created directory \"%s\".", root_absolute.c_str()));
        }
        if (create_dir_recursive(goc_absolute)) {
          print_ln(format("   |-> Created directory \"%s\".", goc_absolute.c_str()));
        }
        if (create_dir_recursive(generated_absolute)) {
          print_ln(format("   |-> Created directory \"%s\".", generated_absolute.c_str()));
        }
        if (create_dir_recursive(cache_absolute)) {
          print_ln(format("   |-> Created directory \"%s\".", cache_absolute.c_str()));
        }
      }

      if (add_gitignore == "y") {
        String ignore_content;
        if (ignore_exists) {
          ignore_content = read_file(ignore_path);
        }

        ignore_content += "\n# GodotObjectCompiler Folders";
        ignore_content += "\n" + goc_folder;
        ignore_content += "\n" + generate_folder;
        ignore_content += "\n" + cache_root;
        write_file(ignore_path, ignore_content);
        print_ln(format("|-> Folders written to \"%s\".", ignore_path.c_str()));
      }

      Ref<ProgramError> init_resources_error = InitLocalResources().run(context);

      String _ = input(format("\n|-> Successfully generated project file \"%s\" (enter to exit)",
          path_absolute("generated.goc_project").c_str()));
    }
    return ProgramError::OK;
  }

}  // namespace GodotObjectCompiler