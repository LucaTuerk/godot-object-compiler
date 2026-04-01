/**************************************************************************/
/* node_db.cpp                                                            */
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

#include "node_db.h"

#include "tree/syntax/node.h"

namespace GodotObjectCompiler
{

    UID NodeDB::request_id_change(UID p_from, UID p_to)
    {
        if (p_from == INVALID_ID) {
            print_err("Trying to change a node id from invalid id. This is not possible.");
            return p_from;
        }

        if (p_to == INVALID_ID) {
            p_to = _generate_unique_id();
        }

        if (_nodes.find(p_from) == _nodes.end()) {
            print_err("Trying to change a node id, but no node is registered for this id.");
            return p_from;
        }

        _uids.erase(p_from);
        _uids.insert(p_to);
        _nodes[p_to] = _nodes[p_from];
        _nodes.erase(p_from);
        return p_to;
    }

    NodeDB::~NodeDB()
    {
    }

    Ref<Node> NodeDB::create(const String& p_type)
    {
        NodeDB* db = LibraryContext::instance()->get_node_db();

        auto itr = db->_node_constructors.find(p_type);

        if (itr == db->_node_constructors.end()) {
            return nullptr;
        }

        return itr->second();
    }

    bool NodeDB::register_node_constructor(const String& p_name, NodeCreator p_creator)
    {
        return _node_constructors.try_emplace(p_name, p_creator).second;
    }

    UID NodeDB::_generate_unique_id()
    {
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<uint64_t> dis;

        UID generated = dis(gen);
        while (_has_uid(generated)) {
            generated = dis(gen);
        }

        _uids.insert(generated);
        return generated;
    }

    bool NodeDB::_has_uid(UID p_uid)
    {
        return _uids.find(p_uid) != _uids.end();
    }

} // namespace GodotObjectCompiler
