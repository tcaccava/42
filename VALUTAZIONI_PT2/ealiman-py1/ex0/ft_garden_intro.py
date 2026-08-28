#! /usr/bin/env python3
def start_program() -> None:
    print("=== Welcome to My Garden ===")


def plant() -> None:
    print("Plant: Rose")
    print("Height: 25 cm")
    print("Age: 30 days")


def end_program() -> None:
    print("\n")
    print("=== End of Program ===")


def main() -> None:
    start_program()
    plant()
    end_program()


if __name__ == "__main__":
    main()
