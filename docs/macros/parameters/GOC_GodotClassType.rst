GOC_GodotClassType
^^^^^^^^^^^^^^^^^^

This parameter type is used to set the classes type in the GODOT_CLASS macro parameters.

The following values are available:

+----------------+-------------------------------------------------------------------------------------------------------+
| Value          | Description                                                                                           |
+================+=======================================================================================================+
| RegisterCustom | Do not register this class. Custom registration code needs to be added for this class to be available |
|                | in engine.                                                                                            |
|                |                                                                                                       |
+----------------+-------------------------------------------------------------------------------------------------------+
| GodotClass     | Register this class using the GDREGISTER_CLASS macro. This is the default value.                      |
|                |                                                                                                       |
+----------------+-------------------------------------------------------------------------------------------------------+
| VirtualClass   | Register this class using the GDREGISTER_VIRTUAL_CLASS macro.                                         |
|                |                                                                                                       |
+----------------+-------------------------------------------------------------------------------------------------------+
| AbstractClass  | Register this class using the GDREGISTER_ABSTRACT_CLASS macro.                                        |
|                |                                                                                                       |
+----------------+-------------------------------------------------------------------------------------------------------+
| InternalClass  | Register this class using the GDREGISTER_INTERNAL_CLASS macro.                                        |
|                |                                                                                                       |
+----------------+-------------------------------------------------------------------------------------------------------+
| RuntimeClass   | Register this class using the GDREGISTER_RUNTIME_CLASS macro.                                         |
|                |                                                                                                       |
+----------------+-------------------------------------------------------------------------------------------------------+
