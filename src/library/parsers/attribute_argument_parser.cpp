/**************************************************************************/
/* attribute_argument_parser.cpp                                          */
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

#include "attribute_argument_parser.h"

#include "library/core/string_utilities.h"

namespace GodotObjectCompiler
{

    Vector<String> IAttributeArgumentParser::split_arguments(const String& p_content)
    {
        std::stringstream strstr;
        Vector<String> result;

        Size brackets_open = 0;
        Size quotes_open = 0;
        bool escaped = false;

        if (string_trim(p_content).empty()) {
            return {};
        }

        for (char c : p_content) {
            if (!escaped) {
                if (c == '(') {
                    brackets_open++;
                } else if (c == ')') {
                    if (brackets_open == 0) {
                        return {};
                    }
                    brackets_open--;
                } else if (c == '"') {
                    if (quotes_open) {
                        quotes_open--;
                    } else {
                        quotes_open++;
                    }
                } else if (c == '\\') {
                    escaped = true;
                    strstr << c;
                    continue;
                } else if (c == ',') {
                    if (!quotes_open && !brackets_open) {
                        result.push_back(strstr.str());
                        strstr = {};
                        continue;
                    }
                }
            }

            strstr << c;
            escaped = false;
        }

        result.push_back(string_trim(strstr.str()));
        return result;
    }

    void IAttributeArgumentParser::split_outer_inner(
        const String& p_content, String& r_outer, String& r_inner)
    {
        std::stringstream inner_strm;
        std::stringstream outer_strm;
        r_outer = "";
        r_inner = "";

        bool open = false;
        bool escaped = false;
        for (char c : p_content) {
            if (!escaped) {
                if (c == '(') {
                    open = true;
                    continue;
                } else if (c == ')') {
                    break;
                } else if (c == '\\') {
                    escaped = true;
                    inner_strm << c;
                    continue;
                }
            }

            if (open) {
                inner_strm << c;
                escaped = false;
            } else {
                outer_strm << c;
            }
        }

        r_outer = string_trim(outer_strm.str());
        r_inner = string_trim(inner_strm.str());
    }

    Vector<String> IAttributeArgumentParser::split_flags(const String& p_content)
    {
        Vector<String> split = string_split(p_content, "|");
        std::transform(split.cbegin(), split.cend(), split.begin(), &string_trim);
        return split;
    }

} // namespace GodotObjectCompiler
