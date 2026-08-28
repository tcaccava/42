
import math


def get_player_pos() -> tuple:
    while True:
        pos = input("Enter new coordinates"
                    " as float in format: 'x,y,z':").strip()
        li_pos = pos.split(",")
        count = 0
        for _ in li_pos:
            count += 1
        if count != 3:
            print("Invalid sintax")
            continue
        conv_in_flat = True
        converted = []
        for el in li_pos:
            try:
                converted.append(float(el))
            except ValueError as e:
                print(f"Error on parametr '{el}'", e)
                conv_in_flat = False
        if not conv_in_flat:
            continue
        return tuple(converted)


def distance_from_center(pos: tuple) -> float:
    x, y, z = pos
    distance = math.sqrt((x ** 2) + (y ** 2) + (z ** 2))
    round_dis = round(distance, 4)
    return round_dis


def distance_between_point(firs_coord: tuple, second_coord: tuple) -> float:
    x1, y1, z1 = firs_coord
    x2, y2, z2 = second_coord
    coord_x = (x2 - x1) ** 2
    coord_y = (y2 - y1) ** 2
    coor_z = (z2 - z1) ** 2
    distance = math.sqrt(coord_x + coord_y + coor_z)
    round_dis = round(distance, 4)
    return round_dis


def main() -> None:
    print("=== Game Coordinate System ===\n")
    print("Get a first set of coordinates")
    firs_coord = get_player_pos()
    print("Got a first tuple:", firs_coord)
    print(f"It includes: X={firs_coord[0]}",
          f"Y={firs_coord[1]}, Z={firs_coord[2]}")
    print("Distance to the center:", distance_from_center(firs_coord))
    print("\n")
    print("Get a second set of coordinates")
    second_coord = get_player_pos()
    print("Distance between the 2 set of coordinates:",
          distance_between_point(firs_coord, second_coord))


if __name__ == "__main__":
    main()
