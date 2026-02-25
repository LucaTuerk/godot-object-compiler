GOC_RpcMode
^^^^^^^^^^^

This parameter type is used to set the functions rpc mode within the GODOT_FUNCTION macros body.

The following values are available:

+-----------+-------------------------------------------------------------------------------------------------------------------+
| Value     | Description                                                                                                       |
+===========+===================================================================================================================+
| Disabled  | This function cannot be called via RPC in the high-level multiplayer api.                                         |
|           | This is the default value.                                                                                        |
+-----------+-------------------------------------------------------------------------------------------------------------------+
| AnyPeer   | When used in the high-level multiplayer api any peer can call this function without permission.                   |
+-----------+-------------------------------------------------------------------------------------------------------------------+
| Authority | When used in the high-level multiplayer api only the current authority of the target node can call this function. |
+-----------+-------------------------------------------------------------------------------------------------------------------+
