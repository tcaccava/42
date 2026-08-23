#!/usr/bin/env python3
import sys


def io() -> None:
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0].split('/')[-1]} <file>")
        return

    file_name: str = sys.argv[1]

    print("=== Cyber Archives Recovery & Preservation ===")
    print(f"Accessing file '{file_name}'")

    file_handle = None
    content: str = ""
    try:
        file_handle = open(file_name, "r")
        content = file_handle.read()

        print("---")
        print(content, end="")
        if not content.endswith("\n"):
            print()
        print("---")

    except (FileNotFoundError, PermissionError, OSError) as e:
        print(f"Error opening file '{file_name}': {e}")
        return
    finally:
        if file_handle is not None:
            file_handle.close()
            print(f"File '{file_name}' closed.")

    print("Transform data:")
    print("---")

    lines: list = content.splitlines()
    transformed_content: str = ""

    for line in lines:
        print(f"{line}#")
        transformed_content += f"{line}#\n"

    print("---")

    output_file: str = input("Enter new file name (or empty): ").strip()

    if not output_file:
        print("Not saving data.")
    else:
        print(f"Saving data to '{output_file}'")
        out_handle = None
        try:
            out_handle = open(output_file, "w")
            out_handle.write(transformed_content)
            print(f"Data saved in file '{output_file}'.")
        except OSError as e:
            print(f"Error saving to file '{output_file}': {e}")
        finally:
            if out_handle is not None:
                out_handle.close()

