GODOT_PROPERTY
==============

Usage
~~~~~

.. code-block:: C++

    GODOT_PROPERTY();
    int some_property = 0;

.. include:: descriptions/GODOT_PROPERTY.rst

Binding custom or computed properties is done by providing the property as well as getter and setter names
within the attribute arguments.

.. code-block:: C++

    GODOT_PROPERTY("property", GetSet("get_property", "set_property"));
    int get_property() const;
    void set_property(const int& p_value);

You can also define the name of the property as a string literal when using auto generated getters and setters
if your property name should differ from the backing fields name.

.. code-block:: C++

    GODOT_PROPERTY("exposed_name");
    int internal_name = 0;

You can emit signals on property change by using additional generator options.

.. code-block:: C++

    GODOT_PROPERTY(EmitChanged()); // create signal emitted_changed()
    int emitted = 0;

    GODOT_PROPERTY(EmitChangedValue()); // create signal emitted_changed(int emitted)
    int emitted = 0;

    GODOT_PROPERTY(EmitChanged("changed")); // do not create signal, emit changed()
    int emitted = 0;

    GODOT_PROPERTY(EmitChangedValue("changed")); // do not create signal, emit changed(int value)
    int emitted = 0;


Available Parameter Types
~~~~~~~~~~~~~~~~~~~~~~~~~
.. include:: parameters/GOC_CustomPropertyGetSet.rst
.. include:: parameters/GOC_PropertyGetAccessSpecifier.rst
.. include:: parameters/GOC_PropertySetAccessSpecifier.rst
.. include:: parameters/GOC_PropertyGeneratorOptions.rst
.. include:: parameters/GOC_VariantType.rst
.. include:: parameters/GOC_PropertyHint.rst
.. include:: parameters/GOC_PropertyUsageFlags.rst
