GOC_Virtual
^^^^^^^^^^^

This parameter type is used to set if this function is overwritable in scripts within the GODOT_FUNCTION macros body.

The following values are available:

+-----------------------+----------------------------------------------------------------------+
| Value                 | Description                                                          |
+=======================+======================================================================+
| NoVirtual             | This function will not be bound as a script virtual function         |
|                       | and can not be overwritten in scripts.                               |
+-----------------------+----------------------------------------------------------------------+
| ScriptVirtual         | This function will be bound as a script virtual function             |
|                       | and can be overwritten in scripts.                                   |
|                       | The bounds virtual name will be _<function_name>.                    |
|                       |                                                                      |
|                       | When called from within the extension this                           |
|                       | method can be used to call the script virtual function if it exists, |
|                       | otherwise this function will be called.                              |
+-----------------------+----------------------------------------------------------------------+
| ScriptVirtualRequired | This function will be bound as a script virtual function             |
|                       | and must be overwritten in scripts.                                  |
|                       | The bounds virtual name will be _<function_name>.                    |
|                       | When called from within the extension this                           |
|                       | method can be used to call the script virtual function               |
|                       | if it exists, otherwise this function will be called and             |
|                       | an error is displayed.                                               |
+-----------------------+----------------------------------------------------------------------+
