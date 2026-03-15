/**************************************************************************/
/* godot_function.h                                                       */
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

#pragma once
#include "common.h"
#include "test_registry.h"

using namespace GodotObjectCompiler;

inline bool rpc_config_bound(
		const String &p_function_name, const Vector<String> &p_comp, const String &p_generated_source) {
	const Size config_line =
			find_line_that_contains(p_generated_source, { "rpc_config", p_function_name });
	const Size search_line = find_line_that_contains(p_generated_source, p_comp, config_line - 6);
	return config_line > search_line;
}

#define GOC_ASSERT_RPC_CONFIG(rpc_name, ...) \
	GOC_TEST_ASSERT( \
			rpc_config_bound(rpc_name, { __VA_ARGS__ }, generated_source), \
			"Invalid rpc config for method \"%s\"", rpc_name)

GOC_INTEGRATION_TEST(RpcConfig) {
	GOC_INTEGRATION_TEST_GEN_FILE("tests/files/integration_tests/godot_function/rpc.h");

	GOC_ASSERT_RPC_CONFIG("rpc_function_authority", "rpc_mode", "RPC_MODE_AUTHORITY")
	GOC_ASSERT_RPC_CONFIG("rpc_function_anypeer", "rpc_mode", "RPC_MODE_ANY_PEER")
	GOC_ASSERT_RPC_CONFIG("rpc_function_reliable", "transfer_mode", "TRANSFER_MODE_RELIABLE")
	GOC_ASSERT_RPC_CONFIG("rpc_function_unreliable", "transfer_mode", "TRANSFER_MODE_UNRELIABLE")
	GOC_ASSERT_RPC_CONFIG(
			"rpc_function_unreliable_ordered", "transfer_mode", "TRANSFER_MODE_UNRELIABLE_ORDERED")
	GOC_ASSERT_RPC_CONFIG("rpc_function_call_remote", "call_local", "false")
	GOC_ASSERT_RPC_CONFIG("rpc_function_call_local", "call_local", "true")
	GOC_ASSERT_RPC_CONFIG("rpc_function_channel_one", "channel", "1")

	return TEST_RESULT_SUCCESS;
};