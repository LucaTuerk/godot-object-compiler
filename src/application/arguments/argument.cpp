/**************************************************************************/
/* argument.cpp                                                           */
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

#include "argument.h"

#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"

namespace GodotObjectCompiler
{
    bool CommandLineArgumentParseResult::succeeded() const
    {
        return missing_required.empty();
    }

    CommandLineArgumentParseResult
    CommandLineArgumentParseResult::operator+(const CommandLineArgumentParseResult& other) const
    {
        CommandLineArgumentParseResult result;
        result.missing_required.insert(
            result.missing_required.end(), missing_required.begin(), missing_required.end());
        result.missing_required.insert(
            result.missing_required.end(), other.missing_required.begin(),
            other.missing_required.end());
        return result;
    }

    Vector<Ref<CommandLineArgument>> CommandLineArgumentParseResult::get_missing_arguments() const
    {
        return missing_required;
    }

    String CommandLineArgumentParseResult::get_error_message() const
    {
        StreamWriter writer;

        if (missing_required.size() == 1) {
            writer.write("Required argument missing:\n");
        } else if (missing_required.size() > 1) {
            writer.write("Required arguments missing:\n");
        }

        for (const auto& argument : missing_required) {
            write_columns(
                &writer,
                {30, format(
                         "--%s=<%s>", argument->get_name().c_str(),
                         argument->get_argument_type().c_str())},
                {70, argument->get_description()});
        }
        return writer.get_string();
    }

    void CommandLineArgument::parse_arguments(const Vector<String>& p_arguments) const
    {
        impl->parse_arguments(p_arguments);
    }

    CommandLineArgumentParseResult CommandLineArgument::parse(
        const Vector<Ref<CommandLineArgument>>& p_arguments,
        const Vector<String>& p_string_arguments)
    {
        CommandLineArgumentParseResult result;

        for (auto& argument : p_arguments) {
            PANIC_COND(argument == nullptr, "Invalid argument.");
            argument->parse_arguments(p_string_arguments);

            if (argument->is_required() && !argument->has_value()) {
                result.missing_required.push_back(argument);
            }
        }

        return result;
    }

    bool CommandLineArgument::is_required() const
    {
        return impl->is_required();
    }

    bool CommandLineArgument::is_positional() const
    {
        return impl->is_positional();
    }

    bool CommandLineArgument::has_value() const
    {
        return impl->value_available;
    }

    Size CommandLineArgument::size() const
    {
        return impl->size();
    }

    String CommandLineArgument::get_name() const
    {
        return impl->get_name();
    }

    String CommandLineArgument::get_short_name() const
    {
        return impl->get_short_name();
    }

    String CommandLineArgument::get_argument_type() const
    {
        return impl->get_argument_type();
    }

    String CommandLineArgument::get_description() const
    {
        return impl->get_description();
    }

    bool CommandLineArgumentImpl::is_required() const
    {
        return required_arg;
    }

    bool CommandLineArgumentImpl::is_positional() const
    {
        return positional_arg;
    }

    bool CommandLineArgumentImpl::has_value() const
    {
        return has_value_function(this);
    }

    Size CommandLineArgumentImpl::size() const
    {
        return get_size_function(this);
    }

    String CommandLineArgumentImpl::get_name() const
    {
        return name;
    }

    String CommandLineArgumentImpl::get_short_name() const
    {
        return short_name;
    }

    String CommandLineArgumentImpl::get_argument_type() const
    {
        return get_argument_type_function(this);
    }

    String CommandLineArgumentImpl::get_description() const
    {
        return description;
    }

    bool CommandLineArgumentImpl::has_correct_name(const String& p_argument) const
    {
        return !positional_arg && (string_prefix(p_argument, name_prefix) ||
                                   string_prefix(p_argument, short_name_prefix));
    }

    String CommandLineArgumentImpl::get_argument_part(const String& p_argument) const
    {
        if (positional_arg) {
            return p_argument;
        }

        if (string_prefix(p_argument, name_prefix)) {
            return p_argument.substr(name_prefix.length());
        }

        if (string_prefix(p_argument, short_name_prefix)) {
            return p_argument.substr(short_name_prefix.length());
        }

        PANIC("Invalid access to argument part. No argument name or short name match.");
    }

    void CommandLineArgumentImpl::parse_arguments(const Vector<String>& p_arguments)
    {
        return parse_arguments_function(this, p_arguments);
    }

} // namespace GodotObjectCompiler
