/**************************************************************************/
/* include.cpp                                                            */
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
#include "include.h"

#include "../../type_db.h"
#include "library/core/helpers.h"
#include "library/execution_context.h"
#include "library/parser/parser.h"
#include "namespace.h"

namespace GodotObjectCompiler {

  String Include::to_string() const {
    return is_system_include ? "Include: " + include_path : "Include: \"" + include_path + "\"";
  }

  void Include::read_from(IStructuredReader* p_reader) {
    // intentionally read from Node as we do to want to copy the children
    Node::read_from(p_reader);  // NOLINT(*-parent-virtual-call)
    include_path = p_reader->read<String, String>("include_path");
    is_system_include = p_reader->read<String, bool>("is_system_include");
  }

  void Include::write_to(IStructuredWriter* p_writer) {
    // intentionally write to Node as we do to want to copy the children
    Node::write_to(p_writer);  // NOLINT(*-parent-virtual-call)
    p_writer->write("include_path", include_path);
    p_writer->write("is_system_include", is_system_include);
  }

  bool Include::copy_to(Ref<Node> p_other) const {
    // intentionally copying from node as we do to want to copy the children
    COPY_GUARD(Include, Node)  // NOLINT(*-parent-virtual-call)
    target->include_path = include_path;
    target->is_system_include = is_system_include;
    return true;
  }

  Include::Error Include::_evaluate_lazy_get() {
    Ref<Node> result = ExecutionContext::instance()->get_include("", include_path);
    if (result) {
      add_child(result);
      return OK;
    } else {
      return ALREADY_INCLUDED;
    }
  }

}  // namespace GodotObjectCompiler