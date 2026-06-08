import math


def get_player_pos():
    print("=== Game Coordinate System ===")
    print("Get a first set of coordinates")
    coords = None
    while coords is None:
        try:
            coords = tuple(
                float(n)
                for n in input(
                    "Enter new coordinates as floats in format 'x,y,z': "
                ).split(",")
            )
        except Exception:
            print("Invalid syntax")
    print(f"Got a first tuple: {coords}")
    print(f"It includes: X={coords[0]}, Y={coords[1]}, Z={coords[2]}")
    distance = round(math.sqrt(
        coords[0] ** 2 + coords[1] ** 2 + coords[2] ** 2), 4)
    print(f"Distance to center: {distance}\n")
    print("Get a second set of coordinates")
    second_coords = None
    while second_coords is None:
        strings = input(
            "Enter new coordinates as floats in format 'x,y,z': ").split(
            ","
        )
        for n in strings:
            try:
                float(n)
            except Exception:
                print(
                    f"Error on parameter '{n}': "
                    f"could not convert string to float: '{n}'"
                )
        try:
            second_coords = tuple(float(n) for n in strings)
        except Exception:
            pass
    distance_between = round(
        math.sqrt(
            (coords[0] - second_coords[0]) ** 2
            + (coords[1] - second_coords[1]) ** 2
            + (coords[2] - second_coords[2]) ** 2
        ),
        4,
    )
    print(f"Distance between the 2 sets of coordinates: {distance_between}")


if __name__ == "__main__":
    get_player_pos()
