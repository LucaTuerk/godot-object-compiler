/**************************************************************************/
/* literal.cpp                                                            */
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

#include "literal.h"

namespace GodotObjectCompiler {

Ref<Literal> Literal::StringLiteral(const String &content) {
	return node_new<Literal>("\"" + content + "\"");
}

String Literal::to_string() const {
	return "Literal: " + content;
}

bool Literal::copy_to(const Ref<Node> &p_other) const {
	COPY_GUARD(Literal, Node);
	target->content = content;
	return true;
}

void Literal::write_to(IStructuredWriter *p_writer) {
	Node::write_to(p_writer);
	p_writer->write("literal_content", content);
}

void Literal::read_from(IStructuredReader *p_reader) {
	Node::read_from(p_reader);
	content = p_reader->read<String, String>("literal_content");
}

bool Literal::unwrap_string_literal(String &p_content) const {
	if (content.size() < 2) {
		p_content = "";
		return false;
	}

	if (content.at(0) != '"' || content.at(content.size() - 1) != '"') {
		return false;
	}
	p_content = content.substr(1, content.size() - 2);
	return true;
}

} // namespace GodotObjectCompiler