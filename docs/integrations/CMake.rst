CMake
=====

The Godot Object Compiler currently comes with CMake integration, which allows you to
mark a target, i.e. your GDExtension, for code generation.

Get the tools
~~~~~~~~~~~~~

You can dump the tools into a local folder using the following command.

.. code-block:: bash

    $ goc init tools <local folder path>

Afterwards you will find a *autogoc.cmake* file in that folder.

Setup
~~~~~

First include the tool file in your GDExtensions *CMakeLists.txt*:

.. code-block:: CMake

    include(tools/autogoc.cmake)

Next mark your GDExtension target for code generation using the *target_autogoc* function,
providing the target identifier and the root source folder.

.. code-block:: CMake

    target_autogoc(${LIBNAME} ${CMAKE_CURRENT_LIST_DIR}/src)

The source folder is used to determine the relative paths of your source files and subsequently
the hirarchy of the generated files. The actual parsed files are parsed from the *SOURCES*
variable of the target.

Finally ensure that the autogoc script can find a prebuild *goc* executable by setting the
**GOC_EXECUTABLE** environment variable, or alternatively clone the godot-object-compiler
repository into your project and add it to your *CMakeLists.txt* using:

.. code-block:: CMake

    add_subdirectory(godot-object-compiler)

The script will then use the locally checked out version and build the executable when needed.

.. WARNING::

    The Godot Object Compiler currently requires absolute paths.
    Ensure that your targets sources and include directories are absolute,
    f.e. by prefacing *${CMAKE_CURRENT_LIST_DIR}*

    .. code-block:: CMake

        target_sources(${LIBNAME}
            PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/src/example_node.cpp
            ${CMAKE_CURRENT_LIST_DIR}/src/register_types.cpp
        )

        target_include_directories(${LIBNAME}
            PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/src
        )

    This is WIP.
