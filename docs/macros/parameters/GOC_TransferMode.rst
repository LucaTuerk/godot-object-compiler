GOC_TransferMode
^^^^^^^^^^^^^^^^

This parameter type is used to set the functions rpc transfer mode within the GODOT_FUNCTION macros body.

The following values are available:

+-------------------+------------------------------------------------------------------------------------------------------------------------------------------------------+
| Value             | Description                                                                                                                                          |
+===================+======================================================================================================================================================+
| Unreliable        | Packets are not acknowledged, no resend attempts are made for lost packets. Packets may arrive in any order.                                         |
|                   | Potentially faster than "UnreliableOrdered".                                                                                                         |
|                   | Use for non-critical data, and always consider whether the order matters.                                                                            |
+-------------------+------------------------------------------------------------------------------------------------------------------------------------------------------+
| UnreliableOrdered | Packets are not acknowledged, no resend attempts are made for lost packets. Packets are received in the order they were sent in.                     |
|                   | Potentially faster than "Reliable".                                                                                                                  |
|                   | Use for non-critical data or data that would be outdated if received late due to resend attempt(s) anyway, for example movement and positional data. |
+-------------------+------------------------------------------------------------------------------------------------------------------------------------------------------+
| Reliable          | Packets must be received and resend attempts should be made until the packets are acknowledged.                                                      |
|                   | Packets must be received in the order they were sent in. Most reliable transfer mode, but potentially the slowest due to the overhead.               |
|                   | Use for critical data that must be transmitted and arrive in order, for example an ability being triggered or a chat message.                        |
|                   | Consider carefully if the information really is critical, and use sparingly.                                                                         |
+-------------------+------------------------------------------------------------------------------------------------------------------------------------------------------+
