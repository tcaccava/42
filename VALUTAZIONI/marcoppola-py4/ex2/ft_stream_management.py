import sys
import typing


def stream_management() -> None:
    if len(sys.argv) != 2:
        print("Usage: ft_stream_management.py <file>\n")
        return

    filename = sys.argv[1]

    print("\n=== Cyber Archives Recovery & Preservation ===")
    print(f"Accessing file '{filename}'\n")

    try:
        file: typing.TextIO = open(filename, "r")

        content = file.read()
        file.close()

        print("\n---")
        print(content, end="")
        print("---\n")

        print(f"File '{filename}' closed.\n")

        print("Transform data:")
        print("---")

        lines = content.splitlines()
        new_lines = []

        for line in lines:
            new_lines.append(line + "#")

        new_content = "\n".join(new_lines)

        print(new_content)
        print("---\n")

        print("Enter new file name (or empty): ", end="")
        sys.stdout.flush()

        new_filename = sys.stdin.readline().strip()

        if new_filename == "":
            print("Not saving data.\n")
        else:
            print(f"Saving data to '{new_filename}'")

            try:
                output: typing.TextIO = open(new_filename, "w")
                output.write(new_content)
                output.close()

                print(f"\nData saved in file '{new_filename}'.\n")

            except Exception as e:
                print(
                    f"[STDERR] Error opening file '{new_filename}': {e}\n",
                    file=sys.stderr
                )
                print("\nData not saved.")

    except Exception as e:
        print(
            f"[STDERR] Error opening file '{filename}': {e}\n",
            file=sys.stderr
        )


if __name__ == "__main__":
    stream_management()
