/**************************************************************************/
/* test_registry.cpp                                                      */
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

#include "test_registry.h"

#include "library/core/file_system_utilities.h"

namespace GodotObjectCompiler
{

    TestTimer::TestTimer()
    {
        start = std::chrono::high_resolution_clock::now();
    }

    Size TestTimer::elapsed_nanoseconds() const
    {
        using namespace std::chrono;
        time_point<high_resolution_clock> now = high_resolution_clock::now();
        return duration_cast<nanoseconds>(now - start).count();
    }

    bool TestRegistry::register_test(const String& name, TestFunctor functor)
    {
        auto [_, success] = tests.emplace(name, functor);
        return success;
    }

    bool TestRegistry::register_integration_test(const String& name, TestFunctor functor)
    {
        auto [_, success] = integration_tests.emplace(name, functor);
        return success;
    }

    bool TestRegistry::register_module_test(const String& name, TestFunctor functor)
    {
        auto [_, success] = module_tests.emplace(name, functor);
        return success;
    }

    Path TestRegistry::get_test_root_dir()
    {
        return "tests/files/integration_tests";
    }

    Path TestRegistry::get_goc_path()
    {
        return ".goc_tests/.goc";
    }

    Path TestRegistry::get_generated_path()
    {
        return ".goc_tests/.goc/generated";
    }

    Path TestRegistry::get_type_db_path()
    {
        return ".goc_tests/.goc/type_db";
    }

    Path TestRegistry::get_module_goc_path()
    {
        return ".goc_tests/.goc_modules";
    }

    Path TestRegistry::get_module_type_db_path()
    {
        return ".goc_tests/.goc_modules/type_db";
    }

    Path TestRegistry::get_module_generated_path()
    {
        return ".goc_tests/.goc_modules/generated";
    }

    Vector<String>
    TestRegistry::get_integration_test_application_arguments(const ProgramPath& p_program_path)
    {
        Vector<String> result = p_program_path;
        result.emplace_back("-PT=GDExtension");
        result.emplace_back(format("-R=%s", get_test_root_dir().c_str()));
        result.emplace_back(format("-P=%s", get_goc_path().c_str()));
        result.emplace_back(format("-G=%s", get_generated_path().c_str()));
        result.emplace_back(format("-T=%s", get_type_db_path().c_str()));
        result.emplace_back(format("-E=%s", extension_api.c_str()));
        result.emplace_back(format("-SP=%s", source_parser.c_str()));
        result.emplace_back("-L=Info");
        result.emplace_back("-D=Full");

        Vector<Path> paths;
        for (const auto& path : get_include_paths()) {
            paths.push_back(path);
        }

        result.emplace_back(format("-GPP=%s", path_vector_combine(paths, ",").c_str()));
        return result;
    }

    Vector<String>
    TestRegistry::get_module_test_application_arguments(const ProgramPath& p_program_path)
    {
        Vector<String> result = p_program_path;
        result.emplace_back("-PT=Module");
        result.emplace_back("-N=test_module");
        result.emplace_back(format("-R=%s", (godot_root / "modules" / "test_module").c_str()));
        result.emplace_back(format("-P=%s", get_module_goc_path().c_str()));
        result.emplace_back(format("-G=%s", get_module_generated_path().c_str()));
        result.emplace_back(format("-T=%s", get_module_type_db_path().c_str()));
        result.emplace_back(format("-GR=%s", godot_root.c_str()));
        result.emplace_back("-L=Info");
        result.emplace_back("-D=Full");

        Vector<Path> paths;
        for (const auto& path : get_include_paths()) {
            paths.push_back(path);
        }
        paths.push_back(godot_root / "modules" / "test_module");

        result.emplace_back(format("-TI=%s", path_vector_combine(paths, ",").c_str()));
        return result;
    }

    Vector<Path> TestRegistry::get_include_paths()
    {
        return include_paths;
    }

    void TestRegistry::set_include_paths(const Vector<Path>& p_paths)
    {
        include_paths = p_paths;
    }

    void TestRegistry::set_source_parser(const String& p_source_parser)
    {
        source_parser = p_source_parser;
    }

    void TestRegistry::set_godot_root(const Path& p_godot_root)
    {
        godot_root = p_godot_root;
    }

    Path TestRegistry::get_godot_root()
    {
        return godot_root;
    }

    void TestRegistry::set_extension_api(const Path& p_extension_api)
    {
        extension_api = p_extension_api;
    }

    Path TestRegistry::get_extension_api()
    {
        return extension_api;
    }

    const Dictionary<String, TestFunctor>& TestRegistry::get_integration_tests()
    {
        return integration_tests;
    }
    const Dictionary<String, TestFunctor>& TestRegistry::get_module_tests()
    {
        return module_tests;
    }

    const Dictionary<String, TestFunctor>& TestRegistry::get_tests()
    {
        return tests;
    }

    bool TestRegister::operator<<(TestFunctor functor) const
    {
        return TestRegistry::instance()->register_test(name, std::move(functor));
    }

    bool IntegrationTestRegister::operator<<(TestFunctor functor) const
    {
        return TestRegistry::instance()->register_integration_test(name, std::move(functor));
    }

    bool ModuleTestRegister::operator<<(TestFunctor functor) const
    {
        return TestRegistry::instance()->register_module_test(name, std::move(functor));
    }

} // namespace GodotObjectCompiler
