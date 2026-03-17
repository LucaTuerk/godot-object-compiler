/**************************************************************************/
/* simple_handlers.h                                                      */
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

#include "library/parser/node_handler.h"
#include "library/parser/parser.h"

namespace GodotObjectCompiler {

class TemplateDeclarationInto : public IntoHandler<TemplateDeclarationInto> {
  NODE_HANDLER(TemplateDeclarationInto);

 public:
  static inline String into_type = "template_declaration";
};

class ConditionalExpressionSkip
    : public SkipHandler<ConditionalExpressionSkip> {
  NODE_HANDLER(ConditionalExpressionSkip);

 public:
  static inline String skip_type = "conditional_expression";
};

class CompoundStatementSkip : public SkipHandler<CompoundStatementSkip> {
  NODE_HANDLER(CompoundStatementSkip);

 public:
  static inline String skip_type = "compound_statement";
};

class PreprocCallSkip : public SkipHandler<PreprocCallSkip> {
  NODE_HANDLER(PreprocCallSkip);

 public:
  static inline String skip_type = "preproc_call";
};

class OperatorSkip : public SkipHandler<OperatorSkip> {
  NODE_HANDLER(OperatorSkip);

 public:
  static inline String skip_type = "operator";
};

}  // namespace GodotObjectCompiler
