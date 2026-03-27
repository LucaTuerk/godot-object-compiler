import subprocess
from glob import glob
from pathlib import Path

files = []

files.extend(Path("src").rglob("*.cpp"))
files.extend(Path("src").rglob("*.h"))
files.extend(Path("tests").rglob("*.cpp"))
files.extend(Path("tests").rglob("*.h"))

for path in files:
    subprocess.run(["clang-format-18", "-i", path])
