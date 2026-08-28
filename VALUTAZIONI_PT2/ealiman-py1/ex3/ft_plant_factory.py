
class Plant:
    def __init__(self, name: str, height: float, plant_age: int) -> None:
        self.name = name
        self.height = height
        self.plant_age = plant_age
        self.initial_height = height

    def show(self) -> None:
        print("Created: " + self.name.capitalize() + ": ",
              self.height, "cm, ", self.plant_age, " days old", sep="")

    def age(self, increment_age: int) -> int:
        self.plant_age = self.plant_age + increment_age
        return self.plant_age

    def grow(self, increment_grow: float) -> float:
        self.height = round(self.height + increment_grow, 1)
        return self.height

    def pass_time(self, period: int, increment_age: int,
                  increment_grow: float) -> None:
        for i in range(1, period + 1):
            print("=== Day ", i, " ===", sep="")
            self.age(increment_age)
            self.grow(increment_grow)
            self.show()

    def tot_grow(self) -> None:
        growth = round(self.height - self.initial_height, 1)
        print("Growth this week: ", growth, "cm", sep="")


plant1 = Plant("Rose", 25.0, 30)
plant2 = Plant("Oak", 200.0, 365)
plant3 = Plant("Cactus", 5.0, 90)
plant4 = Plant("Sunflower", 80.0, 45)
plant5 = Plant("Fern", 15.0, 120)


def main() -> None:
    print("=== Plant Factory Output ===")
    plant1.show()
    plant2.show()
    plant3.show()
    plant4.show()
    plant5.show()


if __name__ == "__main__":
    main()
