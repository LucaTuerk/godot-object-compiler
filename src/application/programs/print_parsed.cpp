/**************************************************************************/
/* print_parsed.cpp                                                       */
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

#if GOC_TREE_SITTER_PARSER_ENABLED
#include "print_parsed.h"

#include "library/core/file_system_utilities.h"
#include "library/parsers/tree-sitter/parser.h"
#include "library/parsers/tree-sitter/tree_sitter_node.h"

namespace GodotObjectCompiler
{

    Vector<Ref<CommandLineArgument>> PrintParsedArguments::get_arguments() const
    {
        return {input_files};
    }

    CommandLineArgumentParseResult
    PrintParsed::register_required_arguments(ApplicationContext& p_context) const
    {
        return p_context.register_argument_lists<PrintParsedArguments>();
    }

    Ref<ProgramError> PrintParsed::execute(ApplicationContext& p_context)
    {
        const auto arguments = p_context.get_argument_list<PrintParsedArguments>();

        for (const auto& path : arguments->input_files->get_vector<Path>()) {
            PROG_ERR_COND(
                !file_exists(path), "Invalid path argument for program %s. File does not exist.",
                get_type_static().c_str());

            ParserContext parser_context(read_file(path));

            PROG_ERR_COND(!parser_context.is_valid(), "Failed to parse file %s.", path.c_str());

            print_ln(parser_context.current_src->pretty_print());
        }
        return ProgramError::OK;
    }

} // namespace GodotObjectCompiler
#endif
