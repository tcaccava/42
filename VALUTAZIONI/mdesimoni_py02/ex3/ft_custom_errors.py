#!/bin/python3
class GardenError(Exception):
    def __init__(self, message: str = "Unknown Garden error") -> None:
        super().__init__(message)


class PlantError(GardenError):
    def __init__(self, message: str = "Unknown plant error") -> None:
        super().__init__(message)


class WaterError(GardenError):
    def __init__(self, message: str = "Unknown water error") -> None:
        super().__init__(message)


def garden_operations(operation_number: int) -> None:
    if operation_number == 0:
        raise GardenError()
    elif operation_number == 1:
        raise PlantError("The tomato plant is wilting!")
    elif operation_number == 2:
        raise WaterError("Not enough water in the tank!")
    else:
        return


def test_error_types() -> None:
    print("=== Custom Garden Errors Demo ===")
    try:
        print("\nTesting PlantError...")
        garden_operations(1)
    except PlantError as ex:
        print("Caught PlantError:", ex)

    try:
        print("\nTesting WaterError...")
        garden_operations(2)
    except WaterError as ex:
        print("Caught WaterError:", ex)

    print("\nTesting catching all garden errors...")
    operations: list[int] = [1, 2]
    for op in operations:
        try:
            garden_operations(op)
        except GardenError as ex:
            print("Caught GardenError:", ex)

    print("\nAll custom error types work correctly!")


if __name__ == "__main__":
    test_error_types()
