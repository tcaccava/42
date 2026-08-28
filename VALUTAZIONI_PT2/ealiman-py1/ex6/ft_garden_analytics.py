
class Plant:
    def __init__(self, name: str, height: float, plant_age: int) -> None:
        self._name = name
        self._height = height
        self._plant_age = plant_age
        self._initial_height = height
        self._stats = self._Statistics(0, 0, 0)

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
        self._stats._show_count += 1
        print(self._name.capitalize() + ": ",
              self._height, "cm, ", self._plant_age, " days old", sep="")

    def age(self, increment_age: int) -> int:
        self._stats._age_count += 1
        self._plant_age = self._plant_age + increment_age
        return self._plant_age

    def grow(self, increment_grow: float) -> float:
        self._stats._grow_count += 1
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

    @staticmethod
    def is_older(days: int) -> bool:
        if (days > 365):
            return True
        else:
            return False

    @classmethod
    def create_anonymous(cls) -> "Plant":
        anonymous = Plant("Unknown plant", 0, 0)
        return anonymous

    class _Statistics:
        def __init__(self, grow_count: int,
                     age_count: int, show_count: int) -> None:
            self._grow_count = grow_count
            self._age_count = age_count
            self._show_count = show_count


class Flower(Plant):
    def __init__(self, name: str, height: float, plant_age: int,
                 color: str) -> None:
        super().__init__(name, height, plant_age)
        self._color = color
        self._has_bloomed = False

    def bloom(self) -> bool:
        self._has_bloomed = True
        return self._has_bloomed

    def show(self) -> None:
        super().show()
        print("Color: ", self._color)
        if (self._has_bloomed):
            print(self._name.capitalize() + " is blooming beautifully!")
        else:
            print(self._name.capitalize(), "has not bloomed yet")


class Tree(Plant):
    def __init__(self, name: str, height: float,
                 plant_age: int, trunk_diameter: float) -> None:
        super().__init__(name, height, plant_age)
        self._trunk_diameter = trunk_diameter
        self._shade_count = 0

    def produce_shade(self) -> None:
        self._shade_count += 1
        print("Tree Oak now produce a shade of ", self._height,
              "cm long and ", self._trunk_diameter, " wide", sep="")

    def show(self) -> None:
        super().show()
        print("Trunk diameter: ", self._trunk_diameter, "cm", sep="")


class Vegetable(Plant):
    def __init__(self, name: str, height: float, plant_age: int,
                 harvest_season: str, nutritional_value: int) -> None:
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


class Seed(Flower):
    def __init__(self, name: str, height: float,
                 plant_age: int, color: str) -> None:
        super().__init__(name, height, plant_age, color)
        self._seed = 0

    def bloom(self) -> bool:
        super().bloom()
        self._seed = 42
        return self._has_bloomed

    def show(self) -> None:
        super().show()
        print("Seed:", self._seed)

    # def show_stats(self) -> None:
    #     super().display_stats()


def display_stats_plant(plant: Plant) -> None:
    print("[statistics for ", plant._name, "]", sep="")
    print("Stats: ", plant._stats._grow_count, "grow,",
          plant._stats._age_count, "age,",
          plant._stats._show_count, "show")
    if isinstance(plant, Tree):
        print(plant._shade_count, "shade")


rose = Flower("Rose", 15.0, 10, "red")
oak = Tree("Oak", 200.0, 365, 5.0)
sunflower = Seed("Sunflower", 80.0, 45, "yellow")
anonymous = Plant.create_anonymous()


def main() -> None:
    print("=== Garden statistics ===")
    print("=== Check year-old")
    print("Is 30 days more than a year? ->", Plant.is_older(30))
    print("Is 400 days more than a year? ->", Plant.is_older(400))
    print("\n")
    print("=== Flower")
    rose.show()
    display_stats_plant(rose)
    print("[asking the rose to grow and bloom]")
    rose.grow(8.0)
    rose.bloom()
    rose.show()
    display_stats_plant(rose)
    print("\n")
    print("=== Tree")
    oak.show()
    display_stats_plant(oak)
    print("[asking the oak to produce shade]")
    oak.produce_shade()
    display_stats_plant(oak)
    print("\n")
    print("=== Seed")
    sunflower.show()
    display_stats_plant(sunflower)
    print("[make sunflower grow, age and bloom]")
    sunflower.grow(30.0)
    sunflower.age(20)
    sunflower.bloom()
    sunflower.show()
    display_stats_plant(sunflower)
    print("\n")
    print("=== Anonymous")
    anonymous.show()
    display_stats_plant(anonymous)


if __name__ == "__main__":
    main()
