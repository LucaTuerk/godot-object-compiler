GOC_CustomPropertyGet
^^^^^^^^^^^^^^^^^^^^^

This argument specified the property getter for custom bound properties.

The following values are available:

+---------+-----------------------------------------------------------------------------------------------------------+
| Value   | Description                                                                                               |
+=========+===========================================================================================================+
| AutoGet | The generator creates getters, setters and property bindings based on the member fields it is applied to. |
|         | This is the default value.                                                                                |
+---------+-----------------------------------------------------------------------------------------------------------+
| Get     | The generator uses the member function identified by this name as the properties getter.                  |
+---------+-----------------------------------------------------------------------------------------------------------+
