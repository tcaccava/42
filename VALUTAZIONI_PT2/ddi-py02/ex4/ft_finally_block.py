#!/usr/bin/env python3

class GardenError(Exception):
    """Produce GardenError message"""

    def __init__(self, message: str = 'Unknown plant error') -> None:
        self.message = message
        super().__init__(self.message)


class PlantError(GardenError):
    """Produce PlantError message"""
    def __init__(self, plant_name: str = "tomato") -> None:
        message = f"Invalid plant name to water: '{plant_name}'"
        super().__init__(message)


def water_plant(plant_name: str) -> None:
    if plant_name == plant_name.capitalize():
        print(f'Watering {plant_name}: [OK]')
    else:
        raise PlantError(plant_name)


def test_watering_system() -> None:
    print("Opening watering system")
    try:
        for plant in plants:
            try:
                water_plant(plant)
            except PlantError as e:
                print(f"Caught {e.__class__.__name__}: {e}"
                      "\n.. ending tests and returning to main")
                break
    finally:
        print("Closing watering system")


if __name__ == '__main__':
    print("=== Garden Watering System ===")
    print("\nTesting valid plants...")
    plants = ['Tomato', 'Lettuce', 'Carrots']
    test_watering_system()

    print("\nTesting invalid plants...")
    plants = ['Tomato', 'lettuce', 'Carrots']
    test_watering_system()

    print("\nCleanup always happens, even with errors")
