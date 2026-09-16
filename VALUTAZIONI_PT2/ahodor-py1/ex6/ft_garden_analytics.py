class Plant:
    class PlantStats:
        def __init__(self) -> None:
            self._grow_count = 0
            self._age_count = 0
            self._show_count = 0

        def log_grow(self) -> None:
            self._grow_count += 1

        def log_age(self) -> None:
            self._age_count += 1

        def log_show(self) -> None:
            self._show_count += 1

        def display(self) -> None:
            print(
                f"Stats: {self._grow_count} grow, "
                f"{self._age_count} age, {self._show_count} show"
            )

    def __init__(self, name: str, height: float, age: int) -> None:
        self.name = name.capitalize()
        self._height = 0.0
        self._age = 0
        self.stats = self.PlantStats()
        self.set_height(height)
        self.set_age(age)

    @staticmethod
    def is_older_than_year(age: int) -> bool:
        return age > 365

    @classmethod
    def create_anonymous(cls) -> "Plant":
        return cls("Unknown plant", 0.0, 0)

    def set_height(self, height: float) -> None:
        if height < 0:
            print(f"{self.name}: Error, height can't be negative")
            print("Height update rejected")
        else:
            self._height = float(height)

    def set_age(self, age: int) -> None:
        if age < 0:
            print(f"{self.name}: Error, age can't be negative")
            print("Age update rejected")
        else:
            self._age = age

    def get_height(self) -> float:
        return self._height

    def get_age(self) -> int:
        return self._age

    def grow(self, amount: float = 0.8) -> None:
        self._height += amount
        self.stats.log_grow()

    def age(self, days: int = 1) -> None:
        self._age += days
        self.stats.log_age()

    def format_height(self) -> str:
        rounded_val = round(self._height, 1)
        if rounded_val == int(rounded_val):
            return f"{int(rounded_val)}.0"
        return f"{rounded_val}"

    def show(self) -> None:
        self.stats.log_show()
        print(f"{self.name}: {self.format_height()}cm, {self._age} days old")


class Flower(Plant):
    def __init__(self, name: str, height: float, age: int, color: str) -> None:
        super().__init__(name, height, age)
        self.color = color
        self.is_blooming = False

    def bloom(self) -> None:
        self.is_blooming = True

    def show(self) -> None:
        super().show()
        print(f"Color: {self.color}")
        if not self.is_blooming:
            print(f"{self.name} has not bloomed yet")
        else:
            print(f"{self.name} is blooming beautifully!")


class Tree(Plant):
    class TreeStats(Plant.PlantStats):
        def __init__(self) -> None:
            super().__init__()
            self._shade_count = 0

        def log_shade(self) -> None:
            self._shade_count += 1

        def display(self) -> None:
            super().display()
            print(f"{self._shade_count} shade")

    def __init__(
        self, name: str, height: float, age: int, trunk_diameter: float
    ) -> None:
        super().__init__(name, height, age)
        self.trunk_diameter = float(trunk_diameter)
        self.stats: Tree.TreeStats = self.TreeStats()

    def produce_shade(self) -> None:
        self.stats.log_shade()
        print(
            f"Tree {self.name} now produces a shade of "
            f"{self.format_height()}cm long and {self.trunk_diameter}cm wide."
        )

    def show(self) -> None:
        super().show()
        print(f"Trunk diameter: {self.trunk_diameter}cm")


class Vegetable(Plant):
    def __init__(
        self,
        name: str,
        height: float,
        age: int,
        harvest_season: str,
        nutritional_value: int = 0
    ) -> None:
        super().__init__(name, height, age)
        self.harvest_season = harvest_season
        self.nutritional_value = nutritional_value

    def grow(self, amount: float = 2.1) -> None:
        super().grow(amount)
        self.nutritional_value += 1

    def age(self, days: int = 1) -> None:
        super().age(days)

    def show(self) -> None:
        super().show()
        print(f"Harvest season: {self.harvest_season}")
        print(f"Nutritional value: {self.nutritional_value}")


class Seed(Flower):
    def __init__(
        self,
        name: str,
        height: float,
        age: int,
        color: str,
        seed_count: int = 0
    ) -> None:
        super().__init__(name, height, age, color)
        self.seed_count = seed_count

    def bloom(self) -> None:
        super().bloom()
        self.seed_count = 42

    def show(self) -> None:
        super().show()
        print(f"Seeds: {self.seed_count}")


def print_plant_stats(plant: Plant) -> None:
    print(f"[statistics for {plant.name}]")
    plant.stats.display()


def ft_garden_analytics() -> None:
    print("=== Garden statistics ===")
    print("=== Check year-old")
    print(f"Is 30 days more than a year? -> {Plant.is_older_than_year(30)}")
    print(f"Is 400 days more than a year? -> {Plant.is_older_than_year(400)}")

    print("\n=== Flower")
    rose = Flower("Rose", 15.0, 10, "red")
    rose.show()
    print_plant_stats(rose)

    print("[asking the rose to grow and bloom]")
    rose.grow(8.0)
    rose.bloom()
    rose.show()
    print_plant_stats(rose)

    print("\n=== Tree")
    oak = Tree("Oak", 200.0, 365, 5.0)
    oak.show()
    print_plant_stats(oak)

    print("[asking the oak to produce shade]")
    oak.produce_shade()
    print_plant_stats(oak)

    print("\n=== Seed")
    sunflower = Seed("Sunflower", 80.0, 45, "yellow")
    sunflower.show()

    print("[make sunflower grow, age and bloom]")
    sunflower.grow(30.0)
    sunflower.age(20)
    sunflower.bloom()
    sunflower.show()
    print_plant_stats(sunflower)

    print("\n=== Anonymous")
    anon_plant = Plant.create_anonymous()
    anon_plant.show()
    print_plant_stats(anon_plant)


if __name__ == "__main__":
    ft_garden_analytics()
