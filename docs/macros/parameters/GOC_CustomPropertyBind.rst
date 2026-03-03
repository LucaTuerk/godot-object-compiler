GOC_CustomPropertyBind
^^^^^^^^^^^^^^^^^^^^^^

This argument specified if the property generator generates the getters, setters and bindings based
on the field the attribute is applied to or a custom binding is applied.

The following values are available:

+----------+---------------------------------------------------------------------------------------------------------------------+
| Value    | Description                                                                                                         |
+==========+=====================================================================================================================+
| AutoBind | The generator creates getters, setters and property bindings based on the member fields it is applied to.           |
|          | This is the default value.                                                                                          |
+----------+---------------------------------------------------------------------------------------------------------------------+
| Name     | The generator binds a custom property named this with the getters and setters provided by GOC_CustomPropertyGet and |
|          | GOC_CustomPropertySet arguments.                                                                                    |
+----------+---------------------------------------------------------------------------------------------------------------------+
