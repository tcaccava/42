def garden_operation(operation_number: int) -> None:
    if operation_number == 0:
        int("abc")
    elif operation_number == 1:
        10 / 0
    elif operation_number == 2:
        open("/snow/in/summer")
    elif operation_number == 3:
        10 + "the forgotten string"
    else:
        return


def test_error_types() -> None:
    try:
        garden_operation(0)
    except ValueError:
        print("The value couldn't be converted into an integer.")
    try:
        garden_operation(1)
    except ZeroDivisionError:
        print("Numbers can't be divided by 0")
    try:
        garden_operation(2)
    except FileNotFoundError:
        print("File couldn't be found.")
    try:
        garden_operation(3)
    except TypeError:
        print("The operation includes different types of values.")
    print("Testing operation 4...")
    garden_operation(4)
    print("Operation completed!\n")
    print("All error types tested without any crash!")


if __name__ == "__main__":
    test_error_types()
