GOC_PropertyGeneratorOptions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Additional options.

The following values are available:

+---------------------------------+----------------------------------------------------------------------------------------------------------+
| Value                           | Description                                                                                              |
+=================================+==========================================================================================================+
| PropertyGeneratorOptionsDefault | No additional generator options. This is the default value.                                              |
+---------------------------------+----------------------------------------------------------------------------------------------------------+
| EmitChanged                     | Emit signal with no arguments identified by the provided name when this property is changed.             |
|                                 | If no name is provided, a signal named "<property_name>_changed" is bound.                               |
+---------------------------------+----------------------------------------------------------------------------------------------------------+
| EmitChangedValue                | Emit signal with the properties new value identified by the provided name when this property is changed. |
|                                 | If no name is provided, a signal named "<property_name>_changed" is bound.                               |
+---------------------------------+----------------------------------------------------------------------------------------------------------+
