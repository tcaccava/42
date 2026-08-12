import sys
import typing

def ancient_text() -> None:
    if len(sys.argv) != 2:
        print("Usage: ft_ancient_text.py <file>\n")
        return

    filename: str = sys.argv[1]

    print("\n=== Cyber Archives Recovery ===")
    print(f"Accessing file '{filename}'\n")

    try:
        file: typing.TextIO = open(filename, "r")

        print("---")
        print(file.read(), end="")
        print("---")

        file.close()
        print(f"\nFile '{filename}' closed.\n")

    except Exception as e:
        print(f"Error opening file '{filename}': {e}\n")


if __name__ == "__main__":
    ancient_text()
