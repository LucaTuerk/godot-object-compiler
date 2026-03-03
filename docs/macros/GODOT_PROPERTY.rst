GODOT_PROPERTY
==============

Usage
~~~~~

.. code-block:: C++

    GODOT_PROPERTY();
    int some_property = 0;

.. include:: descriptions/GODOT_PROPERTY.rst

You can also bind custom or computed properties by providing the property as well as getter and setter names
within the attribute arguments.

.. code-block:: C++

    GODOT_PROPERTY(Name("property"), Get("get_property"), Set("set_property"));
    int get_property() const;
    void set_property(const int& p_value);

Available Parameter Types
~~~~~~~~~~~~~~~~~~~~~~~~~
.. include:: parameters/GOC_CustomPropertyBind.rst
.. include:: parameters/GOC_CustomPropertyGet.rst
.. include:: parameters/GOC_CustomPropertySet.rst
.. include:: parameters/GOC_PropertyGetAccessSpecifier.rst
.. include:: parameters/GOC_PropertySetAccessSpecifier.rst
.. include:: parameters/GOC_VariantType.rst
.. include:: parameters/GOC_PropertyHint.rst
.. include:: parameters/GOC_PropertyUsageFlags.rst
