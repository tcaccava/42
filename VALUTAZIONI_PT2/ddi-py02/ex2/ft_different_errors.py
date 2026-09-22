#!/usr/bin/env python3

def garden_operations(operation_number: int) -> None:
    if operation_number == 0:
        int("abc")
        # generate ValueError
    elif operation_number == 1:
        0 / 0
        # generate ZeroDivisionError
    elif operation_number == 2:
        open("/non/existent/file")
        # generate FileNotFoundError
    elif operation_number == 3:
        "str" + 0
        # generate TypeError
    else:
        return


def test_error_types() -> None:
    print("=== Garden Error Types Demo ===")

    for i in range(0, 5, 1):
        print(f"Testing operation {i}...")
        try:
            garden_operations(i)
            print("Operation completed successfully!")
        except Exception as e:
            print(f'Caught {e.__class__.__name__}: {e}')
    print("\nAll error types tested successfully!")


if __name__ == '__main__':
    test_error_types()
