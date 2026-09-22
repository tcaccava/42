#!/usr/bin/env python3

class GardenError(Exception):
    """Produce GardenError message"""

    def __init__(self, message: str = 'Unknown plant error') -> None:
        self.message = message
        super().__init__(self.message)


class PlantError(GardenError):
    """Produce PlantError message"""
    def __init__(self, plant_name: str = "tomato") -> None:
        if plant_name:
            message = f"The {plant_name} plant is wilting!"
        else:
            message = "The plant is wilting!"
        super().__init__(message)


class WaterError(GardenError):
    """Produce WaterError message"""
    def __init__(self, message: str = "Not enough water in the tank!") -> None:
        super().__init__(message)


def ft_test_custom_errors() -> None:
    errors: list[type[Exception]] = [PlantError, WaterError]
    print("=== Custom Garden Errors Demo ===")

    for i in range(len(errors)):
        print(f"\nTesting {errors[i].__name__}...")
        try:
            raise errors[i]()
        except PlantError as e:
            print(f'Caught {e.__class__.__name__}: {e}')
        except WaterError as e:
            print(f'Caught {e.__class__.__name__}: {e}')

    print("\nTesting catching all garden errors...")
    for i in range(len(errors)):
        try:
            raise errors[i]()
        except GardenError as e:
            print(f'Caught {e.__class__.__name__}: {e}')
    print("\nAll custom error types work correctly!")


if __name__ == '__main__':
    ft_test_custom_errors()
