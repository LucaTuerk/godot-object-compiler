import os
import platform
import subprocess
from os import environ

from SCons.Environment import Environment
from SCons.Errors import BuildError
from SCons.Node import FS


def normalize_path(path):
    return os.path.abspath(str(path))


def normalize_paths(paths):
    result = [normalize_path(path) for path in paths]
    result = [valid for valid in result if valid is not None]
    return result


def in_generated_sources(sources, root_path, generated_path):
    root_norm = normalize_path(root_path)
    gen_norm = normalize_path(generated_path)
    result = [
        f"{gen_norm}/generated_register_types.cpp",
    ]

    if root_norm is None or gen_norm is None:
        return []

    for source in sources:
        normalized = normalize_path(source)
        if (
            normalized
            and normalized.startswith(root_norm)
            and normalized.endswith(".cpp")
        ):
            relative = normalized.removeprefix(root_norm)
            relative = relative.replace(".cpp", ".generated.cpp")
            result.append(f"{generated_path}{relative}")

    return result


def goc_build(target, source, env):
    source_dir = env["GOC_SOURCE_DIR"]
    build_dir = os.path.join(source_dir, "build")

    if not os.path.isdir(build_dir):
        os.mkdir(build_dir)

    configure = ["cmake", f"-B{build_dir}", f"-S{source_dir}"]
    build = ["cmake", "--build", build_dir]

    if platform.system() == "Windows":
        build.extend(["--config", "Release"])
    else:
        configure.append("-DCMAKE_BUILD_TYPE=Release")

    space = " "
    subprocess.run(space.join(configure), shell=True, env=environ.copy(), check=True)
    subprocess.run(space.join(build), shell=True, env=environ.copy(), check=True)


def find_goc(env: Environment):
    environment_var = environ.get("GOC_EXECUTABLE")
    if environment_var is not None:
        print(f"Using GOC executable: {environment_var}")
        return environment_var

    raise BuildError(
        "GOC executable not found. Set the GOC_EXECUTABLE environment variable to the path of the goc executable."
    )
