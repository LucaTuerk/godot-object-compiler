GOC_RpcSync
^^^^^^^^^^^

This parameter type is used to set the functions rpc sync behavior within the GODOT_FUNCTION macros body.

The following values are available:

+------------+----------------------------------------------------------------------------------------+
| Value      | Description                                                                            |
+============+========================================================================================+
| CallRemote | When called via RPC, this function will only be called on the remote peer.             |
+------------+----------------------------------------------------------------------------------------+
| CallLocal  | When called via RPC, this function will be called on both the remote peer and locally. |
+------------+----------------------------------------------------------------------------------------+
