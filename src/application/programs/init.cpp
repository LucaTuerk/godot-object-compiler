/**************************************************************************/
/* init.cpp                                                               */
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

#include "init.h"

#include "application/application.h"
#include "application/project.h"
#include "library/core/file_system_utilities.h"
#include "library/core/permissions.h"
#include "library/core/resources.h"
#include "library/core/string_utilities.h"

namespace GodotObjectCompiler
{

    Ref<ProgramError> Init::run(ApplicationContext& p_context)
    {
        Vector<String> files = directory_files(path_cwd());
        bool project_exists = std::any_of(files.begin(), files.end(), [](const String& path) {
            return string_suffix(path, "goc_project");
        });

        PROG_ERR_COND(project_exists, "A GOC project already exists at this location");

        print_ln(format("\nInitializing project at location \"%s\"\n", path_cwd().c_str()));
        String project_name = input("|- Enter Project Name: ");

        print_ln("|  Paths:");
        String root_folder = input("   |- Sources Root (default: \"%s\"): ", "src");
        String goc_folder = input("   |- GOC Root: (default: \"%s\"): ", ".goc");
        String generate_folder =
            input("   |- GOC Generated (default: \"%s\"): ", path_concat(goc_folder, "generated"));
        String cache_root =
            input("   |- GOC Cache (default: \"%s\"): ", path_concat(goc_folder, "cache"));
        String godot_cpp =
            input("   |- Godot CPP Include (default: \"%s\"): ", "godot-cpp/include");
        String godot_cpp_gen =
            input("   |- Godot CPP Generated Include (default: \"%s\"): ", "godot-cpp/gen/include");
        String extension_api = input(
            "   |- Extension API (default: \"%s\"): ",
            path_concat_ext(path_concat(godot_cpp, "gdextension"), "extension_api", "json"));

        String root_absolute = path_absolute(root_folder);
        String goc_absolute = path_absolute(goc_folder);
        String generated_absolute = path_absolute(generate_folder);
        String cache_absolute = path_absolute(cache_root);

        print_ln("\n| Summary");
        print_ln(format("   |  Project Name: %s", project_name.c_str()));
        print_ln(format("   |  Root: %s", root_absolute.c_str()));
        print_ln(format("   |  GOC Root: %s", goc_absolute.c_str()));
        print_ln(format("   |  Generated: %s", generated_absolute.c_str()));
        print_ln(format("   |  Cache: %s", cache_absolute.c_str()));
        print_ln(format("   |  Extension API: %s", extension_api.c_str()));
        print_ln(format("   |  Godot CPP Include: %s", godot_cpp.c_str()));
        print_ln(format("   |  Godot CPP Generated Include: %s", godot_cpp_gen.c_str()));
        print_ln("");

        if (input("|? Accept? (y/%s) ", "n") == "y") {
            Permissions::instance()->add_write_path(path_cwd());
            Project project;
            project.project_target = TARGET_GDEXTENSION;
            project.paths_root = root_folder;
            project.paths_generated = generate_folder;
            project.paths_cache = cache_root;
            project.path_extension_api = extension_api;
            project.paths_godot_cpp_include = {godot_cpp, godot_cpp_gen};

            String project_file_path = format("%s.goc_project", project_name.c_str());
            project.write_to_file(project_file_path);

            if (!p_context.set_from_project(project)) {
                return make_ref<ProgramError>(
                    ERROR,
                    "Failed to setup ApplicationContext from provided project settings. Abort!");
            }

            String _ = input(format(
                "\n|-> Successfully generated project file \"%s\" (enter to exit)",
                path_absolute(project_file_path).c_str()));
        }
        return ProgramError::OK;
    }

} // namespace GodotObjectCompiler
