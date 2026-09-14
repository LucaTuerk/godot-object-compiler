from os import environ

from SCons.Action import Action
from SCons.Environment import Environment


def find_goc(env: Environment):
    environment_var = environ.get("GOC_EXECUTABLE")
    if environment_var is not None:
        print(f"Using GOC executable: {environment_var}")
        return environment_var

    raise Exception(
        "GOC executable not found. Set the GOC_EXECUTABLE environment variable to the path of the goc executable."
    )


def autogoc(env: Environment, module_env: Environment):
    goc_path = find_goc(env)
    godot_root = module_env.Dir("#").abspath
    root = module_env.Dir(".").abspath
    generated_dir = module_env.Dir("#.goc/generated")
    module_name = module_env.Dir(".").dirname.removesuffix("/").removesuffix("\\")

    include_dirs = [
        module_env.Dir("#core").abspath,
        module_env.Dir("#scene/main").abspath,
    ]

    sources = [s.abspath for s in module_env.FindSourceFiles(".") if str(s).endswith(".cpp")]

    generated_sources = [
        str(generated_dir) + str(s).replace(godot_root, "").replace(".cpp", ".generated.cpp")
        for s in sources
        if str(s).endswith(".cpp")
    ]
    generated_sources.append(str(generated_dir) + "/" + root.replace(godot_root, "") + "/generated_register_types.cpp")

    includes = [module_env.Dir(p).abspath for p in env.Dictionary("CPPPATH")]

    module_env.add_source_files(env.modules_sources, generated_sources)
    module_env.Prepend(CPPPATH=[generated_dir, root])

    run_action = Action(
        f"{goc_path} generate \
            -PT=Module\
            -N={module_name}\
            -R={root}\
            -GR={godot_root}\
            -S={','.join(str(s) for s in sources)}\
            -G={generated_dir}\
            -I={','.join(p for p in includes)}\
            -TI={','.join(str(i) for i in include_dirs)}",
        cmdstr=f'Godot Object Compiler: Generating bindings for module {module_name}',
    )
    run_goc = env.Command(generated_sources, [], run_action)
    env.AlwaysBuild(run_goc)
    env.Depends(sources, run_goc)
