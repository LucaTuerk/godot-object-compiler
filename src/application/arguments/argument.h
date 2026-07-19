/**************************************************************************/
/* argument.h                                                             */
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
#include "library/core/core.h"
#include "library/core/string_utilities.h"

namespace GodotObjectCompiler
{
    class CommandLineArgument;

    template <typename T> class ICommandLineArgumentParser
    {
      public:
        using ValueType = T;

        virtual ~ICommandLineArgumentParser() = default;

        virtual Opt<T> parse_argument(const String& p_argument) = 0;

        virtual String get_argument_type_string() = 0;
    };

    class CommandLineArgumentParseResult
    {
      public:
        operator bool() const;

        CommandLineArgumentParseResult operator+(const CommandLineArgumentParseResult& other) const;

        Vector<Ref<CommandLineArgument>> get_missing_arguments();

        [[nodiscard]]
        String get_error_message() const;

      private:
        CommandLineArgumentParseResult() = default;

        Vector<Ref<CommandLineArgument>> missing_required;

        friend class CommandLineArgument;
        friend class ApplicationContext;
    };

    class CommandLineArgument
    {
      public:
        CommandLineArgument() = default;

        CommandLineArgument(const CommandLineArgument& other) = default;

        CommandLineArgument(CommandLineArgument&& other) noexcept = default;

        template <typename P>
        static Ref<CommandLineArgument>
        positional(const Ref<P>& p_parser, const String& p_description);

        template <typename P>
        static Ref<CommandLineArgument> optional(
            const Ref<P>& p_parser, const String& p_name, const String& p_short_name,
            const String& p_description);

        template <typename P>
        static Ref<CommandLineArgument> defaulted(
            const Ref<P>& p_parser, const String& p_name, const String& p_short_name,
            const String& p_description, const typename P::ValueType& p_default);

        template <typename P>
        static Ref<CommandLineArgument> required(
            const Ref<P>& p_parser, const String& p_name, const String& p_short_name,
            const String& p_description);

        void parse_arguments(const Vector<String>& p_arguments);

        [[nodiscard]]
        static CommandLineArgumentParseResult parse(
            const Vector<Ref<CommandLineArgument>>& p_arguments,
            const Vector<String>& p_string_arguments);

        [[nodiscard]] bool is_required() const;

        [[nodiscard]] bool is_positional() const;

        [[nodiscard]] bool has_value() const;

        template <typename T> [[nodiscard]] bool value_equals(const T& p_value) const;

        template <typename T> [[nodiscard]] T get(Index p_index = 0) const;

        template <typename T> [[nodiscard]] Vector<T> get_vector() const;

        [[nodiscard]] Size size() const;

        [[nodiscard]] String get_name() const;

        [[nodiscard]] String get_short_name() const;

        [[nodiscard]] String get_argument_type() const;

        [[nodiscard]] String get_description();

        static bool validate_arguments(const Vector<CommandLineArgument>& p_arguments);

      private:
        template <typename T> void parse_arguments_t(const Vector<String>& p_arguments);

        template <typename T> String get_argument_type_t() const;

        [[nodiscard]] bool has_correct_name(const String& p_argument) const;

        [[nodiscard]] String get_argument_part(const String& p_argument) const;

        std::function<void(CommandLineArgument*, const Vector<String>&)> parse_arguments_func;
        std::function<String(const CommandLineArgument*)> get_argument_type_func;
        std::type_index type = typeid(void);
        Vector<std::any> values;
        std::any parser;
        String name;
        String short_name;
        String name_prefix;
        String short_name_prefix;
        String description;
        bool value_available = false;
        bool required_arg = false;
        bool positional_arg = false;
    };

    class ICommandLineArgumentList
    {
      public:
        virtual ~ICommandLineArgumentList() = default;
        [[nodiscard]] virtual Vector<Ref<CommandLineArgument>> get_arguments() const = 0;

