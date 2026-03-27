/**************************************************************************/
/* type.cpp                                                               */
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
#include "type.h"

#include "identifier.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"
#include "library/tree/output/output_transformator.h"
#include "library/type_db.h"
#include "modifiers.h"

namespace GodotObjectCompiler
{

    String Type::_type_name_lazy_get()
    {
        StreamWriter writer;
        OutputTransformator().transform(shared_from_this())->get_output(&writer);
        return string_shrink_inner_space(string_trim(writer.get_string()));
    }

    String Type::_type_name_untemplated_lazy_get() const
    {
        StreamWriter writer;
        Ref<Type> temp = node_new<Type>();
        for (const Ref<Node>& child : _children) {
            if (child->is<TemplateParameters>() || child->is<TemplateArguments>()) {
                continue;
            }
            temp->add_child(child->clone());
        }

        OutputTransformator().transform(temp)->get_output(&writer);
        return string_shrink_inner_space(string_trim(writer.get_string()));
    }

    String Type::_type_name_unmodified_lazy_get() const
    {
        StreamWriter writer;
        Ref<Type> temp = node_new<Type>();
        for (const Ref<Node>& child : _children) {
            if (child->is<TypeQualifier>() || child->is<Pointer>() || child->is<Reference>()) {
                continue;
            }

            temp->add_child(child->clone());
        }

        OutputTransformator().transform(temp)->get_output(&writer);
        return string_shrink_inner_space(string_trim(writer.get_string()));
    }

    String Type::_type_name_unmodified_ptr_lazy_get() const
    {
        StreamWriter writer;
        Ref<Type> temp = node_new<Type>();
        for (const Ref<Node>& child : _children) {
            if (child->is<TypeQualifier>() || child->is<Reference>()) {
                continue;
            }

            temp->add_child(child->clone());
        }

        OutputTransformator().transform(temp)->get_output(&writer);
        return string_shrink_inner_space(string_trim(writer.get_string()));
    }

    Size Type::_template_argument_count_lazy_get() const
    {
        if (!is_template_type()) {
            return 0;
        }
        return template_arguments()->get_child_count();
    }

    bool Type::_is_template_type_lazy_get() const
    {
        return template_arguments() != nullptr;
    }

    Ref<TemplateArguments> Type::_template_arguments_lazy_get() const
    {
        return find_child<TemplateArguments>();
    }

    Ref<Type> Type::_qualified_lazy_get() const
    {
        Ref<Type> result = clone<Type>();
        if (is_qualified) {
            return result;
        }

        Ref<TemplateArguments> template_arguments = result->template_arguments();
        if (template_arguments) {
            Ref<TemplateArguments> replacement_arguments = node_new<TemplateArguments>();
            for (const Ref<Node>& argument : *this->template_arguments()) {
                if (Ref<Type> type = argument->as<Type>(); type) {
                    replacement_arguments->add_child(type->qualified());
                } else {
                    replacement_arguments->add_child(argument->clone());
                }
            }
            result->replace_child(template_arguments, replacement_arguments);
        }

        Ref<Namespace> namespace_ = find_ancestor<Namespace>();
        Ref<Identifier> identifier = result->find_child<Identifier>();
        Result<Node> type_result =
            ExecutionContext::instance()->get_type_db()->get_type_data(result, namespace_);
        Ref<NamedContext> named =
            type_result.has_result() ? type_result.get_result()->as<NamedContext>() : nullptr;
        if (type_result.has_error()) {
            type_result.get_error()->set_handled();
        }

        if (named) {
            result->replace_child(identifier, node_new<Identifier>(named->qualified_name()));
        } else {
            result->replace_child(identifier, node_new<Identifier>(name()));
        }
        result->is_qualified = true;
        return result;
    }

} // namespace GodotObjectCompiler