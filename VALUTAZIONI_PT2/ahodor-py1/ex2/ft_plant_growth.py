class Plant:
    def __init__(
            self, name: str,
            height: float,
            age: int,
            growth: float = 0.8
    ) -> None:
        self.name = name.capitalize()
        self.height = height
        self.days = age
        self.growth = growth

    def grow(self) -> None:
        self.height += self.growth

    def age(self) -> None:
        self.days += 1

    def show(self) -> None:
        rounded_height = round(self.height, 1)
        print(f"{self.name}: {rounded_height} cm, {self.days} days old")


def ft_plant_growth() -> None:
    rose = Plant("Rose", 25.0, 30, 0.8)

    print("=== Garden Plant Growth ===")
    rose.show()

    init_height = rose.height

    for day in range(1, 8):
        print("=== Day", str(day), "===")
        rose.grow()
        rose.age()
        rose.show()

    total_growth = round(rose.height - init_height, 1)
    print("Growth this week:", str(total_growth) + "cm")


if __name__ == "__main__":
    ft_plant_growth()
