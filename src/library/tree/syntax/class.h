/**************************************************************************/
/* class.h                                                                */
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

#include "attribute.h"
#include "library/core/lazy.h"
#include "namespace.h"

namespace GodotObjectCompiler {

  class Field;
  class Function;
  class Body;

  class BaseClasses : public Context {
    NODE_TYPE(BaseClasses);
  };

  class Class : public Namespace {
    NODE_TYPE(Class);

  public:

  	LAZY(Class, Size, template_parameter_count);
    /* collections */
    LAZY(Class, Vector<String>, direct_bases_names);
    LAZY_MUT(Class, Vector<Ref<Class>>, base_classes);
    LAZY(Class, bool, has_generated_class_attribute);
    LAZY(Class, Vector<Ref<Attribute>>, attributes);
	LAZY(Class, String, first_base_qualifed_name);
    // Functions
    LAZY(Class, Vector<Ref<Function>>, member_functions);
    LAZY(Class, Vector<Ref<Function>>, public_member_functions);
    LAZY(Class, Vector<Ref<Function>>, protected_member_functions);
    LAZY(Class, Vector<Ref<Function>>, private_member_functions);
    // Fields
    LAZY(Class, Vector<Ref<Field>>, member_fields);
    LAZY(Class, Vector<Ref<Field>>, public_member_fields);
    LAZY(Class, Vector<Ref<Field>>, protected_member_fields);
    LAZY(Class, Vector<Ref<Field>>, private_member_fields);

   public:

    bool has_function_named(const String& name) const;
    bool copy_to(const Ref<Node>& p_other) const override;
  };

}
