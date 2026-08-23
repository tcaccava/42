#!/usr/bin/env python3


def secure_archive(
        file_name: str, action: str = "read", content: str = "") -> tuple:
    try:
        if action == "write":
            with open(file_name, "w") as file_handle:
                file_handle.write(content)
            return (True, "Content successfully written to file")

        with open(file_name, "r") as file_handle:
            data = file_handle.read()
        return (True, data)

    except (FileNotFoundError, PermissionError, OSError) as e:
        return (False, str(e))


def main() -> None:
    print("=== Cyber Archives Security ===")

    print("Using 'secure_archive' to read from a nonexistent file:")
    res1 = secure_archive("/not/existing/file", "read")
    print(res1)

    print("Using 'secure_archive' to read from an inaccessible file:")
    res2 = secure_archive("/etc/shadow", "read")
    print(res2)

    fragment_file = "ancient_fragment.txt"
    sample_text = (
        "[FRAGMENT 001] Digital preservation protocols established 2087\n"
        "[FRAGMENT 002] Knowledge must survive the entropy wars\n"
        "[FRAGMENT 003] Every byte saved is a victory against oblivion\n"
    )
    setup_ok, setup_msg = secure_archive(fragment_file, "write", sample_text)

    print("Using 'secure_archive' to read from a regular file:")
    res3 = secure_archive(fragment_file, "read")
    print(res3)

    print("Using 'secure_archive' to write previous content to a new file:")
    res4 = secure_archive("vault_backup.txt", "write", res3[1])
    print(res4)


if __name__ == "__main__":
    main()
