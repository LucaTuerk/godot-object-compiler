GODOT_FUNCTION
==============

Usage
~~~~~

.. code-block:: C++

    GODOT_FUNCTION();
    void function_name(int p_param);

.. include:: descriptions/GODOT_FUNCTION.rst

You can also bind the function with a different name by providing a string literal argument in the attribute.

.. code-block:: C++

    GODOT_FUNCTION("exposed_name");
    void internal_name(int p_param);

Available Parameter Types
~~~~~~~~~~~~~~~~~~~~~~~~~

.. include:: parameters/GOC_Virtual.rst
.. include:: parameters/GOC_RpcMode.rst
.. include:: parameters/GOC_RpcSync.rst
.. include:: parameters/GOC_TransferMode.rst
.. include:: parameters/GOC_RpcChannel.rst
