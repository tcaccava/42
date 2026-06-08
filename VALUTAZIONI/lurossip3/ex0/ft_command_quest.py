#!./usr/bin/env python
import sys

if __name__ == "__main__":
    print("=== Command Quest ===")
    end = len(sys.argv)
    i = 0
    while i < end:
        if i == 0:
            print(f"Program name: {sys.argv[i]}")
            if len(sys.argv) == 1:
                print("No arguments provided!")
            else:
                print(f"Arguments received: {len(sys.argv) - 1}")
            i += 1
        else:
            print(f"Argument {i}: {sys.argv[i]}")
            i += 1
    print(f"Total arguments: {len(sys.argv)}")
