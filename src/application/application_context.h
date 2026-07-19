/**************************************************************************/
/* application_context.h                                                  */
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
#include "arguments/argument.h"
#include "library/core/core.h"

namespace GodotObjectCompiler
{

    class IProgram;

    class ApplicationContext
    {
      public:
        Vector<String> arguments;

        Ref<IProgram> program;

        template <typename... Args> CommandLineArgumentParseResult register_argument_lists();

        template <typename T> Ref<T> get_argument_list() const;

      private:
        template <typename T, typename... Args>
        CommandLineArgumentParseResult register_argument_lists_helper();

        template <class... Args, typename = std::enable_if_t<(sizeof...(Args) == 0)>>
        static CommandLineArgumentParseResult register_argument_lists_helper();

        template <
            typename T, typename = std::enable_if_t<std::is_base_of_v<ICommandLineArgumentList, T>>>
        CommandLineArgumentParseResult register_argument_list(const Ref<T>& p_argument_list);

        Dictionary<TypeIndex, Ref<ICommandLineArgumentList>> argument_lists;
    };

    template <typename T, typename>
    CommandLineArgumentParseResult
    ApplicationContext::register_argument_list(const Ref<T>& p_argument_list)
    {
        if (argument_lists.find(typeid(T)) != argument_lists.end()) {
            return {};
        }

        Ref<ICommandLineArgumentList> argument_list =
            std::dynamic_pointer_cast<ICommandLineArgumentList>(p_argument_list);
        const auto result = CommandLineArgument::parse(argument_list->get_arguments(), arguments);

        if (result) {
            argument_lists[typeid(T)] = argument_list;
        }
        return result;
    }

    template <typename... Args, typename>
    CommandLineArgumentParseResult ApplicationContext::register_argument_lists_helper()
    {
        return {};
    }

    template <typename T, typename... Args>
    CommandLineArgumentParseResult ApplicationContext::register_argument_lists_helper()
    {
        return register_argument_list<T>(make_ref<T>()) + register_argument_lists_helper<Args...>();
    }

    template <typename... Args>
    CommandLineArgumentParseResult ApplicationContext::register_argument_lists()
    {
        return register_argument_lists_helper<Args...>();
    }

    template <typename T> Ref<T> ApplicationContext::get_argument_list() const
    {
        auto itr = argument_lists.find(typeid(T));

        if (itr == argument_lists.end()) {
            return nullptr;
        }

        return std::dynamic_pointer_cast<T>(itr->second);
    }
} // namespace GodotObjectCompiler
