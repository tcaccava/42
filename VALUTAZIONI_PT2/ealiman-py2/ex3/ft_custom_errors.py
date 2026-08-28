
class GardenError(Exception):
    def __init__(self, message: str | None = None) -> None:
        if (message is None):
            message = "Unknow garden error"
        super().__init__(message)


class PlantError(GardenError):
    def __init__(self, message: str | None = None) -> None:
        if (message is None):
            message = "Unknow plant error"
        super().__init__(message)


class WaterError(GardenError):
    def __init__(self, message: str | None = None) -> None:
        if (message is None):
            message = "Unknow water error"
        super().__init__(message)


def plant_health(umidity: int) -> int:
    if (umidity < 60):
        raise PlantError("The tomato plant is wilting!")
    return umidity


def water_tank(water_level: int) -> int:
    if (water_level < 20):
        raise WaterError("Not enough water in the tank!")
    return water_level


def catch_errors(err_case: int) -> None:
    match err_case:
        case 0:
            plant_health(30)
        case 1:
            water_tank(5)


def generic_errors(err_case: int) -> None:
    match err_case:
        case 0:
            plant_health(30)
        case 1:
            water_tank(5)


def test_errors() -> None:
    err_cases = [0, 1]
    for err in err_cases:
        try:
            catch_errors(err)
        except (PlantError) as e:
            print("Testing PlantError...")
            print("Caught error:", e, "\n")
        except (WaterError) as e:
            print("Testing WaterError...")
            print("Caught error:", e, "\n")
    print("Testing catching all garden errors..")
    for err in err_cases:
        try:
            generic_errors(err)
        except (GardenError) as e:
            print("Caught error:", e)
    print("\nAll custom error types work correctly!")

    


def main() -> None:
    print("=== Custom Garden Errors Demo ===\n")
    test_errors()


if __name__ == "__main__":
    main()
