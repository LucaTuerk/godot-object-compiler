/**************************************************************************/
/* function.h                                                             */
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

#include "access_specifier.h"
#include "context.h"
#include "library/core/lazy.h"
#include "literal.h"
#include "member.h"

namespace GodotObjectCompiler {

  class Type;

  class Arguments : public Context {
    NODE_TYPE(Arguments)
  };

  class Argument : public Context {
    NODE_TYPE(Argument)
  };

  class Parameters : public Context {
    NODE_TYPE(Parameters);
  };

  class TemplateParameters : public Context {
    NODE_TYPE(TemplateParameters);
  };

  class TemplateArguments : public Context {
    NODE_TYPE(TemplateArguments);
  };

  class Parameter : public NamedContext {
    NODE_TYPE(Parameter);

    LAZY(Parameter, Ref<Type>, type)
    LAZY(Parameter, Ref<Literal>, default_value);
  };

  class Function : public Member {
    NODE_TYPE(Function);

    /* child nodes */
    LAZY(Function, Ref<Type>, type);
    LAZY(Function, Ref<Parameters>, parameters);
    LAZY(Function, Ref<Arguments>, arguments);
    LAZY(Function, Vector<Ref<Literal>>, default_values);

   public:

    bool copy_to(Ref<Node> p_other) const override;
  };

}
