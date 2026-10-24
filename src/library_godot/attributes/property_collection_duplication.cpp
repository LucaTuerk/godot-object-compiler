/**************************************************************************/
/* property_collection_duplication.cpp                                    */
/*                        ___  ___  ___   ___ _____                       */
/*                       / __|/ _ \|   \ / _ \_   _|                      */
/*                      | (_ | (_) | |) | (_) || |                        */
/*                       \___|\___/|___/ \___/ |_|                        */
/*   ___  ___    _ ___ ___ _____    ___ ___  __  __ ___ ___ _    ___ ___  */
/*  / _ \| _ )_ | | __/ __|_   _|  / __/ _ \|  \/  | _ \_ _| |  | __| _ \ */
/* | (_) | _ \ || | _| (__  | |   | (_| (_) | |\/| |  _/| || |__| _||   / */
/*  \___/|___/\__/|___\___| |_|    \___\___/|_|  |_|_| |___|____|___|_|_\ */
/*                                                                        */
/*              This file is part of godot-object-compiler                */
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

#include "property_collection_duplication.h"

#include "library/tree/syntax/identifier.h"

namespace GodotObjectCompiler
{

    bool PropertyCollectionDuplicationArgument::get_duplication(
        CollectionDuplication& r_duplication) const
    {
        const Ref<Identifier> identifier = find_child<Identifier>();
        if (!identifier) {
            ERR("Failed to find identifer.");
            return false;
        }

        if (identifier->name == GetOriginal) {
            r_duplication = GET_ORIGINAL;
            return true;
        }

        if (identifier->name == GetDuplicated) {
            r_duplication = GET_DUPLICATED;
            return true;
        }

        if (identifier->name == GetDuplicatedDeep) {
            r_duplication = GET_DUPLICATED_DEEP;
            return true;
        }

        return false;
    }

    String PropertyCollectionDuplicationParameterType::get_return_type()
    {
        return "GOC_PropertyCollectionDuplication";
    }

    Vector<String> PropertyCollectionDuplicationParameterType::get_value_names()
    {
        return {
            PropertyCollectionDuplicationArgument::GetOriginal,
            PropertyCollectionDuplicationArgument::GetDuplicated,
            PropertyCollectionDuplicationArgument::GetDuplicatedDeep,
        };
    }

    Vector<IAttributeParameterType::Argument>
    PropertyCollectionDuplicationParameterType::get_arguments()
    {
        return {};
    }
} // namespace GodotObjectCompiler
