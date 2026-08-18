from sys import prefix, base_prefix, executable
from os.path import basename
from os import environ
from site import getsitepackages

if __name__ == "__main__":
    if prefix and base_prefix and prefix != base_prefix:
        print("MATRIX STATUS: Welcome to the construct\n")
        print(f"Current Python: {executable}")
        print("Virtual Environment:",
              f"{basename(environ.get('VIRTUAL_ENV', prefix))}")
        print(f"Environment Path: {environ.get('VIRTUAL_ENV', prefix)}\n")
        print("SUCCESS: You're in an isolated environment!")
        print("Safe to install packages without affecting")
        print("the global system.\n")
        print("Package installation path:")
        print(f"{getsitepackages()[0]}")
    else:
        print("MATRIX STATUS: You're still plugged in\n")
        print(f"Current Python: {executable}")
        print("Virtual Environment: None detected\n")
        print("WARNING: You're in the global environment!")
        print("The machines can see everything you install\n")
        print("To enter the construct, run:")
        print("python -m venv matrix_env")
        print("source matrix_env/bin/activate # On Unix")
        print("matrix_env\\Scripts\\activate # On Windows\n")
        print("Then run this program again.")
