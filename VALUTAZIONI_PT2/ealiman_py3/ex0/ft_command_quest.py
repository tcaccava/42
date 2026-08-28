
import sys


def command_quest() -> None:
    print("Program name:", sys.argv[0])
    if len(sys.argv[1:]) >= 1:
        print("Arguments received:", len(sys.argv[1:]))
        i = 0
        while i <= len(sys.argv[1:]):
            print("Argument ", i, ": ", sys.argv[i], sep="")
            i += 1
    else:
        print("No arguments provided!")
    print("Total arguments:", len(sys.argv))


def main() -> None:
    print("=== Command Quest ===")
    command_quest()


if __name__ == "__main__":
    main()
