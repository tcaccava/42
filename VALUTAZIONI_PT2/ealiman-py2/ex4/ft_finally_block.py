
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


def water_plant(plant_name: str) -> None:
    if (plant_name != plant_name.capitalize()):
        raise PlantError(f"Invalid plant name to water: {plant_name}")
    else:
        print("Watering: ", plant_name, ": [OK]", sep="")


def test_watering_system() -> None:
    print("Testing valid plants...")
    try:
        print("Opening watering system")
        water_plant("Tomato")
        water_plant("Lettuce")
        water_plant("Carrots")
        print("Testing invalid plants...")
        print("Opening watering system")
        water_plant("lettuce")
    except PlantError as e:
        print("Caught PlantError:", e)
        print(".. ending test and returning to main")
        return
    finally:
        print("Closing watering system")


def main() -> None:
    print("=== Garden Watering System ===\n")
    test_watering_system()
    print("Cleanup always happens, even with errors!")


if __name__ == "__main__":
    main()
