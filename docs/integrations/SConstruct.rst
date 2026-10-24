SConstruct
==========

godot-object-compiler currently comes with SConstruct integration, which allows you to
mark a target, i.e. your GDExtension, for code generation.

Get the tools
~~~~~~~~~~~~~

You can dump the tools into a local folder using the following command.

.. code-block:: bash

    $ goc init tools <local folder path>

Afterwards you will find a *autogoc.py* file in that folder.

Setup
~~~~~

First include the tools script create_goc_shared_library function in your SConstruct file.

.. code-block:: Python

    from tools.autogoc import create_goc_shared_library


Then replace your GDExtensions SharedLibrary Builder, which looks like this
if you used the official template:

.. code-block:: Python

    library = env.SharedLibrary(
        "bin/{}/{}".format(env['platform'], lib_filename),
        source=sources
    )

with the following:

.. code-block:: Python

    library = create_goc_shared_library(
        env,
        "bin/{}/{}".format(env["platform"], lib_filename),
        source=sources,
        root_path="src",
    )


Finally ensure that the autogoc script can find a prebuild *goc* executable by setting the
**GOC_EXECUTABLE** environment variable, or alternatively clone godot-object-compiler
repository into the root of your project. godot-object-compiler will then be built
locally and used.

Run

.. code-block:: bash

    scons

with your preferred flags and godot-object-compiler should now generate your files.
