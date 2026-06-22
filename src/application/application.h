/**************************************************************************/
/* application.h                                                          */
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
#include "application_context.h"
#include "library/core/core.h"
#include "programs/program.h"

namespace GodotObjectCompiler
{

    struct RunConfig {
        ApplicationContext context;
        Ref<IProgram> program;
    };

    class Application
    {
      public:
        Application();
        ~Application();

        int run(const Vector<String>& p_arguments);

      private:
        int setup_context(Vector<String> p_arguments);

        int run_program(const Ref<IProgram>& p_program);

        int cleanup();

        bool was_last_exit_graceful() const;

        int exit_gracefully(int p_return_code) const;

        bool init_local_resources() const;

        ApplicationContext context;

        static inline bool has_application = false;
    };

} // namespace GodotObjectCompiler

#define APP_ERR(...)                                                                               \
    print_err(format(__VA_ARGS__));                                                                \
    return 1

#define APP_TOP_LEVEL_ERR(...)                                                                     \
    print_err(format(__VA_ARGS__));                                                                \
    return exit_gracefully(1)

#define APP_ERR_COND(condition, ...)                                                               \
    do {                                                                                           \
        if ((condition)) {                                                                         \
            APP_ERR(__VA_ARGS__);                                                                  \
        }                                                                                          \
    } while (false)

#define APP_TOP_LEVEL_ERR_COND(condition, ...)                                                     \
    do {                                                                                           \
        if ((condition)) {                                                                         \
            APP_TOP_LEVEL_ERR(__VA_ARGS__);                                                        \
        }                                                                                          \
    } while (false)
