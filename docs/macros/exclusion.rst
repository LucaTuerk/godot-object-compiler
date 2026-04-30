Exclude Code
============

If some code within your project is causing issues for the parser or generator,
sections can always be excluded at the parser stage using the following comments.

.. code-block:: C++

    // goc off
    void some_excluded_function(); // this is excluded
    // goc on
    // everything after is included again

.. WARNING::

    Because the whole section is excluded before parsing, a wrong configuration by
    ,for example, missing the goc on comment can in turn lead to parser issues if the
    resulting source is not valid C++.