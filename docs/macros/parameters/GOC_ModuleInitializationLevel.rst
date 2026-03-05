GOC_ModuleInitializationLevel
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

This parameter type is used to set this class initialization level within the GODOT_CLASS macros parameters.

The following values are available:

+--------------+-----------------------------------------------------------------------------------------------+
| Value        | Description                                                                                   |
+==============+===============================================================================================+
| LevelCore    | Register this class in the MODULE_INITIALIZATION_LEVEL_CORE step.                             |
|              |                                                                                               |
+--------------+-----------------------------------------------------------------------------------------------+
| LevelServers | Register this class in the MODULE_INITIALIZATION_LEVEL_SERVERS step.                          |
|              |                                                                                               |
+--------------+-----------------------------------------------------------------------------------------------+
| LevelScene   | Register this class in the MODULE_INITIALIZATION_LEVEL_SCENE step. This is the default value. |
|              |                                                                                               |
+--------------+-----------------------------------------------------------------------------------------------+
| LevelEditor  | Register this class in the MODULE_INITIALIZATION_LEVEL_EDITOR step.                           |
|              |                                                                                               |
+--------------+-----------------------------------------------------------------------------------------------+
| LevelMax     | This value is invalid and will be removed in future version.                                  |
|              |                                                                                               |
+--------------+-----------------------------------------------------------------------------------------------+
