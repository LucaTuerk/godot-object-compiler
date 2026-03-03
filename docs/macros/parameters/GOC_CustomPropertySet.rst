GOC_CustomPropertySet
^^^^^^^^^^^^^^^^^^^^^

This argument specified the property setter for custom bound properties.

The following values are available:

+---------+-----------------------------------------------------------------------------------------------------------+
| Value   | Description                                                                                               |
+=========+===========================================================================================================+
| AutoSet | The generator creates getters, setters and property bindings based on the member fields it is applied to. |
|         | This is the default value.                                                                                |
+---------+-----------------------------------------------------------------------------------------------------------+
| Set     | The generator uses the member function identified by this name as the properties setter.                  |
+---------+-----------------------------------------------------------------------------------------------------------+
