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


def water_plant(plant_name: str) -> None:
    if plant_name == plant_name.capitalize():
        print(f"watering {plant_name}: [OK] ")
    else:
        raise PlantError(f"Invalid plant name to water: '{plant_name}'")


def test_watering_system(plants: list[str]) -> None:
    print("Opening watering system")
    try:
        for p in plants:
            water_plant(p)
    except PlantError as ex:
        print(f"Caught PlantError: {ex}")
        print(".. ending tests and returning to main")
    finally:
        print("Closing watering system")


def main() -> None:
    print("=== Garden Watering System ===")
    print("\nTesting valid plants...")
    test_watering_system(["Tomato", "Lettuce", "Carrots"])
    print("\nTesting invalid plants...")
    test_watering_system(["Tomato", "lettuce", "carrots"])
    print("\nCleanup always happens, even with errors!")


if __name__ == "__main__":
    main()