        template <typename... Lists>
        static Vector<Ref<CommandLineArgument>> merge_arguments(Lists&&... p_lists);
    };

    template <typename T> std::type_index cli_arg_type()
    {
        if constexpr (std::is_enum_v<T>) {
            return typeid(int);
        } else {
            return typeid(T);
        }
    }

    template <typename P>
    Ref<CommandLineArgument>
    CommandLineArgument::positional(const Ref<P>& p_parser, const String& p_description)
    {
        Ref<CommandLineArgument> result = make_ref<CommandLineArgument>();
        result->positional_arg = true;
        result->parser =
            std::dynamic_pointer_cast<ICommandLineArgumentParser<typename P::ValueType>>(p_parser);
        result->type = typeid(typename P::ValueType);
        result->description = p_description;
        result->parse_arguments_func =
            &CommandLineArgument::parse_arguments_t<typename P::ValueType>;
        return result;
    }
    template <typename P>
    Ref<CommandLineArgument> CommandLineArgument::optional(
        const Ref<P>& p_parser, const String& p_name, const String& p_short_name,
        const String& p_description)
    {
        Ref<CommandLineArgument> result = make_ref<CommandLineArgument>();
        result->type = typeid(typename P::ValueType);
        result->parser =
            std::dynamic_pointer_cast<ICommandLineArgumentParser<typename P::ValueType>>(p_parser);
        result->name = p_name;
        result->short_name = p_short_name;
        result->name_prefix = format("--%s=", p_name.c_str());
        result->short_name_prefix = format("-%s=", p_short_name.c_str());
        result->description = p_description;
        result->parse_arguments_func =
            &CommandLineArgument::parse_arguments_t<typename P::ValueType>;
        result->get_argument_type_func =
            &CommandLineArgument::get_argument_type_t<typename P::ValueType>;
        return result;
    }

    template <typename P>
    Ref<CommandLineArgument> CommandLineArgument::defaulted(
        const Ref<P>& p_parser, const String& p_name, const String& p_short_name,
        const String& p_description, const typename P::ValueType& p_default)
    {
        Ref<CommandLineArgument> result = make_ref<CommandLineArgument>();
        result->type = typeid(typename P::ValueType);
        result->parser =
            std::dynamic_pointer_cast<ICommandLineArgumentParser<typename P::ValueType>>(p_parser);
        result->name = p_name;
        result->short_name = p_short_name;
        result->name_prefix = format("--%s=", p_name.c_str());
        result->short_name_prefix = format("-%s=", p_short_name.c_str());
        result->description = p_description;
        result->value_available = true;
        result->values.push_back(p_default);
        result->parse_arguments_func =
            &CommandLineArgument::parse_arguments_t<typename P::ValueType>;
        result->get_argument_type_func =
            &CommandLineArgument::get_argument_type_t<typename P::ValueType>;
        return result;
    }

    template <typename P>
    Ref<CommandLineArgument> CommandLineArgument::required(
        const Ref<P>& p_parser, const String& p_name, const String& p_short_name,
        const String& p_description)
    {
        Ref<CommandLineArgument> result = make_ref<CommandLineArgument>();
        result->type = typeid(typename P::ValueType);
        result->parser =
            std::dynamic_pointer_cast<ICommandLineArgumentParser<typename P::ValueType>>(p_parser);
        result->name = p_name;
        result->short_name = p_short_name;
        result->name_prefix = format("--%s=", p_name.c_str());
        result->short_name_prefix = format("-%s=", p_short_name.c_str());
        result->description = p_description;
        result->required_arg = true;
        result->parse_arguments_func =
            &CommandLineArgument::parse_arguments_t<typename P::ValueType>;
        result->get_argument_type_func =
            &CommandLineArgument::get_argument_type_t<typename P::ValueType>;
        return result;
    }

