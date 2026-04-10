GODOT_CATEGORY
==============

Usage
~~~~~

.. code-block:: C++

    GODOT_CATEGORY("Category Name");

    GODOT_PROPERTY();
    int some_property = 0;

.. include:: descriptions/GODOT_CATEGORY.rst

.. WARNING::

    For godot versions that do not implement `issue 14274 <https://github.com/godotengine/godot-proposals/issues/14274>`_
    a property group will be bound instead of the property category. This is done to reduce the complexity of the generated code.
