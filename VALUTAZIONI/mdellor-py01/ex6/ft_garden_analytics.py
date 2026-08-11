class Plant:
    def __init__(self, name: str = "Unknown", height: float = 0.0,
                 age: int = 0) -> None:
        self.name = name
        self._height = height
        self._age = age
        self._stats = self.Stats()

    class Stats:
        def __init__(self) -> None:
            self._grow_count = 0
            self._age_count = 0
            self._show_count = 0

        def show(self) -> None:
            print(f"Stats: {self._grow_count} grow, "
                  f"{self._age_count} age, {self._show_count} show")

    @staticmethod
    def is_older_than_year(age: int) -> bool:
        return age > 365

    @classmethod
    def create_anonymous(cls) -> "Plant":
        return cls("Unknown plant", 0.0, 0)

    def show(self) -> None:
        print(f"{self.name}: {round(self._height, 1)}cm, {self._age} days old")
        self._stats._show_count += 1

    def grow(self) -> None:
        self._height += 0.8
        self._stats._grow_count += 1

    def age_up(self) -> None:
        self._age += 1
        self._stats._age_count += 1

    def show_stats(self) -> None:
        print(f"[statistics for {self.name}]")
        self._stats.show()


class Flower(Plant):
    def __init__(self, name: str = "Unknown", height: float = 0.0,
                 age: int = 0, color: str = "unknown") -> None:
        super().__init__(name, height, age)
        self.color = color
        self.bloomed = False

    def bloom(self) -> None:
        self.bloomed = True

    def show(self) -> None:
        super().show()
        print(f" Color: {self.color}")
        if self.bloomed:
            print(f" {self.name} is blooming beautifully!")
        else:
            print(f" {self.name} has not bloomed yet")


class Tree(Plant):
    class Stats(Plant.Stats):
        def __init__(self) -> None:
            super().__init__()
            self._shade_count = 0

        def show(self) -> None:
            super().show()
            print(f" {self._shade_count} shade")

    def __init__(self, name: str = "Unknown", height: float = 0.0,
                 age: int = 0, trunk_diameter: float = 0.0) -> None:
        super().__init__(name, height, age)
        self.trunk_diameter = trunk_diameter

    def produce_shade(self) -> None:
        print(f"Tree {self.name} now produces a shade of "
              f"{round(self._height, 1)}cm long and "
              f"{round(self.trunk_diameter, 1)}cm wide.")
        self._stats._shade_count += 1  # type: ignore[attr-defined]

    def show(self) -> None:
        super().show()
        print(f" Trunk diameter: {round(self.trunk_diameter, 1)}cm")


class Vegetable(Plant):
    def __init__(self, name: str = "Unknown", height: float = 0.0,
                 age: int = 0, harvest_season: str = "unknown") -> None:
        super().__init__(name, height, age)
        self.harvest_season = harvest_season
        self.nutritional_value = 0

    def grow(self) -> None:
        super().grow()
        self.nutritional_value += 1

    def age_up(self) -> None:
        super().age_up()
        self.nutritional_value += 1

    def show(self) -> None:
        super().show()
        print(f" Harvest season: {self.harvest_season}")
        print(f" Nutritional value: {self.nutritional_value}")


class Seed(Flower):
    def __init__(self, name: str = "Unknown", height: float = 0.0,
                 age: int = 0, color: str = "unknown") -> None:
        super().__init__(name, height, age, color)
        self.seed_count = 0

    def bloom(self) -> None:
        super().bloom()
        self.seed_count = 42

    def show(self) -> None:
        super().show()
        print(f" Seeds: {self.seed_count}")


if __name__ == "__main__":
    print("=== Garden statistics ===")

    print("=== Check year-old")
    print(f"Is 30 days more than a year? -> {Plant.is_older_than_year(30)}")
    print(f"Is 400 days more than a year? -> {Plant.is_older_than_year(400)}")
    print()

    print("=== Flower")
    rose = Flower("Rose", 15.0, 10, "red")
    rose.show()
    rose.show_stats()
    print("[asking the rose to grow and bloom]")
    rose.grow()
    rose.bloom()
    rose.show()
    rose.show_stats()
    print()

    print("=== Tree")
    oak = Tree("Oak", 200.0, 365, 5.0)
    oak.show()
    oak.show_stats()
    print("[asking the oak to produce shade]")
    oak.produce_shade()
    oak.show_stats()
    print()

    print("=== Seed")
    sunflower = Seed("Sunflower", 80.0, 45, "yellow")
    sunflower.show()
    print("[make sunflower grow, age and bloom]")
    sunflower.grow()
    sunflower.age_up()
    sunflower.bloom()
    sunflower.show()
    sunflower.show_stats()
    print()

    print("=== Anonymous")
    unknown = Plant.create_anonymous()
    unknown.show()
    unknown.show_stats()
