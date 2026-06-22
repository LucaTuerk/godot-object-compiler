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

#include "library/core/string_utilities.h"
#include "library/tree/predicates.h"
#include "library_godot/parsers/godot_attribute_argument_parser.h"

namespace GodotObjectCompiler
{

    Ref<IAttributeArgumentParser> GodotAttributeWithParams::get_argument_parser()
    {
        return make_ref<GodotAttributeArgumentParser>(this->as<Attribute>());
    }

    Ref<Arguments> GodotAttributeWithParams::_arguments_lazy_get() const
    {
        return find_child<Arguments>();
    }

    String get_string_literal_content(const Ref<const Attribute>& p_attribute)
    {
        const Ref<Literal> literal =
            p_attribute->find_chain<Literal, Arguments, StringLiteralArgument>();

        if (!literal) {
            ERR("Failed to find literal");
            return "";
        }

        if (!string_enclosed_by(literal->content, "\"") || literal->content.size() < 2) {
            ERR("Expected string literal");
            return "";
        }

        return literal->content.substr(1, literal->content.size() - 2);
    }

    String GodotPropertyCategoryAttribute::_literal_content_lazy_get() const
    {
        return get_string_literal_content(const_as<Attribute>());
    }

    String GodotPropertyGroupAttribute::_literal_content_lazy_get() const
    {
        return get_string_literal_content(const_as<Attribute>());
    }

    String GodotPropertySubgroupAttribute::_literal_content_lazy_get() const
    {
        return get_string_literal_content(const_as<Attribute>());
    }

    Ref<Field> GodotPropertyAttribute::TargetField()
    {
        Ref<Arguments> args = arguments();
        if (!args) {
            return nullptr;
        }

        Ref<GodotCustomPropertyGetSetArgument> get_set =
            args->find_child<GodotCustomPropertyGetSetArgument>();
        if (!get_set) {
            return get_next_sibling<Field>();
        }

        Ref<Identifier> bind_identifier = get_set->find_child<Identifier>();
        if (!bind_identifier ||
            bind_identifier->name == GodotCustomPropertyGetSetParameterType::AutoGetSet) {
            return get_next_sibling<Field>();
        }
        return nullptr;
    }

    Opt<GodotCustomPropertyBind> GodotPropertyAttribute::CustomBind()
    {
        Ref<Arguments> args = arguments();
        if (!args) {
            return std::nullopt;
        }

        Ref<Literal> name_literal = args->find_chain<Literal, StringLiteralArgument>();
        if (!name_literal) {
            return std::nullopt;
        }

        Ref<GodotCustomPropertyGetSetArgument> get_set =
            args->find_child<GodotCustomPropertyGetSetArgument>();
        if (!get_set) {
            return std::nullopt;
        }

        Vector<Ref<Literal>> literals = get_set->find_children<Literal>(true);
        if (literals.size() != 2) {
            return std::nullopt;
        }

        Ref<Literal> get_literal = literals[0];
        Ref<Literal> set_literal = literals[1];

        Ref<Class> containing_class = find_ancestor<Class>();
        if (!containing_class) {
            return std::nullopt;
        }

        String property_name, getter_name, setter_name;
        if (!name_literal->unwrap_string_literal(property_name)) {
            return std::nullopt;
        }

        if (!get_literal->unwrap_string_literal(getter_name)) {
            return std::nullopt;
        }

        if (!set_literal->unwrap_string_literal(setter_name)) {
            return std::nullopt;
        }

        Ref<Function> getter = containing_class->body()->find_child<Function>(
            0, NamedContextPredicates::name<Function>(getter_name.c_str()));
        Ref<Function> setter = containing_class->body()->find_child<Function>(
            0, NamedContextPredicates::name<Function>(setter_name.c_str()));

        return {{
            property_name,
            getter_name,
            setter_name,
            getter,
            setter,
        }};
    }

    bool GodotPropertyAttribute::_verify_target_class(Ref<Node> p_resolved) const
    {
        Ref<Arguments> args = arguments();
        if (!args) {
            return false;
        }

        Ref<Identifier> get_set_identifier =
            args->find_chain<Identifier, Arguments, GodotCustomPropertyGetSetArgument>();
        if (!get_set_identifier ||
            get_set_identifier->name == GodotCustomPropertyGetSetParameterType::AutoGetSet) {
            return p_resolved->is<Field>();
        }
        return true;
    }

    Attribute::Target GodotPropertyAttribute::_get_target() const
    {
        Ref<Arguments> args = arguments();
        if (!args) {
            return NONE;
        }

        Ref<Identifier> get_set_identifier =
            args->find_chain<Identifier, Arguments, GodotCustomPropertyGetSetArgument>();
        if (!get_set_identifier ||
            get_set_identifier->name == GodotCustomPropertyGetSetParameterType::AutoGetSet) {
            return NEXT;
        }
        return NONE;
    }

} // namespace GodotObjectCompiler
