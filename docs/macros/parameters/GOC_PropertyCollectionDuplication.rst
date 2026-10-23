GOC_PropertyCollectionDuplication
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This parameter type is used to specify if godot collection types are duplicated before being returned in the generated getter.

The following values are available:

+-------------------+---------------------------------------------------------------------------------------------------------+
| Value             | Description                                                                                             |
+===================+=========================================================================================================+
| GetOriginal       | Return the original godot collection instance in the getter. This is the default value.                 |
+-------------------+---------------------------------------------------------------------------------------------------------+
| GetDuplicated     | Duplicate the godot collection property backing field before returning it in the generated getter.      |
+-------------------+---------------------------------------------------------------------------------------------------------+
| GetDuplicatedDeep | Duplicate the godot collection property backing field deep before returning it in the generated getter. |
+-------------------+---------------------------------------------------------------------------------------------------------+
