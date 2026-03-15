/**************************************************************************/
/* node_handler.cpp                                                       */
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

#include "node_handler.h"

namespace GodotObjectCompiler {

ParserStep ParserStep::Undecided() {
	return { TYPE_UNDECIDED, nullptr };
}

ParserStep ParserStep::StepInto() {
	return { TYPE_STEP_INTO, nullptr };
}

ParserStep ParserStep::StepOver() {
	return { TYPE_STEP_OVER, nullptr };
}

ParserStep ParserStep::StepOut() {
	return { TYPE_STEP_OUT, nullptr };
}

ParserStep ParserStep::GoTo(const Ref<TreeSitterNode> &target) {
	return { TYPE_GO_TO, target };
}

bool ParserStep::is_undecided() const {
	return type == TYPE_UNDECIDED;
}

bool ParserStep::is_step_into() const {
	return type == TYPE_STEP_INTO;
}

bool ParserStep::is_step_over() const {
	return type == TYPE_STEP_OVER;
}

bool ParserStep::is_step_out() const {
	return type == TYPE_STEP_OUT;
}

bool ParserStep::is_go_to(Ref<TreeSitterNode> &p_target) const {
	const bool result = type == TYPE_GO_TO;
	if (result) {
		p_target = goto_target;
	}
	return result;
}

} // namespace GodotObjectCompiler
