GODOT_SIGNAL
============

Usage
~~~~~

.. code-block:: C++

    GODOT_SIGNAL();
    void signal_name(int p_param);

.. include:: descriptions/GODOT_SIGNAL.rst

You can also bind the signal with a different name by providing a string literal argument in the attribute.

.. code-block:: C++

    GODOT_SIGNAL("exposed_signal");
    void emit_internal_signal(int p_param);
