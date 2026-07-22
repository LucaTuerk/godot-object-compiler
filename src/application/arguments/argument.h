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
#include "library/core/string_writer.h"
#include "library/tree/syntax/function.h"

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

        virtual String value_to_string(const T& p_value) = 0;

        virtual String get_info_string();
    };

    class CommandLineArgumentParseResult
    {
      public:
        bool succeeded() const;

        CommandLineArgumentParseResult operator+(const CommandLineArgumentParseResult& other) const;

        Vector<Ref<CommandLineArgument>> get_missing_arguments() const;

        [[nodiscard]]
        String get_error_message() const;

      private:
        CommandLineArgumentParseResult() = default;

        Vector<Ref<CommandLineArgument>> missing_required;

        friend class CommandLineArgument;
        friend class ApplicationContext;
    };

    class CommandLineArgumentImpl
    {
      public:
        CommandLineArgumentImpl() = default;

        CommandLineArgumentImpl(const CommandLineArgumentImpl& other) = default;

        CommandLineArgumentImpl(CommandLineArgumentImpl&& other) noexcept = default;

        [[nodiscard]] bool is_required() const;

        [[nodiscard]] bool is_unnamed() const;

        [[nodiscard]] bool has_value() const;

        [[nodiscard]] Size size() const;

        [[nodiscard]] String get_name() const;

        [[nodiscard]] String get_short_name() const;

        [[nodiscard]] String get_argument_type() const;

        [[nodiscard]] String get_description() const;

        [[nodiscard]] String get_as_string() const;

        [[nodiscard]] String get_info_string() const;

        [[nodiscard]] bool has_correct_name(const String& p_argument) const;

        [[nodiscard]] String get_argument_part(const String& p_argument) const;

        void parse_arguments(const Vector<String>& p_arguments);

      protected:
        std::function<bool(const CommandLineArgumentImpl*)> has_value_function;
        std::function<Size(const CommandLineArgumentImpl*)> get_size_function;
        std::function<void(CommandLineArgumentImpl*, const Vector<String>&)>
            parse_arguments_function;
        std::function<String(const CommandLineArgumentImpl*)> get_argument_type_function;
        std::function<String(const CommandLineArgumentImpl*)> get_as_string_function;
        std::function<String(const CommandLineArgumentImpl*)> get_info_string_function;

        String name;
        String short_name;
        String name_prefix;
        String short_name_prefix;
        String description;
        bool value_available = false;
        bool required_arg = false;
        bool unnamed_arg = false;

        friend CommandLineArgument;
    };

    template <class T> class CommandLineArgumentImplT : public CommandLineArgumentImpl
    {
      public:
        CommandLineArgumentImplT();

        CommandLineArgumentImplT(const CommandLineArgumentImplT& other) = default;

        CommandLineArgumentImplT(CommandLineArgumentImplT&& other) noexcept = default;

        [[nodiscard]] bool value_equals(const T& p_value) const;

        [[nodiscard]] T get(Index p_index = 0) const;

        [[nodiscard]] Vector<T> get_vector() const;

      private:
        bool has_value_impl() const;

        Size get_size_impl() const;

        void parse_arguments_impl(const Vector<String>& p_arguments);

        String get_argument_type_impl() const;

        String get_as_string_impl() const;

        String get_info_string_impl() const;

        Vector<T> values;
        Ref<ICommandLineArgumentParser<T>> parser;

        friend CommandLineArgument;
    };

    class CommandLineArgument
    {
      public:
        CommandLineArgument() = default;

        CommandLineArgument(const CommandLineArgument& other) = default;

        CommandLineArgument(CommandLineArgument&& other) noexcept = default;

        template <typename P>
        static Ref<CommandLineArgument>
        unnamed(const Ref<P>& p_parser, const String& p_description);

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

        void parse_arguments(const Vector<String>& p_arguments) const;

        [[nodiscard]]
        static CommandLineArgumentParseResult parse(
            const Vector<Ref<CommandLineArgument>>& p_arguments,
            const Vector<String>& p_string_arguments);

        [[nodiscard]] bool is_required() const;

        [[nodiscard]] bool is_unnamed() const;

        [[nodiscard]] bool has_value() const;

        template <typename T> [[nodiscard]] bool value_equals(const T& p_value) const;

        template <typename T> [[nodiscard]] T get(Index p_index = 0) const;

        template <typename T> [[nodiscard]] Vector<T> get_vector() const;

        String get_as_string() const;

        [[nodiscard]] Size size() const;

        [[nodiscard]] String get_name() const;

        [[nodiscard]] String get_short_name() const;

        [[nodiscard]] String get_argument_type() const;

        [[nodiscard]] String get_description() const;

        [[nodiscard]] String get_info_string() const;

      private:
        TypeIndex type = typeid(void);
        Ref<CommandLineArgumentImpl> impl;
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

    template <typename T> String ICommandLineArgumentParser<T>::get_info_string()
    {
        return "";
    }

    template <class T> CommandLineArgumentImplT<T>::CommandLineArgumentImplT()
    {
        has_value_function = [](const CommandLineArgumentImpl* ptr) {
            return static_cast<const CommandLineArgumentImplT*>(ptr)->has_value_impl();
        };

        get_size_function = [](const CommandLineArgumentImpl* ptr) {
            return static_cast<const CommandLineArgumentImplT*>(ptr)->get_size_impl();
        };

        get_argument_type_function = [](const CommandLineArgumentImpl* ptr) {
            return static_cast<const CommandLineArgumentImplT*>(ptr)->get_argument_type_impl();
        };

        get_as_string_function = [](const CommandLineArgumentImpl* ptr) {
            return static_cast<const CommandLineArgumentImplT*>(ptr)->get_as_string_impl();
        };

        get_info_string_function = [](const CommandLineArgumentImpl* ptr) {
            return static_cast<const CommandLineArgumentImplT*>(ptr)->get_info_string_impl();
        };

        parse_arguments_function = [](CommandLineArgumentImpl* ptr,
                                      const Vector<String>& p_arguments) {
            return static_cast<CommandLineArgumentImplT*>(ptr)->parse_arguments_impl(p_arguments);
        };
    }

    template <class T> bool CommandLineArgumentImplT<T>::value_equals(const T& p_value) const
    {
        if (!has_value()) {
            return false;
        }
        return values[0] == p_value;
    }

    template <class T> T CommandLineArgumentImplT<T>::get(Index p_index) const
    {
        PANIC_COND(p_index >= values.size(), "Invalid out of bounds access");
        PANIC_COND(!unnamed_arg && p_index != 0, "Invalid unnamed access on no unnamed argument");
        return values[p_index];
    }

    template <class T> Vector<T> CommandLineArgumentImplT<T>::get_vector() const
    {
        return values;
    }

    template <class T> bool CommandLineArgumentImplT<T>::has_value_impl() const
    {
        return !values.empty();
    }

    template <class T> Size CommandLineArgumentImplT<T>::get_size_impl() const
    {
        return values.size();
    }

    template <class T>
    void CommandLineArgumentImplT<T>::parse_arguments_impl(const Vector<String>& p_arguments)
    {
        auto is_named_arg = [](const String& p_argument) { return string_prefix(p_argument, "-"); };

        Index unnamed_i = 0;

        for (const auto& argument : p_arguments) {
            if (has_correct_name(argument)) {
                Opt<T> opt_value = parser->parse_argument(get_argument_part(argument));
                if (opt_value.has_value()) {
                    value_available = true;
                    values.clear();
                    values.push_back(*opt_value);
                } else {
                    value_available = false;
                    values.clear();
                }
            } else if (unnamed_arg && !is_named_arg(argument)) {
                Opt<T> opt_value = parser->parse_argument(argument);
                if (opt_value.has_value()) {
                    value_available = true;
                    if (values.size() >= unnamed_i) {
                        values.resize(unnamed_i + 1);
                    }

                    values[unnamed_i++] = *opt_value;
                } else {
                    value_available = false;
                    values.clear();
                }
            }
        }
    }

    template <class T> String CommandLineArgumentImplT<T>::get_argument_type_impl() const
    {
        return parser->get_argument_type_string();
    }

    template <class T> String CommandLineArgumentImplT<T>::get_as_string_impl() const
    {
        PANIC_COND(values.size() == 0, "No value available");
        return parser->value_to_string(get(0));
    }

    template <class T> String CommandLineArgumentImplT<T>::get_info_string_impl() const
    {
        return parser->get_info_string();
    }

    template <typename P>
    Ref<CommandLineArgument>
    CommandLineArgument::unnamed(const Ref<P>& p_parser, const String& p_description)
    {
        auto result = make_ref<CommandLineArgument>();
        auto impl = make_ref<CommandLineArgumentImplT<typename P::ValueType>>();
        result->impl = std::dynamic_pointer_cast<CommandLineArgumentImpl>(impl);
        result->type = typeid(typename P::ValueType);
        impl->unnamed_arg = true;
        impl->parser = p_parser;
        impl->description = p_description;
        return result;
    }

    template <typename P>
    Ref<CommandLineArgument> CommandLineArgument::optional(
        const Ref<P>& p_parser, const String& p_name, const String& p_short_name,
        const String& p_description)
    {
        auto result = make_ref<CommandLineArgument>();
        auto impl = make_ref<CommandLineArgumentImplT<typename P::ValueType>>();
        result->impl = std::dynamic_pointer_cast<CommandLineArgumentImpl>(impl);
        result->type = typeid(typename P::ValueType);
        impl->parser = p_parser;
        impl->name = p_name;
        impl->short_name = p_short_name;
        impl->name_prefix = format("--%s=", p_name.c_str());
        impl->short_name_prefix = format("-%s=", p_short_name.c_str());
        impl->description = p_description;
        return result;
    }

    template <typename P>
    Ref<CommandLineArgument> CommandLineArgument::defaulted(
        const Ref<P>& p_parser, const String& p_name, const String& p_short_name,
        const String& p_description, const typename P::ValueType& p_default)
    {
        auto result = make_ref<CommandLineArgument>();
        auto impl = make_ref<CommandLineArgumentImplT<typename P::ValueType>>();
        result->impl = std::dynamic_pointer_cast<CommandLineArgumentImpl>(impl);
        result->type = typeid(typename P::ValueType);
        impl->parser = p_parser;
        impl->name = p_name;
        impl->short_name = p_short_name;
        impl->name_prefix = format("--%s=", p_name.c_str());
        impl->short_name_prefix = format("-%s=", p_short_name.c_str());
        impl->description = p_description;
        impl->value_available = true;
        impl->values.push_back(p_default);
        return result;
    }

    template <typename P>
    Ref<CommandLineArgument> CommandLineArgument::required(
        const Ref<P>& p_parser, const String& p_name, const String& p_short_name,
        const String& p_description)
    {
        auto result = make_ref<CommandLineArgument>();
        auto impl = make_ref<CommandLineArgumentImplT<typename P::ValueType>>();
        result->impl = std::dynamic_pointer_cast<CommandLineArgumentImpl>(impl);
        result->type = typeid(typename P::ValueType);
        impl->parser = p_parser;
        impl->name = p_name;
        impl->short_name = p_short_name;
        impl->name_prefix = format("--%s=", p_name.c_str());
        impl->short_name_prefix = format("-%s=", p_short_name.c_str());
        impl->description = p_description;
        impl->required_arg = true;
        return result;
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
        auto impl_t = static_cast<CommandLineArgumentImplT<T>*>(impl.get());
        return impl_t->value_equals(p_value);
    }

    template <typename T> T CommandLineArgument::get(Index p_index) const
    {
        PANIC_COND(type != typeid(T), "Invalid type access.");
        auto impl_t = static_cast<CommandLineArgumentImplT<T>*>(impl.get());
        return impl_t->get(p_index);
    }

    template <typename T> Vector<T> CommandLineArgument::get_vector() const
    {
        PANIC_COND(type != typeid(T), "Invalid type access.");
        auto impl_t = static_cast<CommandLineArgumentImplT<T>*>(impl.get());
        return impl_t->get_vector();
    }
} // namespace GodotObjectCompiler

#define CLI_PARS_ERR_V(result, value)                                                              \
    do {                                                                                           \
        if (!result.succeeded()) {                                                                 \
            return value;                                                                          \
        }                                                                                          \
    } while (false)
