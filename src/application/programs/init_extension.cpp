/**************************************************************************/
/* init_extension.cpp                                                     */
/*                        ___  ___  ___   ___ _____                       */
/*                       / __|/ _ \|   \ / _ \_   _|                      */
/*                      | (_ | (_) | |) | (_) || |                        */
/*                       \___|\___/|___/ \___/ |_|                        */
/*   ___  ___    _ ___ ___ _____    ___ ___  __  __ ___ ___ _    ___ ___  */
/*  / _ \| _ )_ | | __/ __|_   _|  / __/ _ \|  \/  | _ \_ _| |  | __| _ \ */
/* | (_) | _ \ || | _| (__  | |   | (_| (_) | |\/| |  _/| || |__| _||   / */
/*  \___/|___/\__/|___\___| |_|    \___\___/|_|  |_|_| |___|____|___|_|_\ */
/*                                                                        */
/*              This file is part of godot-object-compiler                */
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

#include "init_extension.h"

#include "init_tools.h"
#include "library/core/file_system_utilities.h"
#include "library/core/permissions.h"
#include "library/core/process.h"
#include "library/core/resources.h"

namespace GodotObjectCompiler
{
    const auto template_path = "res:/templates/extension";
    const auto tools_path = "res:/tools";
    const auto extension_name_replace_key = "EXTENSION_NAME";
    const auto api_version_replace_key = "GOC_GODOTCPP_API_VERSION";

#ifndef TESTS_ENABLED
    bool git_exists()
    {
        String _;
        return run_subprocess("git version", _) == 0;
    }

    bool is_git_repository()
    {
        String _;
        return run_subprocess("git status", _) == 0;
    }

    bool git_submodule_add(const String& p_repo, const Path& p_path)
    {
        String output;
        if (const int result = run_subprocess(
                format("git submodule add %s %s", p_repo.c_str(), p_path.c_str()), output);
            result == 0) {
            print(output);
            return true;
        }
        print_err(output);
        return false;
    }
#endif

    CommandLineArgumentParseResult
    InitExtension::register_required_arguments(ApplicationContext& p_context) const
    {
        return p_context.register_argument_lists<InitExtensionArguments>();
    }

    Ref<ProgramError> InitExtension::execute(ApplicationContext& p_context)
    {
        const auto arguments = p_context.get_argument_list<InitExtensionArguments>();

        const auto path = arguments->path->get<Path>();
        const auto name = arguments->extension_name->has_value()
                              ? arguments->extension_name->get<String>()
                              : path.filename().string();
        const auto api_version = arguments->godot_api_version->get<String>();
        Permissions::instance()->add_write_path(path);

#ifndef TESTS_ENABLED
        if (const auto godot_cpp_git = arguments->godot_cpp_git->get<String>();
            !godot_cpp_git.empty()) {
            PROG_ERR_COND(!git_exists(), "Tried to checkout godot-cpp but git was not found.");
            PROG_ERR_COND(
                !is_git_repository(),
                "Tried to checkout godot-cpp as a submodule but \"%s\" is not a git repository",
                path_cwd().c_str());
            PROG_ERR_COND(
                !git_submodule_add(godot_cpp_git, path / "godot-cpp"),
                "Failed to checkout godot-cpp submodule.");
        }
#endif

        auto apply_replacement = [name, api_version](String& p_content) {
            string_reinplace(p_content, extension_name_replace_key, name);
            string_reinplace(p_content, api_version_replace_key, api_version);
        };

        for (const auto& resource : Resources::instance()->resources_recursive(template_path)) {
            auto content = Resources::instance()->load_text_resource(resource);
            apply_replacement(content);

            auto relative_path = path_relative(resource, template_path).string();
            apply_replacement(relative_path);

            FileWriter writer(path / Path(relative_path));
            writer.write(content);
        }

        for (const auto& tool : Resources::instance()->resources_recursive(tools_path)) {
            if (tool.extension() != ".cmake") {
                continue;
            }

            auto content = Resources::instance()->load_text_resource(tool);
            auto file_path = path / "cmake" / path_relative(tool, tools_path);
            FileWriter writer(file_path);
            writer.write(content);
        }

        {
            FileWriter writer(path / "cmake" / ".gdignore");
            writer.write("");
        }

        return ProgramError::OK;
    }
} // namespace GodotObjectCompiler
