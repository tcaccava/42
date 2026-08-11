class GardenError(Exception):
    def __init__(self, message: str = "Found an error in the garden!") -> None:
        super().__init__(message)


class PlantError(GardenError):
    def __init__(self, message: str = "The plant seems to be "
                 "getting really sick!") -> None:
        super().__init__(message)


class WaterError(GardenError):
    def __init__(self, message: str = "There's not enough "
                 "water in the tank!") -> None:
        super().__init__(message)


def plant_error_test() -> None:
    print("Testing Plant Error...")
    try:
        raise PlantError("The plant is wilting!")
    except PlantError as e:
        print(f"Plant error found: {e}")


def water_error_test() -> None:
    print("Testing Water Error...")
    try:
        raise WaterError("The water tank is empty.")
    except WaterError as e:
        print(f"Water error found: {e}")


def garden_error_check() -> None:
    print("Checking the whole garden for errors")
    try:
        raise PlantError("The plant is wilting!")
    except GardenError as e:
        print(f"Garden error found: {e}")
    try:
        raise WaterError("The water tank is empty.")
    except GardenError as e:
        print(f"Garden error found: {e}")


if __name__ == "__main__":
    print("=== Garden Error Checker ===")
    plant_error_test()
    water_error_test()
    garden_error_check()
    print("=== Every error checked ===")
