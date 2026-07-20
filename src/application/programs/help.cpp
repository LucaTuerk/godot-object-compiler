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

    void Help::write_header(IStringWriter* p_writer)
    {
        write_title(
            p_writer,
            format(
                "Godot Object Compiler v%d.%d.%d \"%s\"", BuildInfo::major_version,
                BuildInfo::minor_version, BuildInfo::patch_version, BuildInfo::version_name),
            100);
        p_writer->write("\n");

        if (Resources::instance()->has_resource("res://help/header_content.txt")) {
            p_writer->write(
                Resources::instance()->load_text_resource("res://help/header_content.txt"));
            p_writer->write("\n");
        }

        if (Resources::instance()->has_resource("res://help/args.txt")) {
            p_writer->write(Resources::instance()->load_text_resource("res://help/args.txt"));
            p_writer->write("\n");
        }
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

            String help_path = format("res://help/%s.txt", parser->get_type().c_str());

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
        IStringWriter* p_writer, const ProgramPath& p_path, const Ref<IProgram>& program)
    {
        for (auto itr = p_path.begin(); itr != p_path.end() - 1; ++itr) {
            Vector<String> sub{p_path.begin(), itr + 1};
            if (written.find(sub) != written.end()) {
                continue;
            }

            StreamWriter sub_writer;
            for (Size i = 0; i < sub.size() - 1; ++i) {
                sub_writer.write("  ");
            }
            sub_writer.write(sub.back());
            written.insert(sub);
            p_writer->write("\n");
            write_columns(p_writer, {30, sub_writer.get_string()}, {70, ""});
        }

        StreamWriter identifier_writer;
        for (Size i = 0; i < p_path.size() - 1; ++i) {
            identifier_writer.write("  ");
        }
        identifier_writer.write(p_path.back());

        if (program->is_readonly()) {
            identifier_writer.write(" [!p]");
        }

        p_writer->write("\n");
        write_columns(p_writer, {30, identifier_writer.get_string()}, {70, get_help_text(p_path)});

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
            write_parser_info(p_writer);
            p_writer->write("\n");
            write_title(p_writer, "PROGRAMS", 100);
        }

        for (const auto& [path, program] : programs_sorted) {
            if (path.empty()) {
                continue;
            }

            bool skip = false;
            for (Size i = 0; i < path.size() && i < p_args.size(); i++) {
                if (path[i] != p_args[i]) {
                    skip = true;
                }
            }

            if (skip) {
                continue;
            }

            write_program_info(p_writer, path, program);
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
        String res_path = "res://" + path_concat_ext("help", file_stem, "txt");
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
