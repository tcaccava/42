# generate_test_files.py

def create_file(filename, content):
    with open(filename, "w") as f:
        f.write(content)
    print(f"Generated: {filename}")

def main():
    # 1. File valido
    create_file("valid.txt", "This is a valid file.")

    # 2. File vuoto
    create_file("empty.txt", "")

    # 3. Script con errore di sintassi
    create_file("syntax_error.py", "def bad_function()\n    print('missing colon')")

    # 4. Script con loop infinito
    create_file("infinite_loop.py", "while True:\n    pass")

    # 5. File con caratteri binari (non testuale)
    with open("binary.dat", "wb") as f:
        f.write(bytes([0x00, 0xFF, 0x7F, 0x20, 0x00, 0x10]))
    print("Generated: binary.dat")

    # 6. File molto grande (10 MB)
    with open("large.txt", "w") as f:
        f.write("A" * 10 * 1024 * 1024)
    print("Generated: large.txt (10MB)")

    # 7. File con encoding non UTF-8 (es. latin1)
    with open("latin1.txt", "w", encoding="latin1") as f:
        f.write("Café crème élève déjà été")
    print("Generated: latin1.txt")

if __name__ == "__main__":
    main()
