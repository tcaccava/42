from importlib.metadata import version
from typing import List, Dict


print("LOADING STATUS: Loading programs...\n")

print("Checking dependencies:")
missing: bool = False
try:
    import pandas as pd
    print(f"[OK] pandas ({version('pandas')}) - Data manipulation ready")
except (ModuleNotFoundError, ImportError):
    print("[MISSING] pandas - Data manipulation unavailable")
    missing = True

try:
    import numpy as np
    print(f"[OK] numpy ({version('numpy')}) - Numerical computation ready")
except (ModuleNotFoundError, ImportError):
    print("[MISSING] numpy - Numerical computation unavailable")
    missing = True

try:
    import matplotlib.pyplot as plt
    print(f"[OK] matplotlib ({version('matplotlib')}) - Visualization ready")
except (ModuleNotFoundError, ImportError):
    print("[MISSING] matplotlib - Visualization unavailable")
    missing = True


def comparison() -> None:
    print("\nRequired package versions:")
    print("\nPip3 versions:")
    try:
        with open("requirements.txt") as r:
            lines: List[str] = r.readlines()
            hash: Dict[str, str] = {}
            for x in lines:
                splitted: List[str] = x.strip("\n").split("==")
                hash[splitted[0]] = splitted[1]
        for k in ["pandas", "numpy", "matplotlib"]:
            try:
                print(f"{k} version: {hash[k]}")
            except KeyError:
                pass
        r.close()
    except (FileNotFoundError, PermissionError):
        print("Cannot access requirements.txt")
    lines = []
    hash = {}
    print("\nPoetry versions:")
    try:
        with open("pyproject.toml") as r:
            lines = r.readlines()
            i: int = 0
            while lines[i] != "dependencies = [\n":
                i += 1
            i += 1
            while lines[i] != "]\n":
                splitted = lines[i].strip(" \",\n)").split(" (")
                hash[splitted[0]] = splitted[1]
                i += 1
            for k in ["pandas", "numpy", "matplotlib"]:
                try:
                    print(f"{k} version: {hash[k]}")
                except KeyError:
                    pass
        r.close()
    except (FileNotFoundError, PermissionError):
        print("Cannot access pyproject.toml")


def main() -> None:
    if missing is True:
        print("\nPip installation: bash the command",
              "'pip install -r requirements.txt'")
        print("Poetry installation: bash the command 'poetry install'")
        return
    steps: int = 1000
    origin: int = 0
    step_set = np.array([-1, 1])
    random_steps = np.random.choice(step_set, size=steps)
    print("\nAnalyzing Matrix data...\nProcessing 1000 data points...")
    path = np.concatenate([[origin], np.cumsum(random_steps)])
    df = pd.DataFrame(path, columns=["Position"])
    print("The data describe a 1D Brownian motion")
    print(f"Average position: {round(df['Position'].mean(), 3)}",
          f"+/- {round(df['Position'].std(), 3)}")
    print("Generating visualization...\n")
    plt.figure(figsize=(10, 4))
    plt.plot(df, color='#2ca02c', linewidth=1)
    plt.title("1D Random Walk (Symmetric)")
    plt.xlabel("Steps")
    plt.ylabel("Position")
    plt.grid(True, alpha=0.3)
    plt.savefig("matrix_analysis.png")
    print("Analysis complete!\nResults saved to: matrix_analysis.png")


if __name__ == "__main__":
    main()
    comparison()
