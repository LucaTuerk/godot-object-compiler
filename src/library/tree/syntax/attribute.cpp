/**************************************************************************/
/* attribute.cpp                                                          */
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

#include "attribute.h"

namespace GodotObjectCompiler
{

    Ref<Node> Attribute::resolve_target() const
    {
        switch (_get_target()) {
        case NEXT: {
            Ref<Node> target = get_next_sibling();
            if (verify_target(target)) {
                return target;
            }
            return nullptr;
        } break;
        case CONTAINING: {
            return find_parent<Node>(
                [this](const Ref<Node>& p_parent) { return verify_target(p_parent); });
        } break;
        case NONE: {
            return nullptr;
        } break;
        default: {
            PANIC("UNIMPLEMENTED");
        }
        }
    }

    bool Attribute::verify_target(const Ref<Node>& p_resolved) const
    {
        if (!_verify_target_class(p_resolved)) {
            return false;
        }

        return _verify_target(p_resolved);
    }

    Ref<IAttributeArgumentParser> Attribute::get_argument_parser()
    {
        return nullptr;
    }

    bool Attribute::_verify_target(const Ref<Node>& p_resolved) const
    {
        UNUSED(p_resolved);
        return true;
    }
    String UnparsedAttributeArguments::to_string() const
    {
        return format("%s: \"%s\"", Node::to_string().c_str(), content.c_str());
    }

    bool UnparsedAttributeArguments::copy_to(const Ref<Node>& p_other) const
    {
        COPY_GUARD(UnparsedAttributeArguments, Node);
        target->content = content;
        return true;
    }

    void UnparsedAttributeArguments::write_to(IStructuredWriter* p_writer)
    {
        Node::write_to(p_writer);
        p_writer->write<String, String>("_content", content);
    }

    void UnparsedAttributeArguments::read_from(IStructuredReader* p_reader)
    {
        Node::read_from(p_reader);
        content = p_reader->read<String, String>("_content");
    }

} // namespace GodotObjectCompiler