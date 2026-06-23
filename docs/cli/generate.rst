generate
========

The generate command is main program provided by godot-object-compiler, combining the TypeDB cache and source file
generation. When run successful, the provided generated folder will include all the necessary generated code for
the source files provided.

On the build system side, the generated folder must be added as an include, and the .cpp files within must be added
to the project sources.

godot-object-compiler adds generated .cpp and .h files for source files even if they do not
include any attributed code. This allows you to specify the byproducts of the generate command at build system configure
time from the list of the original sources.

.. include:: project.rst

**Help Text:**

.. include:: descriptions/generate.rst

.. toctree::
    :name: cli_clear

    generate_bindings
    generate_type_db