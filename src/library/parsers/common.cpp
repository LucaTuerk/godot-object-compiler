/**************************************************************************/
/* common.cpp                                                             */
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

#include "common.h"

#include "library/attribute_db.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"

namespace GodotObjectCompiler::ParserUtilities
{
    Result<Attribute> parse_attribute(
        const String& p_macro, const String& p_arguments, const SourceLocation& p_sourceLocation,
        bool p_parse_attributes)
    {
        ERROR_COND(
            !LibraryContext::instance()->get_attribute_db()->is_known_macro(p_macro),
            "Unknown attribute \"%s\"", p_macro.c_str());

        const Result<Attribute> attribute_result =
            LibraryContext::instance()->get_attribute_db()->create_for_macro(p_macro);
        RESULT_ERROR_PASS_ON(Error, attribute_result, attribute);

        attribute->start = p_sourceLocation.start;
        attribute->end = p_sourceLocation.end;
        attribute->line = p_sourceLocation.line;

        if (p_parse_attributes) {
            const Ref<Arguments> arguments = attribute->B<Arguments>();

            if (const Ref<IAttributeArgumentParser> argument_parser =
                    attribute->get_argument_parser();
                argument_parser != nullptr) {
                const Ref<ParserError> error =
                    argument_parser->parse_attribute_arguments(p_arguments, arguments);
                if (error != ParserError::OK) {
                    return ERROR_CAST(Error, error);
                }
            }
        } else {
            attribute->B<UnparsedAttributeArguments>(p_arguments);
        }
        return attribute;
    }

    String remove_macros(const String& p_input)
    {
        StreamWriter writer;
        for (String line : string_split(p_input, "\n")) {
            if (!string_prefix(string_trim(line), "#")) {
                for (const String& macro : LibraryContext::instance()->get_remove_macros()) {
                    line = string_replace(line, macro, "");
                }
            }
            writer.write(line);
            writer.write("\n");
        }

        return writer.get_string();
    }

    String strip_excluded_sections(const String& p_source)
    {
        const String off_marker = "// goc off";
        const String on_marker = "// goc on";
        Vector<std::pair<Size, Size>> exclusions;
        Size pos = 0;

        while (pos < p_source.size()) {
            Size off_pos = p_source.find(off_marker, pos);
            Size on_pos = p_source.find(on_marker, pos);

            if (off_pos == String::npos)
                break;

            Size start = p_source.rfind('\n', off_pos);
            if (start == String::npos)
                start = 0;
            else
                start++;

            if (on_pos == String::npos || on_pos < off_pos) {
                exclusions.emplace_back(start, p_source.size());
                break;
            }

            Size end = p_source.find('\n', on_pos);
            if (end == String::npos)
                end = p_source.size();
            else
                end++;

            exclusions.emplace_back(start, end);
            pos = end;
        }

        StreamWriter result;
        Size last = 0;
        for (const auto& [start, end] : exclusions) {
            if (start > last)
                result.write(p_source.substr(last, start - last));
            last = end;
        }
        if (last < p_source.size())
            result.write(p_source.substr(last));

        return result.get_string();
    }

    String strip_known_macro_contents(const String& p_input, Dictionary<Size, String>& r_parameters)
    {
        String local_input = p_input;
        Vector<String> macros = LibraryContext::instance()->get_attribute_db()->get_all_macros();

        for (const String& macro : macros) {
            Size index = 0;
            Vector<Size> positions;
            {
                Size position = local_input.find(macro);
                while (position != String::npos) {
                    positions.push_back(position);
                    position = local_input.find(macro, position + 1);
                }
            }

            std::sort(positions.begin(), positions.end());

            StreamWriter writer;
            for (Size position : positions) {
                Size open_index = position;
                bool found_whitespace = false;
                bool no_args = false;

                auto itr = std::next(local_input.begin(), position);
                while (itr != local_input.end()) {
                    if (*itr == '(') {
                        break;
                    }

                    bool whitespace = is_whitespace(*itr);
                    if (whitespace && !found_whitespace) {
                        found_whitespace = true;
                    } else if (!whitespace && found_whitespace) {
                        no_args = true;
                        break;
                    }

                    ++itr;
                    ++open_index;
                }

                if (no_args) {
                    continue;
                }

                StreamWriter content;
                Size opened = 1;
                Size closed_index = open_index + 1;

                if (itr == local_input.end()) {
                    continue;
                }

                ++itr;
                while (itr != local_input.end()) {
                    if (*itr == '(') {
                        opened++;
                    }
                    if (*itr == ')') {
                        opened--;
                    }

                    if (opened == 0) {
                        break;
                    }
                    content.write_generic(*itr);
                    ++itr;
                    ++closed_index;
                }

                r_parameters.insert({position, content.get_string()});

                writer.write(local_input.substr(index, open_index - index + 1));
                for (char c : content.get_string()) {
                    if (is_whitespace(c)) {
                        writer.write_generic(c);
                    } else {
                        writer.write(" ");
                    }
                }
                index = closed_index;
            }
            writer.write(local_input.substr(index));
            local_input = writer.get_string();
        }
        return local_input;
    }
} // namespace GodotObjectCompiler::ParserUtilities