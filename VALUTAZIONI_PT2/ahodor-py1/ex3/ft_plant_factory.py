class Plant:
    def __init__(self, name: str, height: float, age: int) -> None:
        self.name = name.capitalize()
        self.height = height
        self.age = age

    def show(self) -> None:
        rounded_height = round(self.height, 1)
        print(f"Created: {self.name}: {rounded_height}cm, {self.age} days old")


def ft_plant_factory() -> None:
    plants = [
        Plant("Rose", 25.0, 30),
        Plant("Oak", 200.0, 365),
        Plant("Cactus", 5.0, 90),
        Plant("Sunflower", 80.0, 45),
        Plant("Fern", 15.0, 120),
    ]

    print("=== Plant Factory Output ===")
    for i in range(len(plants)):
        plants[i].show()


if __name__ == "__main__":
    ft_plant_factory()
