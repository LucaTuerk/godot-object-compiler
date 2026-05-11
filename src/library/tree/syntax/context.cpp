/**************************************************************************/
/* context.cpp                                                            */
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
#include "context.h"

#include "function.h"
#include "identifier.h"
#include "include.h"
#include "library/core/string_utilities.h"
#include "library/core/string_writer.h"
#include "library/type_db.h"
#include "namespace.h"

namespace GodotObjectCompiler
{

    bool Context::copy_to(const Ref<Node>& p_other) const
    {
        COPY_GUARD(Context, Node);

        for (const Ref<Node>& child : _children) {
            const Ref<Node> cloned = child->clone();
            if (!cloned) {
                return false;
            }
            target->add_child(cloned);
        }

        return true;
    }

    void Context::add_child(Ref<Node> p_child)
    {
        PANIC_COND(p_child == nullptr, "Trying to add null child.");

        Ref<Context> parent = p_child->get_parent();
        if (parent != nullptr) {
            if (this == parent.get()) {
                return;
            }
            return p_child->reparent(this->as<Context>());
        }

        if (p_child.get() == this) {
            return;
        }

        p_child->_parent = this->as<Context>();
        p_child->_index = _children.size();
        _children.push_back(p_child);
    }

    void Context::add_child_before(Ref<Node> p_child, Ref<Node> p_existing)
    {
        auto itr = std::find(_children.begin(), _children.end(), p_existing);
        PANIC_COND(p_child == nullptr, "Trying to add null child.");
        PANIC_COND(p_existing == nullptr, "Existing child is null.");
        PANIC_COND(p_child->get_parent() != nullptr, "Child is already parented");
        PANIC_COND(
            itr == _children.end(), "Cannot insert child after existing child. Was not found.");

        itr = _children.insert(itr, p_child);
        Index index = p_existing->_index;
        p_child->_index = index;
        p_child->_parent = this->as<Context>();
        while (itr != _children.end()) {
            (*(itr++))->_index = index++;
        }
    }

    void Context::add_children(std::initializer_list<Ref<Node>>&& p_children)
    {
        for (const Ref<Node>& child : p_children) {
            add_child(child);
        }
    }

    Size Context::get_child_count() const
    {
        return _children.size();
    }

    Size Context::get_descendant_count() const
    {
        Size count = 0;
        for (const Ref<Node>& child : _children) {
            if (const Ref<Context>& context_child = child->as<Context>()) {
                count += context_child->get_descendant_count() + 1;
            } else {
                count += 1;
            }
        }
        return count;
    }

    Ref<Node> Context::get_child(SignedIndex p_idx) const
    {
        Index actual_idx = p_idx;
        if (p_idx < 0) {
            actual_idx = _children.size() + p_idx;
        }

        if (actual_idx >= _children.size()) {
            return nullptr;
        }

        auto itr = _children.begin();
        std::advance(itr, actual_idx);
        return *itr;
    }

    Ref<Node> Context::get_child_strict(SignedIndex p_idx) const
    {
        if (p_idx < 0 || static_cast<Size>(p_idx) >= _children.size()) {
            return nullptr;
        }

        return get_child(p_idx);
    }

    List<Ref<Node>>& Context::get_children()
    {
        return _children;
    }

    Context::ChildIterator Context::begin()
    {
        return _children.begin();
    }

    Context::ChildIterator Context::end()
    {
        return _children.end();
    }

    Context::ChildIterator Context::remove_child(decltype(_children)::iterator p_itr)
    {
        const Ref<Node> child = *p_itr;
        p_itr = _children.erase(p_itr);
        child->_index = 0;
        child->_parent = {};

        auto next = p_itr;
        while (next != _children.end()) {
            (*next)->_index -= 1;
            ++next;
        }

        return p_itr;
    }

    Context::ChildIterator
    Context::reparent_child(decltype(_children)::iterator p_itr, Ref<Context> p_new_parent)
    {
        if (p_itr != _children.end()) {
            Ref<Node> child = *p_itr;
            p_itr = remove_child(p_itr);
            p_new_parent->add_child(child);
            return p_itr;
        }

        return _children.end();
    }

    void Context::remove_all_children()
    {
        while (!_children.empty()) {
            Ref<Node> child = _children.back();
            _children.pop_back();
            child->_parent = {};
        }
    }

