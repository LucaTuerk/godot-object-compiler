
from autogoc_common import *
from SCons.Action import Action
from SCons.Environment import Environment
from SCons.Errors import BuildError


def find_godot_cpp_path(include_dirs):
    for dir in include_dirs:
        if "godot-cpp" in dir:
            return dir.split("godot-cpp")[0] + "godot-cpp"

    raise BuildError("Could no find godot-cpp in include paths.")


def create_goc_shared_library(env: Environment, lib_name: str, source, root_path: str):
    goc_path = find_goc(env)
    goc_dir = normalize_path(".goc")

    generated_dir = normalize_path(".goc/generated")
    cache_dir = normalize_path(".goc/cache")
    root_dir = normalize_path(root_path)
    sources = normalize_paths(source)
    current_includes = env.Dictionary("CPPPATH")
    include_dirs = normalize_paths(current_includes)
    generated_source = in_generated_sources(source, root_dir, generated_dir)
    generated_headers = [path.replace(".cpp", ".h") for path in generated_source]
    generated_headers.append(
        normalize_path(f"{generated_dir}/godot_object_compiler/macros.h")
    )
    comma = ","

    godot_cpp_include_dirs = []
    additional_include_dirs = []

    for include_dir in include_dirs:
        if root_dir == include_dir:
            continue
        if "godot-cpp" in include_dir:
            if "gdextension" in include_dir:
                continue
            godot_cpp_include_dirs.append(include_dir)
        else:
            additional_include_dirs.append(include_dir)
    godot_cpp_path = find_godot_cpp_path(include_dirs)

    run_action = Action(
        f"{goc_path} generate \
            -PT=GDExtension \
            -P={goc_dir} \
            -T={cache_dir} \
            -G={generated_dir} \
            -R={root_dir}\
            -I={comma.join(str(f) for f in additional_include_dirs)} \
            -S={comma.join(str(f) for f in sources)} \
            -E={godot_cpp_path}/gdextension/extension_api.json \
            -GPP={comma.join(str(f) for f in godot_cpp_include_dirs)}",
        cmdstr="Godot Object Compiler: Generating bindings",
    )

    run_goc = env.Command(generated_source, source=[], action=run_action)
    env.SideEffect(generated_headers, run_goc)
    env.AlwaysBuild(run_goc)
    env.NoCache(run_goc)  # goc uses its own cache
    env.Alias("goc_generated", generated_source)
    env.Depends(run_goc, goc_path)

    godot_cpp_lib = (
            godot_cpp_path + "/bin/libgodot-cpp" + env["suffix"] + env["LIBSUFFIX"]
    )

    env.Depends(run_goc, godot_cpp_lib)

    final_source = []
    final_source.extend(generated_source)
    final_source.extend(sources)
    env.Depends(source, generated_source)
    env.Depends(source, generated_headers)
    env.AppendUnique(CPPPATH=[generated_dir])

    return env.SharedLibrary(lib_name, source=final_source)
