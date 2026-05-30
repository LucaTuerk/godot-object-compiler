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
#include "application/programs/generate_resources.h"
#include "library/core/config.h"
#include "library/core/core.h"
#include "library/library_context.h"
#include "library/parsers/libclang/parser.h"
#include "library_godot/parsers/extension_api_parser.h"

using namespace GodotObjectCompiler;

int main(int argc, char* argv[])
{
    LibraryContext::instance()->set_default_parser<ClangParser>(
        IParser::CAPABILITIES_SOURCE_PARSER | IParser::CAPABILITIES_SOURCE_SUPPORT_MACRO_EXPANSION);
    LibraryContext::instance()->set_default_parser<TreeSitterParser>(
        IParser::CAPABILITIES_SOURCE_PARSER);
    LibraryContext::instance()->set_default_parser<ExtensionAPIParser>(
        IParser::CAPABILITIES_JSON_CONFIG_PARSER);

    Ref<IParser> parser =
        LibraryContext::instance()->get_default_parser(IParser::CAPABILITIES_SOURCE_PARSER);
    Ref<Context> context = node_new<Namespace>();
    Ref<ParserError> error = parser->parse_file(
        "/home/luca/Repositories/godot-object-compiler/tests/files/class_tests/simple_class.h",
        context);
    std::cout << context->pretty_print();
    return 0;

    Vector<String> args;
    for (int i = 1; i < argc; i++) {
        args.emplace_back(argv[i]);
    }

    Application application;
    LibraryContext::instance()->set_error_level(ERROR, FULL);
    return application.run(args);
}
