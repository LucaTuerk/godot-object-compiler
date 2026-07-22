/**************************************************************************/
/* generate.cpp                                                           */
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

#include "generate.h"

#include "application/arguments/argument_lists.h"
#include "application/arguments/argument_parsers.h"
#include "application/arguments/flag_argument.h"
#include "application/arguments/path_argument.h"
#include "generate_bindings.h"
#include "generate_type_db.h"

namespace GodotObjectCompiler
{
    CommandLineArgumentParseResult
    Generate::register_required_arguments(ApplicationContext& p_context) const
    {
        return p_context.register_argument_lists<GDExtensionProjectArguments, GenerateArguments>();
    }

    Ref<ProgramError> Generate::execute(ApplicationContext& p_context)
    {
        const auto project_arguments = p_context.get_argument_list<GDExtensionProjectArguments>();
        const auto generate_arguments = p_context.get_argument_list<GenerateArguments>();

        GenerateTypeDB generate_type_db;
        GenerateBindings generate_bindings;

        if (generate_arguments->flags->value_equals(REGENERATE_BINDINGS)) {
            for (const Path& input_file : project_arguments->sources->get<Vector<Path>>()) {
                LibraryContext::instance()->force_regenerate(input_file);
            }
        }

        Ref<ProgramError> generate_type_db_error = generate_type_db.run(p_context);
        if (generate_type_db_error != ProgramError::OK) {
            return generate_type_db_error;
        }

        Ref<ProgramError> generate_bindings_error = generate_bindings.run(p_context);
        if (generate_bindings_error != ProgramError::OK) {
            return generate_bindings_error;
        }

        return ProgramError::OK;
    }

} // namespace GodotObjectCompiler
