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


def water_plant(plant_name: str) -> None:
    print(f"Asking garden to water {plant_name}...")
    if plant_name != plant_name.capitalize():
        raise PlantError(f"Invalid Name ({plant_name}), "
                         "try writing it capitalized!")
    else:
        print(f"Watering successful! {plant_name} is now fresh and clean!")


def test_watering_system() -> None:
    try:
        water_plant("Rose")
        water_plant("Daisy")
        water_plant("oak")
        water_plant("basil")
        water_plant("Oak")
    except PlantError as e:
        print(f"Caught PlantError: {e}")
        print("Ending watering procedure and returning to main.")
        return
    finally:
        print("===Watering procedure finalized!===")


if __name__ == "__main__":
    test_watering_system()
