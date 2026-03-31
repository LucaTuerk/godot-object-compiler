/**************************************************************************/
/* node.cpp                                                               */
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
#include "node.h"

#include "identifier.h"
#include "include.h"
#include "library/core/config.h"
#include "library/core/core.h"
#include "library/core/string_utilities.h"
#include "namespace.h"

namespace GodotObjectCompiler
{

    Ref<Node> Node::clone() const
    {
        if (Ref<Node> new_node = create(); copy_to(new_node)) {
            new_node->_tag = _tag;
            return new_node;
        }
        PANIC("Failed to clone node of type %s", get_type().c_str());
    }

    void Node::write_to(IStructuredWriter* p_writer)
    {
        Ref<Context> parent = get_parent();
        p_writer->write("_class", get_type());
        p_writer->write("_id", get_id());
        p_writer->write("_parent", parent ? parent->get_id() : INVALID_ID);
    }

    void Node::read_from(IStructuredReader* p_reader)
    {
        UID before = _id;
        UID new_id = p_reader->read<String, UID>("_id");
        if (before != new_id) {
            _id = LibraryContext::instance()->get_node_db()->request_id_change(before, new_id);
        }
    }

    bool Node::has_parent() const
    {
        return _parent.lock() != nullptr;
    }

    bool Node::has_next_sibling() const
    {
        return has_parent() && _index != get_parent()->get_child_count() - 1;
    }

    bool Node::has_previous_sibling() const
    {
        return has_parent() && _index > 0;
    }

    void Node::set_tag(const String& p_tag)
    {
        _tag = p_tag;
    }

    String Node::get_tag() const
    {
        return _tag;
    }

    String Node::pretty_print() const
    {
        Size dummy;
        return print_pretty_and_get_child_line(nullptr, dummy);
    }

    String Node::print_pretty_and_get_child_line(const Ref<Node>& p_child, Size& p_line) const
    {
        String result;
        String line_prefix;

        Index depth = get_depth();
        for (Index i = 0; i < depth; i++) {
            if (i == depth - 1) {
                result += "    |- ";
                line_prefix += "       ";
            } else {
                line_prefix += "   ";
                result += "   ";
            }
        }

        bool first = true;
        for (const String& line : string_split(to_string(), "\n")) {
            if (!line.empty()) {
                if (first) {
                    result += line + "\n";
                } else {
                    result += line_prefix;
                    result += line;
                    result += "\n";
                }
            }
            first = false;
        }

        if (const auto context = std::dynamic_pointer_cast<const Context>(shared_from_this())) {
            for (Index i = 0; i < context->get_child_count(); i++) {
                auto child = context->get_child(i);

                if (child == p_child && p_child != nullptr) {
                    p_line = 1;
                    std::stringstream strstr(result);
                    String line_str;
                    while (std::getline(strstr, line_str)) {
                        p_line++;
                    }
                }

                result += child->pretty_print();
            }
        }

        return result;
    }

    UID Node::get_id() const
    {
        return _id;
    }

    Index Node::get_index() const
    {
        return _index;
    }

    Index Node::get_depth() const
    {
        Ref<const Node> current = shared_from_this();
        Size i = 0;

        do {
            current = current->get_parent();
            if (i++ > 16) {
                return 16;
            }
        } while (current);

        return i;
    }

    Ref<Context> Node::get_parent() const
    {
        return _parent.lock();
    }

    Ref<Node> Node::get_root()
    {
        if (!_root) {
            Ref<Node> current = shared_from_this();
            while (current) {
                _root = current;
                current = current->get_parent();
            }
        }

        return _root ? _root : shared_from_this();
    }

    void Node::reparent(const Ref<Context>& p_new_parent)
    {
        if (Ref<Context> parent = get_parent()) {
            parent->remove_child(shared_from_this());
        }
        p_new_parent->add_child(shared_from_this());
    }

    Ref<Node> Node::get_sibling(int p_offset) const
    {
        const Ref<Context> parent = get_parent();
        if (parent == nullptr) {
            return nullptr;
        }

        return parent->get_child_strict(static_cast<SignedIndex>(_index) + p_offset);
    }

    Ref<Node> Node::get_next_sibling() const
    {
        return get_sibling(+1);
    }

    Ref<Node> Node::get_previous_sibling() const
    {
        return get_sibling(-1);
    }

} // namespace GodotObjectCompiler
