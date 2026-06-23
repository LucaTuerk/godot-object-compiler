/**************************************************************************/
/* main.cpp                                                               */
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

#include "main.h"

#include "application/application.h"
#include "application/programs/all.h"
#include "library/core/config.h"
#include "library/core/core.h"
#include "library/library_context.h"
#include "library_godot/parsers/extension_api_parser.h"
#if DEV_BUILD
#include "application/programs_dev/all.h"
#endif

#if GOC_LIBCLANG_PARSER_ENABLED
#include "library/parsers/libclang/parser.h"
#endif
#if GOC_TREE_SITTER_PARSER_ENABLED
#include "library/parsers/tree-sitter/parser.h"
#endif

using namespace GodotObjectCompiler;

int main(int argc, char* argv[])
{
    Vector<String> args;
    for (int i = 1; i < argc; i++) {
        args.emplace_back(argv[i]);
    }

    Application application;
    LibraryContext::instance()->set_error_level(INFO, FULL);

#ifdef GOC_LIBCLANG_PARSER_ENABLED
#ifndef GOC_TREE_SITTER_PARSER_ENABLED
    LibraryContext::instance()->set_default_parser<ClangParser>(IParser::SOURCE_PARSER);
#endif
#endif
#if GOC_TREE_SITTER_PARSER_ENABLED
    LibraryContext::instance()->set_default_parser<TreeSitterParser>(IParser::SOURCE_PARSER);
#endif

    LibraryContext::instance()->set_default_parser<ExtensionAPIParser>(IParser::JSON_CONFIG_PARSER);
    return application.run(args);
}