    void Context::remove_child(const Ref<Node>& p_child)
    {
        const auto itr = std::find(_children.begin(), _children.end(), p_child);
        remove_child(itr);
    }

    void Context::replace_child(
        const Ref<Node>& p_child, const Ref<Node>& p_new_child, bool p_take_children)
    {
        auto itr = std::find(_children.begin(), _children.end(), p_child);
        if (itr != _children.end()) {
            *itr = p_new_child;

            UID child_uid = p_child->get_id();
            p_new_child->_index = p_child->_index;
            p_new_child->_parent = this->as<Context>();
            p_child->_id = LibraryContext::instance()->get_node_db()->request_id_change(child_uid);
            p_new_child->_id = LibraryContext::instance()->get_node_db()->request_id_change(
                p_new_child->get_id(), child_uid);
            p_child->_parent = {};

            Ref<Context> child_context = p_child->as<Context>();
            Ref<Context> new_child_context = p_new_child->as<Context>();

            if (p_take_children && child_context && new_child_context) {
                for (auto itr = child_context->_children.begin();
                     itr != child_context->_children.end();) {
                    itr = child_context->reparent_child(itr, new_child_context);
                }
            }
        }
    }

    bool Context::empty() const
    {
        return _children.empty();
    }

    void Context::write_to(IStructuredWriter* p_writer)
    {
        Node::write_to(p_writer);
    }

    String NamedContext::_name_lazy_get() const
    {
        Ref<Identifier> identifier = find_child<Identifier>();
        if (!identifier) {
            return "";
        }
        return identifier->name;
    }

    Ref<Body> NamedContext::_body_lazy_get() const
    {
        return find_child<Body>();
    }

    String NamedContext::_qualified_name_lazy_get() const
    {
        StreamWriter writer;

        if (Ref<Namespace> ns = find_ancestor<Namespace>()) {
            auto parent_qualified_name = ns->qualified_name();
            if (!parent_qualified_name.empty()) {
                writer.write(parent_qualified_name);
                writer.write("::");
            }
        }

        writer.write(name());
        return writer.get_string();
    }

    bool NamedContext::copy_to(const Ref<Node>& p_other) const
    {
        COPY_GUARD(NamedContext, Context);
        COPY_LAZY(name);
        COPY_LAZY(qualified_name);
        COPY_LAZY(namespaces_names);
        target->header = header;
        return true;
    }

    void NamedContext::read_from(IStructuredReader* p_reader)
    {
        Context::read_from(p_reader);
        _name_lazy = p_reader->read<String, String>("_name");
        _qualified_name_lazy = p_reader->read<String, String>("_qualified_name");
        _namespaces_names_lazy =
            string_split(p_reader->read<String, String>("_namespaces_names"), ",");
        header = p_reader->read<String, String>("header");
    }

    void NamedContext::write_to(IStructuredWriter* p_writer)
    {
        Context::write_to(p_writer);
        p_writer->write<String, String>("_name", name());
        p_writer->write<String, String>("_qualified_name", qualified_name());
        p_writer->write<String, String>(
            "_namespaces_names", string_vector_combine(namespaces_names(), ","));
        p_writer->write<String, String>("header", header);
    }

    Vector<String> NamedContext::_namespaces_names_lazy_get() const
    {
        Vector<String> names;
        Ref<const NamedContext> current = this->const_as<NamedContext>();
        if (current->is<Namespace>()) {
            names.push_back(current->name());
        }

        while (const Ref<Namespace>& namespace_ = current->find_ancestor<Namespace>()) {
            if (!namespace_->name().empty()) {
                names.push_back(namespace_->name());
            }
            current = namespace_;
        }

        Vector<String> reversed(names.size());
        std::copy(names.rbegin(), names.rend(), reversed.begin());

        return reversed;
    }

    String NamedContext::_mangled_name_lazy_get() const
    {
        Size template_parameter_count = 0;
        for (const Ref<Node>& child : _children) {
            if (const Ref<TemplateParameters> parameters = child->as<TemplateParameters>()) {
                template_parameter_count = parameters->get_child_count();
            } else if (const Ref<TemplateArguments> arguments = child->as<TemplateArguments>()) {
                template_parameter_count = arguments->get_child_count();
            }
        }

        return TypeDB::mangle_name(qualified_name(), template_parameter_count);
    }

} // namespace GodotObjectCompiler
