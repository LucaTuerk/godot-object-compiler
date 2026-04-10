/**************************************************************************/
/* assumptions.cpp                                                        */
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
#include "assumptions.h"

namespace GodotObjectCompiler
{
    namespace AssumedGodotTypes
    {

        bool validate_assumptions()
        {
            bool success = true;

            success &= ClassDB.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &= Object.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &=
                RefCounted.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &= Resource.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &= Node.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &= GodotRef.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &= ModuleInitializationLevel.validate(
                           LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &=
                PropertyHintEnum.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &= PropertyUsageFlagsEnum.validate(LibraryContext::instance()->get_type_db()) ==
                       STATE_VALID;
            success &=
                ADD_PROPERTY.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &= ADD_GROUP.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &=
                ADD_SUBGROUP.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &= Variant.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &=
                VariantTypeEnum.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &= Array.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &=
                TypedArray.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &=
                Dictionary.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &=
                TypedDictionary.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &= GDCLASS.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &=
                GDREGISTER_CLASS.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &= GDREGISTER_VIRTUAL_CLASS.validate(
                           LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &= GDREGISTER_ABSTRACT_CLASS.validate(
                           LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &= GDREGISTER_INTERNAL_CLASS.validate(
                           LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &= GDREGISTER_RUNTIME_CLASS.validate(
                           LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &=
                GDVIRTUAL_CALL.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &=
                GDVIRTUAL_BIND.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &= BIND_ENUM_CONSTANT.validate(LibraryContext::instance()->get_type_db()) ==
                       STATE_VALID;
            success &= BIND_BITFIELD_FLAG.validate(LibraryContext::instance()->get_type_db()) ==
                       STATE_VALID;
            success &= VARIANT_ENUM_CAST.validate(LibraryContext::instance()->get_type_db()) ==
                       STATE_VALID;
            success &= VARIANT_BITFIELD_CAST.validate(LibraryContext::instance()->get_type_db()) ==
                       STATE_VALID;
            success &=
                MultiplayerAPI.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &=
                MultiplayerPeer.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &= MultiplayerAPIRPCMode.validate(LibraryContext::instance()->get_type_db()) ==
                       STATE_VALID;
            success &= MultiplayerPeerTransferMode.validate(
                           LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &=
                StringName.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &= Callable.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &= List.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;
            success &=
                PropertyInfo.validate(LibraryContext::instance()->get_type_db()) == STATE_VALID;

            return success;
        }

    } // namespace AssumedGodotTypes
} // namespace GodotObjectCompiler
