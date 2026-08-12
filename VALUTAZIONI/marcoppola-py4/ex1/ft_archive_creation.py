import sys
import typing


def archive_creation() -> None:
    if len(sys.argv) != 2:
        print("Usage: ft_archive_creation.py <file>\n")
        return

    filename: str = sys.argv[1]

    print("=== Cyber Archives Recovery & Preservation ===")
    print(f"Accessing file '{filename}'\n")

    try:
        file: typing.TextIO = open(filename, "r")

        content = file.read()
        file.close()

        print("\n---")
        print(content, end="")
        print("---\n")

        print(f"File '{filename}' closed.\n")

        print("Transform data: ")
        print("---\n")

        lines = content.splitlines()
        new_lines = []

        for line in lines:
            new_lines.append(line + "#")

        new_content = "\n".join(new_lines)

        print(new_content)
        print("---")

        new_filename = input("\nEnter new file name (or empty): ")

        if new_filename == "":
            print("Not saving data.")
        else:
            print(f"Saving data to '{new_filename}'")

            output: typing.TextIO = open(new_filename, "w")
            output.write(new_content)
            output.close()

            print(f"\nData saved in file '{new_filename}'.")

    except Exception as e:
        print(f"\nError opening file '{filename}': {e}\n")


if __name__ == "__main__":
    archive_creation()
