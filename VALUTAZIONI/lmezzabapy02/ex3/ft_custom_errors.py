class GardenError(Exception):
    def __init__(self, message: str = "Unknown garden error") -> None:
        super().__init__(message)


class PlantError(GardenError):
    def __init__(self, message: str = "Unknown plant error") -> None:
        super().__init__(message)


class WaterError(GardenError):
    def __init__(self, message: str = "Unknown water error") -> None:
        super().__init__(message)


def raise_plant_error() -> None:
    raise PlantError("The tomato plant is wilting!")


def raise_water_error() -> None:
    raise WaterError("Not enough water in the tank!")


def test_custom_errors() -> None:
    print("=== Custom Garden Errors Demo ===")

    print("Testing PlantError...")
    try:
        raise_plant_error()
    except PlantError as error:
        print(f"Caught PlantError: {error}")

    print("Testing WaterError...")
    try:
        raise_water_error()
    except WaterError as error:
        print(f"Caught WaterError: {error}")

    print("Testing catching all garden errors...")
    for func in (raise_plant_error, raise_water_error):
        try:
            func()
        except GardenError as error:
            print(f"Caught GardenError: {error}")

    print("All custom error types work correctly!")


if __name__ == "__main__":
    test_custom_errors()
