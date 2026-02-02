import pathlib

def file_var_name(path: pathlib.Path):
    return str(path) \
        .replace('/', '_') \
        .replace('.', '_') \
        .replace('\\', '_') \
        .replace(':', '_')


def compile_folder(resources_path: pathlib.Path, target_path: pathlib.Path, resource_prefix: str,
                   resource_namespace_name: str):
    out = open(target_path, "wt")
    out.write('#pragma once\n')
    out.write('#include "library/core/resources.h"\n')

    out.write(f'namespace {resource_namespace_name} {{\n')
    for file_path in resources_path.rglob('*.txt'):
        print(file_path)
        out.write(f'constexpr char {file_var_name(file_path)}[] = {{\n\t')
        with open(file_path, 'rb') as file:
            count = 0
            while byte := file.read(1):
                out.write(f'0x{byte.hex()},')

                count += 1
                if (count % 16) == 0:
                    out.write(f'\n\t')
        out.write("'\\0'")
        out.write('};\n')

    out.write(f'static inline GodotObjectCompiler::Resources::ResourcePack Pack = {{\n')
    for file_path in resources_path.rglob('*.txt'):
        res_path = f'{resource_prefix}/{str(file_path.relative_to(resources_path))}'

        out.write(f'{{"{res_path}", &{file_var_name(file_path)}[0]}},\n')
    out.write('};')
    out.write('}\n')


compile_folder(pathlib.Path("./resources"), pathlib.Path("./application/compiled_resources/res.gen.h"), "res:/", "GOC_Resources")
