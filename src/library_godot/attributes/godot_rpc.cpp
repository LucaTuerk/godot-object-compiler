/**************************************************************************/
/* godot_rpc.cpp                                                          */
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

#include "godot_rpc.h"

#include "library/attribute_db.h"
#include "library/core/string_utilities.h"
#include "library/tree/syntax/enum.h"
#include "library/tree/syntax/identifier.h"
#include "library/type_db.h"
#include "library_godot/assumptions.h"

namespace GodotObjectCompiler
{

    String GodotRpcModeParameterType::get_return_type()
    {
        return "GOC_RpcMode";
    }

    Vector<String> GodotRpcModeParameterType::get_value_names()
    {
        return value_names();
    }

    Vector<IAttributeParameterType::Argument> GodotRpcModeParameterType::get_arguments()
    {
        return {};
    }

    String GodotRpcSyncParameterType::get_return_type()
    {
        return "GOC_RpcSync";
    }

    Vector<String> GodotRpcSyncParameterType::get_value_names()
    {
        return {"CallRemote", "CallLocal"};
    }

    Vector<IAttributeParameterType::Argument> GodotRpcSyncParameterType::get_arguments()
    {
        return {};
    }

    String GodotRpcTransferModeParameterType::get_return_type()
    {
        return "GOC_TransferMode";
    }

    Vector<String> GodotRpcTransferModeParameterType::get_value_names()
    {
        return value_names();
    }

    Vector<IAttributeParameterType::Argument> GodotRpcTransferModeParameterType::get_arguments()
    {
        return {};
    }

    Vector<String> GodotRpcModeParameterType::_value_names_lazy_get()
    {
        Ref<Enum> rpc_mode = AssumedGodotTypes::MultiplayerAPIRPCMode.unwrap().type;
        PANIC_COND(!rpc_mode, "Could not find RPCMode enum.");

        Vector<String> result;
        for (const String& value_name : rpc_mode->value_names()) {
            String exposed = macro_case_to_pascal_case(string_replace(value_name, "RPC_MODE_", ""));
            exposed_name_to_godot_value_name[exposed] = value_name;
            result.push_back(exposed);
        }
        return result;
    }

    Vector<String> GodotRpcTransferModeParameterType::_value_names_lazy_get()
    {
        const Ref<Enum> transfer_mode =
            AssumedGodotTypes::MultiplayerPeerTransferMode.unwrap().type;
        PANIC_COND(!transfer_mode, "Could not find RPCMode enum.");

        Vector<String> result;
        for (const String& value_name : transfer_mode->value_names()) {
            String exposed =
                macro_case_to_pascal_case(string_replace(value_name, "TRANSFER_MODE_", ""));
            exposed_name_to_godot_value_name[exposed] = value_name;
            result.push_back(exposed);
        }
        return result;
    }

    bool GodotRpcModeParameterType::get_godot_value_name(
        const String& p_value_name, String& r_godot_value_name)
    {
        const auto itr = exposed_name_to_godot_value_name.find(p_value_name);
        if (itr == exposed_name_to_godot_value_name.end()) {
            r_godot_value_name = "";
            return false;
        }
        r_godot_value_name = itr->second;
        return true;
    }

    bool GodotRpcTransferModeParameterType::get_godot_value_name(
        const String& p_value_name, String& r_godot_value_name)
    {
        const auto itr = exposed_name_to_godot_value_name.find(p_value_name);
        if (itr == exposed_name_to_godot_value_name.end()) {
            r_godot_value_name = "";
            return false;
        }
        r_godot_value_name = itr->second;
        return true;
    }

    String GodotRpcChannelParameterType::get_return_type()
    {
        return "GOC_RpcChannel";
    }

    Vector<String> GodotRpcChannelParameterType::get_value_names()
    {
        return {"Channel"};
    }

    Vector<IAttributeParameterType::Argument> GodotRpcChannelParameterType::get_arguments()
    {
        return {{ARG_INTEGER, "int", "p_channel", false}};
    }

    String GodotRpcModeArgument::_rpc_mode_lazy_get() const
    {
        const Ref<Identifier> identifier = find_child<Identifier>();
        PANIC_COND(!identifier, "Failed to find identifier.");
        return identifier->name;
    }

    String GodotRpcModeArgument::_godot_rpc_mode_lazy_get() const
    {
        Ref<Identifier> identifier = find_child<Identifier>();
        PANIC_COND(!identifier, "Failed to find identifier.");

        Ref<GodotRpcModeParameterType> param_type = GodotRpcModeParameterType::instance();
        PANIC_COND(!param_type, "Failed to find parameter type.");

        String result;
        PANIC_COND(
            !param_type->get_godot_value_name(identifier->name, result),
            "Failed to find godot RPCMode for value.");

        return result;
    }

    String GodotRpcSyncArgument::_rpc_sync_lazy_get() const
    {
        Ref<Identifier> identifier = find_child<Identifier>();
        PANIC_COND(!identifier, "Failed to find identifier.");
        return identifier->name;
    }

    String GodotRpcTransferModeArgument::_transfer_mode_lazy_get() const
    {
        Ref<Identifier> identifier = find_child<Identifier>();
        PANIC_COND(!identifier, "Failed to find identifier.");

        Ref<GodotRpcTransferModeParameterType> param_type =
            GodotRpcTransferModeParameterType::instance();
        PANIC_COND(!param_type, "Failed to find parameter type.");

        String result;
        PANIC_COND(
            !param_type->get_godot_value_name(identifier->name, result),
            "Failed to find godot RPCMode for value.");
        return result;
    }

    int GodotRpcChannelArgument::_channel_lazy_get() const
    {
        const Ref<Literal> literal = find_chain<Literal, Arguments, Argument>();
        PANIC_COND(!literal, "Failed to find literal.");
        return string_to_int(literal->content);
    }

} // namespace GodotObjectCompiler
