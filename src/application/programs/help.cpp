/**************************************************************************/
/* help.cpp                                                               */
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

#include "help.h"

#include "application/arguments/argument_lists.h"
#include "application/build_info.h"
#include "application/programs/program.h"
#include "library/core/core.h"
#include "library/core/file_system_utilities.h"
#include "library/core/resources.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"
#include "library/parser.h"

namespace GodotObjectCompiler
{
    void
    write_named_arguments(IStringWriter* p_writer, Vector<Ref<CommandLineArgument>>& p_arguments)
    {
        std::sort(
            p_arguments.begin(), p_arguments.end(),
            [](const Ref<CommandLineArgument>& a, const Ref<CommandLineArgument>& b) {
                return a->get_name() < b->get_name();
            });

        for (const auto& argument : p_arguments) {
            if (!argument->is_unnamed()) {
                StreamWriter desc_writer;
                if (argument->has_value()) {
                    desc_writer.write(format("Default: %s", argument->get_as_string().c_str()));
                }

                if (String info = argument->get_info_string(); !info.empty()) {
                    desc_writer.write(
                        format(argument->has_value() ? ", %s\n" : "%s\n", info.c_str()));
                } else {
                    desc_writer.write("\n");
                }

                desc_writer.write(argument->get_description());

                write_columns(
                    p_writer,
                    {30,
                     format(
                         "--%s=<%s>\n -%s=<%s>\n", argument->get_name().c_str(),
                         argument->get_argument_type().c_str(), argument->get_short_name().c_str(),
                         argument->get_argument_type().c_str())},
                    {70, desc_writer.get_string()});
                p_writer->write("\n");
            }
        }
    }

    void Help::write_header(IStringWriter* p_writer)
    {
        write_title(
            p_writer,
            format(
                "Godot Object Compiler v%d.%d.%d \"%s\"", BuildInfo::major_version,
                BuildInfo::minor_version, BuildInfo::patch_version, BuildInfo::version_name),
            100);
        p_writer->write("\n");

        if (Resources::instance()->has_resource("res:/help/header_content.txt")) {
            p_writer->write(
                Resources::instance()->load_text_resource("res:/help/header_content.txt"));
            p_writer->write("\n");
        }

        ApplicationContext argument_context;
        argument_context.register_argument_lists<ApplicationArguments>();
        argument_context.register_argument_lists<GeneratorArguments>();
        argument_context.register_argument_lists<GDExtensionProjectArguments>();

        auto arguments = argument_context.get_command_line_arguments();

        write_title(p_writer, "Arguments", 100);
        p_writer->write("\n");
        write_named_arguments(p_writer, arguments);
    }

    void Help::write_parser_info(IStringWriter* p_writer)
    {
        write_title(p_writer, "SOURCE PARSERS", 100);
        for (const auto& parser : LibraryContext::instance()->get_parsers()) {
            if ((parser->get_capabilities() & IParser::SOURCE_PARSER) == 0) {
                continue;
            }

            bool is_default =
                LibraryContext::instance()->get_default_parser(IParser::SOURCE_PARSER) == parser;

            Path help_path = Path("res:/") / "help" / format("%s.txt", parser->get_type().c_str());

            String help_text = "No description available";
            if (Resources::instance()->has_resource(help_path)) {
                String content = Resources::instance()->load_text_resource(help_path);
                if (!content.empty()) {
                    help_text = content;
                }
            }

            p_writer->write("\n");
            write_columns(
                p_writer,
                {30, is_default ? format("%s (Default)", parser->get_type().c_str())
                                : parser->get_type()},
                {70, help_text});
        }
    }

