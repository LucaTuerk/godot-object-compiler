from pathlib import Path


def create_all_h(directory):
    print(directory)
    dirpath = Path(directory)

    includes = []
    has_all_h = False

    for file in dirpath.iterdir():
        if file.is_file() and file.suffix == ".h":
            if file.stem == "all":
                has_all_h = True
                continue
            includes.append(f'#include "{file.stem}.h"')

    if has_all_h:
        includes.sort()

        with open(dirpath / "all.h", "w") as all_h:
            all_h.write("#pragma once\n")
            for include in includes:
                all_h.write(f"{include}\n")

    for dir in dirpath.iterdir():
        if dir.is_dir():
            create_all_h(dir)


create_all_h(Path("./tests"))
create_all_h(Path("./src"))
