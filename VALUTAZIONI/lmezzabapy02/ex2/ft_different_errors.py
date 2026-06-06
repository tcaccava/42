def garden_operations(operation_number: int) -> None:
    if operation_number == 0:
        int("abc")
    elif operation_number == 1:
        1 / 0
    elif operation_number == 2:
        open("/non/existent/file")
    elif operation_number == 3:
        "hello" + 42
    else:
        return


def test_error_types() -> None:
    print("=== Garden Error Types Demo ===")

    for i in range(5):
        print(f"Testing operation {i}...")

        try:
            garden_operations(i)
            print("Operation completed successfully")

        except ValueError as error:
            print(f"Caught ValueError: {error}")
        except ZeroDivisionError as error:
            print(f"Caught ZeroDivisionError: {error}")
        except FileNotFoundError as error:
            print(f"Caught FileNotFoundError: {error}")
        except TypeError as error:
            print(f"Caught TypeError: {error}")

    print("All error types tested successfully!")

    try:
        garden_operations(0)
    except (ValueError, TypeError) as error:
        print(f"Caught multiple types example: {error}")


if __name__ == "__main__":
    test_error_types()
