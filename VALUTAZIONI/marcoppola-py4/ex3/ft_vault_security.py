def secure_archive(
        filename: str,
        action: str = 'read',
        content: str = ""
        ) -> tuple[bool, str]:
    try:
        if action == "read":
            with open(filename, "r") as file:
                data = file.read()
            isclosed: bool = file.closed
            print(f" Il file '{filename}' è chiuso? {isclosed}")
            
            return True, data
        elif action == "write":
            with open(filename, "w") as file:
                file.write(content)
            return True, "Content successfully written to file."

        else:
            return False, "Invalid action"
    
    except OSError as error:
        return False, str(error)

if __name__ == "__main__":
    print("\n=== Cyber Archivist Security ===\n")

    print("Using 'secure_archive' to read from a nonexistent file:")
    print(secure_archive("/not/existing/file"))
    
    print("\nUsing 'secure_archive' to read from a regular file:")
    filename = open("archive.txt", "r")
    print(secure_archive("archive.txt"))
    print(f"File is closed: {filename.closed}")
    
    print("\nUsing 'secure_archive' to write previous content to a new file:")
    content = secure_archive("archive.txt")[1]
    print(secure_archive("new_archive.txt", "write", content))
