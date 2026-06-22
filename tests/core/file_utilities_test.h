/**************************************************************************/
/* file_utilities_test.h                                                  */
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
#include "library/core/file_system_utilities.h"
#include "library/core/permissions.h"
#include "library/core/string_utilities.h"
#include "test_registry.h"

using namespace GodotObjectCompiler;

GOC_TEST(FileReadWriteTest)
{
    String base = path_absolute(".goc_tests/tmp");

    for (Size i = 0; i < 100; ++i) {
        String filename = path_concat_ext(base, generate_random_string(10), "txt");
        Size size = i * 100;
        String content = generate_random_string(size);
        write_file(filename, content);
        GOC_TEST_ASSERT(
            file_exists(filename), "File \"%s\" does not exist after write", filename.c_str());

        String read_content = read_file(filename);
        GOC_TEST_EQ(content, read_content, "Invalid read content from \"%s\"", filename.c_str());
        GOC_TEST_EQ(read_content.size(), size, "Invalid read size from \"%s\"", filename.c_str());

        GOC_TEST_ASSERT(remove_file(filename), "Failed to remove file \"%s\"", filename.c_str());
        GOC_TEST_ASSERT(
            !file_exists(filename), "File \"%s\" still exists after sucessfull remove call",
            filename.c_str());
    }

    return TEST_RESULT_SUCCESS;
};

GOC_TEST(EnsurePermission)
{
    Vector<String> paths = {".goc_tests", "/home", "/bin"};

    for (const String& path : paths) {
        bool expect_fail = !Permissions::instance()->is_allowed_write_path(path);
        bool failed = false;
        try {
            Permissions::instance()->ensure_is_allowed_write_path(path);
        } catch (std::exception& e) {
            failed = true;
        }

        GOC_TEST_EQ(expect_fail, failed, "Unexpected write permission.");
    }
    return TEST_RESULT_SUCCESS;
};
