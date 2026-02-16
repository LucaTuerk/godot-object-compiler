import os
import subprocess

def goc_generate():
    goc = os.environ.get("GOC_EXECUTABLE")
    if goc == None:
        print("Failed to get goc executable.")
        return None

    return subprocess.run([goc, 'generate']).returncode == 0
