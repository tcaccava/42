#!/usr/bin/env python3
import os
import subprocess

def main():
    run = 5
    if os.path.basename(__file__) not in ("Sully.py", "Sully"):
        run -= 1
    if run < 0:
        return
    filename = f"Sully_{run}.py"
    code = '#!/usr/bin/env python3\nimport os\nimport subprocess\n\ndef main():\n    run = %d\n    if os.path.basename(__file__) not in ("Sully.py", "Sully"):\n        run -= 1\n    if run < 0:\n        return\n    filename = f"Sully_{run}.py"\n    code = %r\n    with open(filename, "w") as f:\n        f.write(code %% (run, code))\n    os.chmod(filename, 0o755)\n    subprocess.run(["python3", filename])\n\nif __name__ == "__main__":\n    main()\n'
    with open(filename, "w") as f:
        f.write(code % (run, code))
    os.chmod(filename, 0o755)
    subprocess.run(["python3", filename])

if __name__ == "__main__":
    main()