    template <typename T>
    void CommandLineArgument::parse_arguments_t(const Vector<String>& p_arguments)
    {
        PANIC_COND(
            parser.type() != typeid(Ref<ICommandLineArgumentParser<T>>), "Invalid parser type.");

        auto is_named_arg = [](const String& p_argument) { return string_prefix(p_argument, "-"); };

        auto parser_t = std::any_cast<Ref<ICommandLineArgumentParser<T>>>(parser);
        Index positional_i = 0;

        for (const auto& argument : p_arguments) {
            if (has_correct_name(argument)) {
                Opt<T> opt_value = parser_t->parse_argument(get_argument_part(argument));
                if (opt_value.has_value()) {
                    value_available = true;
                    values.clear();
                    values.push_back(*opt_value);
                } else {
                    value_available = false;
                    values.clear();
                }
            } else if (positional_arg && !is_named_arg(argument)) {
                Opt<T> opt_value = parser_t->parse_argument(argument);
                if (opt_value.has_value()) {
                    value_available = true;
                    if (values.size() >= positional_i) {
                        values.resize(positional_i + 1);
                    }

                    values[positional_i++] = *opt_value;
                } else {
                    value_available = false;
                    values.clear();
                }
            }
        }
    }

    template <typename T> String CommandLineArgument::get_argument_type_t() const
    {
        PANIC_COND(
            parser.type() != typeid(Ref<ICommandLineArgumentParser<T>>), "Invalid parser type.");
        auto parser_t = std::any_cast<Ref<ICommandLineArgumentParser<T>>>(parser);
        return parser_t->get_argument_type_string();
    }

    template <typename T>
    void merge_arguments_helper(Vector<Ref<CommandLineArgument>>& r_result, T* p_list)
    {
        auto* list = dynamic_cast<const ICommandLineArgumentList*>(p_list);
        PANIC_COND(list == nullptr, "Invalid type.");

        auto arguments = list->get_arguments();
        r_result.insert(r_result.end(), arguments.begin(), arguments.end());
    }

    template <typename T, typename... Lists>
    void merge_arguments_helper(
        Vector<Ref<CommandLineArgument>>& r_result, T* p_list, Lists&&... p_lists)
    {
        merge_arguments_helper(r_result, p_list);
        merge_arguments_helper(r_result, p_lists...);
    }

    template <typename... Lists>
    Vector<Ref<CommandLineArgument>> ICommandLineArgumentList::merge_arguments(Lists&&... p_lists)
    {
        Vector<Ref<CommandLineArgument>> result;
        merge_arguments_helper(result, p_lists...);
        return result;
    }

    template <typename T> bool CommandLineArgument::value_equals(const T& p_value) const
    {
        PANIC_COND(type != typeid(T), "Invalid type access.");
        if (!has_value()) {
            return false;
        }
        return get<T>() == p_value;
    }

    template <typename T> T CommandLineArgument::get(Index p_index) const
    {
        PANIC_COND(
            !positional_arg && p_index != 0, "%s: Invalid positional access.", get_name().c_str());
        PANIC_COND(
            !value_available || p_index >= values.size(),
            "%s: Invalid value access. No value available.", get_name().c_str());
        PANIC_COND(
            values[p_index].type() != typeid(T), "%s: Invalid argument type access.",
            get_name().c_str());
        return std::any_cast<T>(values[p_index]);
    }

    template <typename T> Vector<T> CommandLineArgument::get_vector() const
    {
        PANIC_COND(
            !positional_arg, "%s: Invalid vector access on non position argument.",
            get_name().c_str());

        Vector<T> result;
        for (const auto& argument : values) {
            PANIC_COND(
                argument.type() != typeid(T), "%s: Invalid argument type access.",
                get_name().c_str());
            result.push_back(std::any_cast<T>(argument));
        }
        return result;
    }
} // namespace GodotObjectCompiler

#define CLI_PARS_ERR_V(result, value)                                                              \
    do {                                                                                           \
        if (!result) {                                                                             \
            return value;                                                                          \
        }                                                                                          \
    } while (false)
