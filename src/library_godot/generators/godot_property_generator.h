/**************************************************************************/
/* godot_property_generator.h                                             */
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
#include "library/attribute_db.h"
#include "library/generator/generator.h"
#include "library_godot/attributes/godot_attributes.h"

namespace GodotObjectCompiler {

  class GodotPropertyGenerator : public IClassGenerator<GodotPropertyAttribute> {
    GENERATOR(GodotPropertyGenerator);

   public:
	Ref<GeneratorError> check_for_property_type_errors(const Ref<Node> &p_target, const Ref<Type> &p_type, const Ref<Namespace> &p_from_namespace);
	Ref<GeneratorError> check_for_field_property_errors(const Ref<Field> &p_field, const Ref<GodotPropertyAttribute> &p_attribute, const Ref<Namespace> &p_from_namespace);
  	Ref<GeneratorError> check_for_custom_bound_property_errors(const GodotCustomPropertyBind &custom_bind, const Ref<GodotPropertyAttribute> &p_attribute, const Ref<Namespace> &p_from_namespace);

    Ref<GeneratorError> do_generate_default_attribute_arguments(
        Ref<Class> p_target_class, Ref<GodotPropertyAttribute> p_attribute, Ref<Context> p_default_values) override;

    Ref<GeneratorError> do_generate(Ref<Class> p_target_class, Ref<GodotPropertyAttribute> p_attribute, ClassGeneratorResult &r_result) override;
  };

  REGISTER_CLASS_GENERATOR(GodotPropertyGenerator);

}
