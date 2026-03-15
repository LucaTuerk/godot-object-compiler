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

namespace GodotObjectCompiler {
namespace AssumedGodotTypes {

bool validate_assumptions() {
	bool success = true;

	success &= ClassDB.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= Object.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= RefCounted.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= Resource.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= Node.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= GodotRef.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= ModuleInitializationLevel.validate(ExecutionContext::instance()->get_type_db()) ==
			STATE_VALID;
	success &=
			PropertyHintEnum.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &=
			PropertyUsageFlagsEnum.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= ADD_PROPERTY.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= Variant.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= VariantTypeEnum.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= Array.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= TypedArray.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= Dictionary.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= TypedDictionary.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= GDCLASS.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &=
			GDREGISTER_CLASS.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= GDREGISTER_VIRTUAL_CLASS.validate(ExecutionContext::instance()->get_type_db()) ==
			STATE_VALID;
	success &= GDREGISTER_ABSTRACT_CLASS.validate(ExecutionContext::instance()->get_type_db()) ==
			STATE_VALID;
	success &= GDREGISTER_INTERNAL_CLASS.validate(ExecutionContext::instance()->get_type_db()) ==
			STATE_VALID;
	success &= GDREGISTER_RUNTIME_CLASS.validate(ExecutionContext::instance()->get_type_db()) ==
			STATE_VALID;
	success &= GDVIRTUAL_CALL.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= GDVIRTUAL_BIND.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &=
			BIND_ENUM_CONSTANT.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &=
			BIND_BITFIELD_FLAG.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &=
			VARIANT_ENUM_CAST.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &=
			VARIANT_BITFIELD_CAST.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= MultiplayerAPI.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= MultiplayerPeer.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &=
			MultiplayerAPIRPCMode.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= MultiplayerPeerTransferMode.validate(ExecutionContext::instance()->get_type_db()) ==
			STATE_VALID;
	success &= StringName.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= Callable.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= List.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;
	success &= PropertyInfo.validate(ExecutionContext::instance()->get_type_db()) == STATE_VALID;

	return success;
}

} // namespace AssumedGodotTypes
} // namespace GodotObjectCompiler
