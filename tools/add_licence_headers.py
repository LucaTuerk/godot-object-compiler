import pathlib


def add_licence_headers(path: pathlib.Path):
    header_template = ''
    with open('licence_header.txt', 'r') as header:
        header_template = str(header.read())

    if len(header_template) == 0:
        return

    for file in list(path.rglob('**/*.h')) + list(path.rglob('**/*.cpp')):
        write = ''
        if file.name == 'all.h':
            continue
            
        with open(file, 'r') as current:
            content = current.read()
            if "This file is part of Godot Object Compiler" in content:
                continue

            write += header_template.replace("FILENAME                                                              ",
                                             f'{str(file.name):<70}')
            write += '\n'
            write += content

        with open(file, 'w') as current:
            current.write(write)
