
def garden_operations(operation_number: int) -> None:
    match operation_number:
        case 0:
            int("abc")
        case 1:
            10 / 0
        case 2:
            open("/non/existent/file")
        case 3:
            print("ciao" + 3) 
        case 4:
            print("Operation completed successfully\n")


def test_error_types() -> None:
    numbers = [0, 1, 2, 3, 4]
    for num in numbers:
        try:
            print("Testing operation ", num, "...", sep="")
            garden_operations(num)
        except (ValueError, TypeError) as e:
            if num == 3:
                print("Caugh TypeError:", e)
            else:
                print("Caught ValueError:", e)
        except (ZeroDivisionError) as e:
            print("Caught ZeroDivisionError:", e)
        except (FileNotFoundError) as e:
            print("Caught FileNotFoundError:", e)
    print("All error types tested successfully!")


def main() -> None:
    print("=== Garden Error Types Demo ===")
    test_error_types()


if __name__ == "__main__":
    main()
