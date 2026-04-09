import subprocess
from pathlib import Path


def format_project():
    print("Formatting.")
    files = []

    files.extend(Path("src").rglob("*.cpp"))
    files.extend(Path("src").rglob("*.h"))
    files.extend(Path("tests").rglob("*.cpp"))
    files.extend(Path("tests").rglob("*.h"))

    for path in files:
        subprocess.run(["clang-format-18", "-i", path])


if __name__ == "__main__":
    format_project()
