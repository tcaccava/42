#!/bin/python3

def garden_operations(operation_number: int) -> None:
    if operation_number == 0:
        int("abc")
    elif operation_number == 1:
        10 / 0
    elif operation_number == 2:
        open("/non/existent/file")
    elif operation_number == 3:
        "str" + 42
    else:
        return


def test_error_types() -> None:
    operations: list[int] = [0, 1, 2, 3, 4]
    print("=== Garden Error Types Demo ===")
    for op in operations:
        print(f"Testing operation {op}...")
        try:
            garden_operations(op)
            print("Operation completed successfully")
        except ValueError as ex:
            print("Caught ValueError:", ex)
        except ZeroDivisionError as ex:
            print("Caught ZeroDivisionError:", ex)
        except FileNotFoundError as ex:
            print("Caught FileNotFoundError:", ex)
        except TypeError as ex:
            print("Caught TypeError:", ex)
    print("\nAll error types tested successfully!")


if __name__ == "__main__":
    test_error_types()
