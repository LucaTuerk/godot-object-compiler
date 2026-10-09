/**************************************************************************/
/* path_argument.cpp                                                      */
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

#include "path_argument.h"
#include "library/core/file_system_utilities.h"
#include "library/core/string_utilities.h"

namespace GodotObjectCompiler
{
    struct CheckAliasResult {
        bool has_alias = false;
        Opt<Path> alias;
        String rest;

        static CheckAliasResult no_alias()
        {
            return {false, std::nullopt, ""};
        }

        static CheckAliasResult aliased(Opt<Path> p_resolved, String rest)
        {
            return {true, std::move(p_resolved), std::move(rest)};
        }
    };

    CheckAliasResult check_alias(const String& p_path)
    {
        if (p_path.find("{alias:") != 0) {
            return CheckAliasResult::no_alias();
        }

        const size_t close = p_path.find('}');

        if (close == String::npos) {
            return CheckAliasResult::no_alias();
        }

        StreamWriter alias_writer;
        bool started = false;
        for (auto c : p_path.substr(7, close - 7)) {
            const bool whitespace = is_whitespace(c);
            if (started && whitespace) {
                break;
            }

            if (!started && !whitespace) {
                started = true;
            }

            if (started) {
                alias_writer.write_generic(c);
            }
        }

        const String rest = p_path.substr(close + 2);
        const Opt<Path> resolved =
            LibraryContext::instance()->get_path_alias(alias_writer.get_string());

        return CheckAliasResult::aliased(resolved, rest);
    }

    Opt<Path> PathCommandLineArgumentParser::parse_argument(const String& p_argument)
    {
        const auto [has_alias, alias, rest] = check_alias(p_argument);
        if (has_alias && !alias.has_value()) {
            return std::nullopt;
        }

        Path argument = has_alias ? alias.value() / Path(rest) : path_absolute(Path(p_argument));

        if (!could_be_path(argument)) {
            return std::nullopt;
        }

        return argument;
    }

    String PathCommandLineArgumentParser::value_to_string(const Path& p_value)
    {
        return p_value.string();
    }

    Opt<Vector<Path>> PathListCommandLineArgumentParser::parse_argument(const String& p_argument)
    {
        const String& argument = p_argument;
        const Vector<String> paths = string_split(argument, ",");
        Vector<Path> results;

        for (const String& path : paths) {
            const auto [has_alias, alias, rest] = check_alias(path);
            if (has_alias && !alias.has_value()) {
                return std::nullopt;
            }

            Path result_path = has_alias ? alias.value() / Path(rest) : path_absolute(Path(path));

            if (!could_be_path(result_path)) {
                return std::nullopt;
            }

            results.push_back(path_absolute(result_path));
        }

        return results;
    }

    String PathListCommandLineArgumentParser::value_to_string(const Vector<Path>& p_value)
    {
        StreamWriter writer;
        writer.write("[");

        Vector<String> paths;
        for (const Path& path : p_value) {
            paths.push_back(path_absolute(path).string());
        }

        writer.write(string_vector_combine(paths, ", "));
        writer.write("]");
        return writer.get_string();
    }
} // namespace GodotObjectCompiler
