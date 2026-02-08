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

#include "print_parsed.h"

#include "library/core/file_system_utilities.h"

#include "library/core/string_utilities.h"
#include "library/parser/parser.h"
#include "library/parser/tree_sitter_node.h"
#include "library/tree/syntax/namespace.h"

namespace GodotObjectCompiler {

  Ref<ProgramError> PrintParsed::run(ApplicationContext& p_context) {
    if (p_context.program_arguments.size() != 1) {
      return node_new<ProgramError>(
          ERROR, format("Invalid argument count for program %s. Expected 1 path argument.", get_type_static().c_str()));
    }

    auto path = path_absolute(p_context.program_arguments[0]);

    if (!file_exists(path)) {
      return node_new<ProgramError>(
          ERROR, format("Invalid path argument for program %s. File does not exist.", get_type_static().c_str()));
    }

    ParserContext parser_context(read_file(path));
    if (!parser_context.is_valid()) {
      return node_new<ProgramError>(ERROR, format("Failed to parse file %s.", path.c_str()));
    }

    print_ln(parser_context.current_src->pretty_print());
    return ProgramError::OK;
  }

}