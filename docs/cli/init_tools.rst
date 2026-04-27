init tools
==========

Copies the build-system integration scripts bundled with GOC into a local
directory. These are the helpers consumed by the integrations described in
:doc:`../integrations/index` (currently CMake).

Usage
-----

.. code-block:: bash

    $ goc init tools <directory>

Arguments
---------

``<directory>``
    Required. Target directory the bundled files are copied into. Created
    recursively if it does not already exist. Overwrites existing files.

