/**************************************************************************/
/* attribute.h                                                            */
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
#include "context.h"
#include "library/parser/attribute_argument_parser.h"

#define ATTRIBUTE_TYPE(node_type, target_type, target)                                                             \
  NODE_TYPE(node_type)                                                                                             \
 protected:                                                                                                        \
                                                                                                                   \
  virtual bool _verify_target_class(Ref<Node> p_resolved) const override { return p_resolved->is<target_type>(); } \
                                                                                                                   \
  virtual Target _get_target() const override { return target; }                                                   \
                                                                                                                   \
 public:                                                                                                           \
                                                                                                                   \
  Ref<target_type> Target##target_type() {                                                                         \
    Ref<Node> node_target = resolve_target();                                                                      \
    if (!node_target) {                                                                                            \
      return nullptr;                                                                                              \
    }                                                                                                              \
    return node_target->as<target_type>();                                                                         \
  }                                                                                                                \
                                                                                                                   \
 private:

namespace GodotObjectCompiler {

  class Attribute : public NamedContext {
   public:

    enum Target {
      NEXT,        // attribute applies to next sibling in the context
      CONTAINING,  // attribute applies to the containing context
      NONE,
    };

    Ref<Node> resolve_target() const;
    bool verify_target(Ref<Node> p_resolved) const;
    virtual Ref<IAttributeArgumentParser> get_argument_parser();

    Size start;
    Size end;
    Size line;

   protected:

    virtual Target _get_target() const = 0;
    virtual bool _verify_target_class(Ref<Node> p_resolved) const = 0;
    virtual bool _verify_target(Ref<Node> p_resolved) const;
  };

}  // namespace GodotObjectCompiler
