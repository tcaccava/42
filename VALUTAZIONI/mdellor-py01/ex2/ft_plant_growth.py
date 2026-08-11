class Plant:
    def __init__(self) -> None:
        self.name = ""
        self.height = 0.0
        self.age = 0

    def show(self) -> None:
        print(f"{self.name}: {round(self.height, 1)}cm, {self.age} days old")

    def grow(self) -> None:
        self.height += 0.8

    def age_up(self) -> None:
        self.age += 1


if __name__ == "__main__":
    rose = Plant()
    rose.name = "Rose"
    rose.height = 25.0
    rose.age = 30

    starting_height = rose.height

    print("=== Garden Plant Growth ===")
    rose.show()

    for day in range(1, 8):
        rose.grow()
        rose.age_up()

        print(f"=== Day {day} ===")
        rose.show()

    increase = round(rose.height - starting_height, 1)
    print(f"Growth this week: {increase}cm")
