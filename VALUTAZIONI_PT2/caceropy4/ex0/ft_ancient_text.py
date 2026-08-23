#!/usr/bin/env python3
import sys


def main() -> None:
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0].split('/')[-1]} <file>")
        return

    file_name: str = sys.argv[1]

    print("=== Cyber Archives Recovery ===")
    print(f"Accessing file '{file_name}'")

    file_handle = None
    try:
        file_handle = open(file_name, "r")
        content: str = file_handle.read()

        print("---")
        print(content, end="")
        if not content.endswith("\n"):
            print()
        print("---")

    except (FileNotFoundError, PermissionError, OSError) as e:
        print(f"Error opening file '{file_name}': {e}")

    finally:
        if file_handle is not None:
            file_handle.close()
            print(f"File '{file_name}' closed.")


if __name__ == "__main__":
    main()
