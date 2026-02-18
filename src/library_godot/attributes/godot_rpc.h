/**************************************************************************/
/* godot_rpc.h                                                            */
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
#include "library/generator/attribute_parameter_type.h"
#include "library/tree/syntax/function.h"

namespace GodotObjectCompiler {

  class GodotRpcModeArgument : public Argument {
    NODE_TYPE(GodotRpcModeArgument);

    LAZY(GodotRpcModeArgument, String, rpc_mode);
    LAZY(GodotRpcModeArgument, String, godot_rpc_mode);
  };

  class GodotRpcModeParameterType : public IAttributeParameterType {
    PARAM_TYPE(GodotRpcModeParameterType, GodotRpcModeArgument);

   public:

    String get_return_type() override;
    Vector<String> get_value_names() override;
    Vector<Argument> get_arguments() override;
    bool get_godot_value_name(const String& p_value_name, String& r_godot_value_name);

    LAZY_MUT(GodotRpcModeParameterType, Vector<String>, value_names);
    Dictionary<String, String> exposed_name_to_godot_value_name;
  };

  class GodotRpcSyncArgument : public Argument {
    NODE_TYPE(GodotRpcSyncArgument);

    LAZY(GodotRpcSyncArgument, String, rpc_sync);
  };

  class GodotRpcSyncParameterType : public IAttributeParameterType {
    PARAM_TYPE(GodotRpcSyncParameterType, GodotRpcSyncArgument);

   public:

    String get_return_type() override;
    Vector<String> get_value_names() override;
    Vector<Argument> get_arguments() override;
  };

  class GodotRpcTransferModeArgument : public Argument {
    NODE_TYPE(GodotRpcTransferModeArgument);

    LAZY(GodotRpcTransferModeArgument, String, transfer_mode);
  };

  class GodotRpcTransferModeParameterType : public IAttributeParameterType {
    PARAM_TYPE(GodotRpcTransferModeParameterType, GodotRpcTransferModeArgument);

   public:

    String get_return_type() override;
    Vector<String> get_value_names() override;
    Vector<Argument> get_arguments() override;

    bool get_godot_value_name(const String& p_value_name, String& r_godot_value_name);

    LAZY_MUT(GodotRpcTransferModeParameterType, Vector<String>, value_names);
    Dictionary<String, String> exposed_name_to_godot_value_name;
  };

  class GodotRpcChannelArgument : public Argument {
    NODE_TYPE(GodotRpcChannelArgument);

    LAZY(GodotRpcChannelArgument, int, channel);
  };

  class GodotRpcChannelParameterType : public IAttributeParameterType {
    PARAM_TYPE(GodotRpcChannelParameterType, GodotRpcChannelArgument);

   public:

    String get_return_type() override;
    Vector<String> get_value_names() override;
    Vector<Argument> get_arguments() override;
  };

}
