/**************************************************************************/
/* godot_attributes.cpp                                                   */
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

#include "library_godot/attributes/godot_attributes.h"

#include "library_godot/parsers/godot_attribute_argument_parser.h"

namespace GodotObjectCompiler {

  Ref<IAttributeArgumentParser> GodotAttributeWithParams::get_argument_parser() {
    return make_ref<GodotAttributeArgumentParser>(this->as<Attribute>());
  }

  Ref<Arguments> GodotAttributeWithParams::_arguments_lazy_get() const { return find_child<Arguments>(); }

  String GodotPropertyGroupAttribute::_literal_content_lazy_get() const {
    const Ref<Literal> literal = find_chain<Literal, Arguments, StringLiteralArgument>();
    if (!literal) {
      ERR("Failed to find literal");
      return "\"\"";
    }
    return literal->content;
  }

  String GodotPropertySubgroupAttribute::_literal_content_lazy_get() const {
    const Ref<Literal> literal = find_chain<Literal, Arguments, StringLiteralArgument>();
    if (!literal) {
      ERR("Failed to find literal");
      return "\"\"";
    }
    return literal->content;
  }

}