    void Help::write_program_info(
        IStringWriter* p_writer, const ProgramPath& p_path, const Ref<IProgram>& p_program,
        bool p_detailed)
    {
        StreamWriter identifier_writer;

        if (p_detailed) {
            identifier_writer.write(string_vector_combine(p_path, " "));
        } else {
            Size i = 0;
            for (auto itr = p_path.begin(); itr != p_path.end() - 1; ++itr, ++i) {
                Vector<String> sub{p_path.begin(), itr + 1};
                if (written.find(sub) != written.end()) {
                    continue;
                }

                StreamWriter sub_writer;
                sub_writer.write(string_indent_lines(sub.back(), ' ', 2 * i));
                written.insert(sub);

                p_writer->write("\n");
                write_columns(p_writer, {30, sub_writer.get_string()}, {70, ""});
            }

            identifier_writer.write(p_path.back());
        }

        p_writer->write("\n");
        write_columns(
            p_writer,
            {30, string_indent_lines(identifier_writer.get_string(), ' ', 2 * p_path.size())},
            {70, get_help_text(p_path)});

        if (p_detailed) {
            p_writer->write("\n");

            ApplicationContext program_context;
            if (!p_program->is_readonly()) {
                program_context.register_argument_lists<ApplicationArguments>();
                program_context.register_argument_lists<GeneratorArguments>();
                program_context.register_argument_lists<GDExtensionProjectArguments>();
            }

            std::ignore = p_program->register_required_arguments(program_context);
            auto arguments = program_context.get_command_line_arguments();

            Vector<Ref<CommandLineArgument>> required_arguments;
            Vector<Ref<CommandLineArgument>> optional_arguments;
            Vector<Ref<CommandLineArgument>> unnamed_arguments;

            std::copy_if(
                arguments.begin(), arguments.end(), std::back_inserter(required_arguments),
                [](const Ref<CommandLineArgument>& argument) { return argument->is_required(); });
            std::copy_if(
                arguments.begin(), arguments.end(), std::back_inserter(optional_arguments),
                [](const Ref<CommandLineArgument>& argument) {
                    return !argument->is_required() && !argument->is_unnamed();
                });
            std::copy_if(
                arguments.begin(), arguments.end(), std::back_inserter(unnamed_arguments),
                [](const Ref<CommandLineArgument>& argument) { return argument->is_unnamed(); });

            if (!required_arguments.empty()) {
                write_title(p_writer, "Required arguments", 100);
                p_writer->write("\n");
                write_named_arguments(p_writer, required_arguments);
            }

            if (!optional_arguments.empty()) {
                write_title(p_writer, "Optional arguments", 100);
                p_writer->write("\n");
                write_named_arguments(p_writer, optional_arguments);
            }

            if (unnamed_arguments.size() == 1) {
                write_title(p_writer, "Unnamed Arguments", 100);
                p_writer->write("\n");
                const auto& argument = unnamed_arguments[0];
                write_columns(
                    p_writer,
                    {30, format("<%s...> (unnamed)", argument->get_argument_type().c_str())},
                    {70, argument->get_description()});
            }
        }

        written.insert(p_path);
    }

    Ref<ProgramError> Help::execute(ApplicationContext& p_context)
    {
        const auto arguments = p_context.get_argument_list<HelpArguments>();

        StreamWriter writer;
        PROG_ERR_COND(
            !get_help(&writer, arguments->program_path->get_vector<String>()),
            "Failed to get help content.");
        print(writer.get_string());
        return ProgramError::OK;
    }

    CommandLineArgumentParseResult
    Help::register_required_arguments(ApplicationContext& p_context) const
    {
        return p_context.register_argument_lists<HelpArguments>();
    }

    bool Help::get_help(IStringWriter* p_writer, const Vector<String>& p_args)
    {
        written = {};

        Dictionary<ProgramPath, Ref<IProgram>> programs = Programs::instance()->get_programs();

        auto cmp = [](const Pair<ProgramPath, Ref<IProgram>>& a,
                      const Pair<ProgramPath, Ref<IProgram>>& b) {
            return string_vector_combine(a.first, "") < string_vector_combine(b.first, "");
        };

        Vector<Pair<ProgramPath, Ref<IProgram>>> programs_sorted;
        std::copy(programs.begin(), programs.end(), std::back_inserter(programs_sorted));
        std::sort(programs_sorted.begin(), programs_sorted.end(), cmp);

        if (p_args.empty()) {
            write_header(p_writer);
            p_writer->write("\n");
            write_title(p_writer, "PROGRAMS", 100);
        }

        if (p_args.empty()) {
            for (const auto& [path, program] : programs_sorted) {
                StreamWriter writer;
                write_program_info(p_writer, path, program, false);
            }
            p_writer->write("\n");
            write_parser_info(p_writer);
        } else {
            auto itr = std::find_if(
                programs_sorted.begin(), programs_sorted.end(), [&p_args](const auto& pair) {
                    const auto& [path, _] = pair;
                    return path.size() == p_args.size() &&
                           std::equal(path.begin(), path.end(), p_args.begin());
                });

            if (itr != programs_sorted.end()) {
                const auto& [path, program] = *itr;
                StreamWriter writer;
                write_program_info(&writer, path, program, true);
                p_writer->write(writer.get_string());
            }
        }

        return true;
    }

    void Help::write_title(IStringWriter* p_writer, const String& p_title, Size width)
    {
        if (p_title.size() + 4 >= width) {
            p_writer->write(p_title);
            p_writer->write("\n");
        }

        Size available_width = width - 4 - p_title.size();
        StreamWriter writer;

        for (Size i = 0; i < available_width / 2; i++) {
            p_writer->write("=");
        }

        p_writer->write("[ ");
        p_writer->write(p_title);
        p_writer->write(" ]");

        for (Size i = 0; i < available_width / 2 + available_width % 2; i++) {
            p_writer->write("=");
        }
        p_writer->write("\n");
    }

    String Help::get_help_text(const ProgramPath& p_path)
    {
        if (p_path.empty()) {
            return "";
        }
        String file_stem = string_vector_combine(p_path, "_");
        Path res_path = Path("res:/") / "help" / format("%s.txt", file_stem.c_str());
        if (!Resources::instance()->has_resource(res_path)) {
            return "-";
        }

        return Resources::instance()->load_text_resource(res_path);
    }

    Size Help::ProgramPathHash::operator()(const ProgramPath& path) const
    {
        return std::hash<String>()(string_vector_combine(path, ""));
    }

} // namespace GodotObjectCompiler
