
class Plant:
    def __init__(self, name: str, height: float, plant_age: int) -> None:
        self._name = name
        self._height = height
        self._plant_age = plant_age
        self._initial_height = height

    def set_age(self, plant_age: int) -> None:
        if (plant_age > 0):
            self._plant_age = plant_age
            print("Age updated: ", self._plant_age, " days", sep="")
        else:
            print(self._name, ": Error, age can't be negative", sep="")
            print("Age update rejected")

    def set_height(self, height: float) -> None:
        if (height > 0):
            self._height = height
            print("Height updated: ", self._height, "cm", sep="")
        else:
            print(self._name, ": Error, height can't be negative", sep="")
            print("Height update rejected")

    def get_age(self) -> int:
        return self._plant_age

    def get_height(self) -> float:
        return self._height

    def get_name(self) -> str:
        return self._name

    def show(self) -> None:
        print(self._name.capitalize() + ": ",
              self._height, "cm, ", self._plant_age, " days old", sep="")

    def age(self, increment_age: int) -> int:
        self._plant_age = self._plant_age + increment_age
        return self._plant_age

    def grow(self, increment_grow: float) -> float:
        self._height = round(self._height + increment_grow, 1)
        return self._height

    def pass_time(self, period: int, increment_age: int,
                  increment_grow: float) -> None:
        for i in range(1, period + 1):
            self.age(increment_age)
            self.grow(increment_grow)
            self.show()

    def tot_grow(self) -> None:
        growth = round(self._height - self._initial_height, 1)
        print("Growth this week: ", growth, "cm", sep="")


class Flower(Plant):
    def __init__(self, name: str, height: float, plant_age: int,
                 color: str) -> None:
        super().__init__(name, height, plant_age)
        self._color = color

    def bloom(self) -> None:
        print(self._name.capitalize() + " has not bloomed yet")
        self.show()
        print(self._name.capitalize() + " is blooming beautifully!")

    def show(self) -> None:
        super().show()
        print("Color: ", self._color)


class Tree(Plant):
    def __init__(self, name: str, height: float, plant_age: int,
                 trunk_diameter: float) -> None:
        super().__init__(name, height, plant_age)
        self._trunk_diameter = trunk_diameter

    def produce_shade(self) -> None:
        print("Tree Oak now produce a shade of ", self._height,
              "cm long and ", self._trunk_diameter, " wide", sep="")

    def show(self) -> None:
        super().show()
        print("Trunk diameter: ", self._trunk_diameter, "cm", sep="")


class Vegetable(Plant):
    def __init__(self, name: str, height: float,
                 plant_age: int, harvest_season: str,
                 nutritional_value: int) -> None:
        super().__init__(name, height, plant_age)
        self._harvest_season = harvest_season
        self._nutritional_value = nutritional_value

    def show(self) -> None:
        super().show()
        print("Harvest season: " + self._harvest_season.capitalize())
        print("Nutritional value: ", self._nutritional_value)

    def pass_time(self, period: int, increment_age: int,
                  increment_grow: float) -> None:
        print("[make tomato grow and age for 20 days]")
        for i in range(period):
            self.age(increment_age)
            self.grow(increment_grow)
            self._nutritional_value += 1

    def tot_grow(self) -> None:
        return super().tot_grow()


flower = Flower("Rose", 15.0, 10, "red")
oak = Tree("Oak", 200.0, 365, 5.0)
tomato = Vegetable("Tomato", 5.0, 10, "April", 0)


def main() -> None:
    print("=== Garden Plant Types ===")
    print("=== Flower")
    flower.show()
    print("[asking the rose to bloom]")
    flower.bloom()
    print("\n")
    print("=== Tree")
    oak.show()
    print("[asking the oak to produce shade]")
    oak.produce_shade()
    print("\n")
    print("=== Vegetable")
    tomato.show()
    tomato.pass_time(20, 1, 2.1)
    tomato.show()


if __name__ == "__main__":
    main()
