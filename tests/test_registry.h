/**************************************************************************/
/* test_registry.h                                                        */
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

#pragma once
#include <utility>

#include "application/programs/program.h"
#include "library/core/core.h"
#include "library/core/string_writer.h"
#include "library/tree/syntax/function.h"

namespace GodotObjectCompiler
{

    enum TestResult { TEST_RESULT_SUCCESS, TEST_RESULT_FAILURE, TEST_RESULT_IGNORED };

    using TestFunctor = std::function<TestResult()>;

    class TestRegistry
    {
      public:
        static TestRegistry* instance()
        {
            static TestRegistry instance;
            return &instance;
        }

        bool register_test(const String& name, TestFunctor functor);
        bool register_integration_test(const String& name, TestFunctor functor);

        String test_generated_folder();
        String test_root_folder();

        Vector<String> get_test_application_arguments(const ProgramPath& p_program_path);

        Vector<String> get_integration_tests_godot_cpp_include_paths();
        void set_integration_tests_godot_cpp_include_paths(const Vector<String>& p_paths);

        void set_extension_api(const String& p_extension_api);
        String get_extension_api();

        const Dictionary<String, TestFunctor>& get_tests();
        const Dictionary<String, TestFunctor>& get_integration_tests();

      private:
        String extension_api;
        Vector<String> include_paths;
        Dictionary<String, TestFunctor> tests;
        Dictionary<String, TestFunctor> integration_tests;
    };

    class TestRegister
    {
        String name;

      public:
        explicit TestRegister(String name) : name(std::move(name))
        {
        }

        bool operator<<(TestFunctor functor) const;
    };

    class IntegrationTestRegister
    {
        String name;

      public:
        explicit IntegrationTestRegister(String name) : name(std::move(name))
        {
        }

        bool operator<<(TestFunctor functor) const;
    };

} // namespace GodotObjectCompiler

// clang-format off
#define GOC_TEST(name)                               \
  static inline bool __##name##__test_registered__ = \
      GodotObjectCompiler::TestRegister(#name)       \
      << []() -> GodotObjectCompiler::TestResult

#define GOC_INTEGRATION_TEST(name)                        \
  static inline bool __##name##__test_registered__ =      \
      GodotObjectCompiler::IntegrationTestRegister(#name) \
      << []() -> GodotObjectCompiler::TestResult
// clang-format on

#define GOC_TEST_IGNORE() return GodotObjectCompiler::TEST_RESULT_IGNORED;

#define GOC_TEST_ASSERT(condition, ...)                                                            \
    if (!(condition)) {                                                                            \
        fmt_print_err(__VA_ARGS__);                                                                \
        return GodotObjectCompiler::TEST_RESULT_FAILURE;                                           \
    }

#define GOC_TEST_EQ(a, b, ...)                                                                     \
    if (!((a) == (b))) {                                                                           \
        GodotObjectCompiler::StreamWriter writer;                                                  \
        writer.write(format(__VA_ARGS__));                                                         \
        writer.write(" Expected to be ");                                                          \
        writer.write_generic(b);                                                                   \
        writer.write_generic(" but was "), writer.write_generic(a);                                \
        writer.write("."), print_err(writer.get_string());                                         \
        return GodotObjectCompiler::TEST_RESULT_FAILURE;                                           \
    }

#define GOC_TEST_NEQ(a, b, ...)                                                                    \
    if (((a) == (b))) {                                                                            \
        GodotObjectCompiler::StreamWriter writer;                                                  \
        writer.write(format(__VA_ARGS__));                                                         \
        writer.write(" Expected not equal to ");                                                   \
        writer.write_generic(b);                                                                   \
        writer.write_generic(" but was "), writer.write_generic(a);                                \
        writer.write("."), print_err(writer.get_string());                                         \
        return GodotObjectCompiler::TEST_RESULT_FAILURE;                                           \
    }

#define GOC_TEST_PARSE_FILE(path)                                                                  \
    Ref<Namespace> global_namespace = node_new<Namespace>();                                       \
    {                                                                                              \
        TreeSitterParser parser;                                                                   \
        Ref<ParserError> error = parser.parse_file(path, global_namespace);                        \
        GOC_TEST_EQ(error, ParserError::OK, "Parser error occurred");                              \
    }                                                                                              \
    GOC_TEST_ASSERT(global_namespace, "Global Namespace is invalid.");
