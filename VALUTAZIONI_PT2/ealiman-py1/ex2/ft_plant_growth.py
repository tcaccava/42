
class Plant:
    def __init__(self, name: str, height: float, plant_age: int) -> None:
        self.name = name
        self.height = height
        self.plant_age = plant_age
        self.initial_height = height

    def show(self) -> None:
        print(self.name.capitalize() + ": ",
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


def main() -> None:
    print("=== Garden Plant Growth ===")
    plant1.show()
    plant1.pass_time(7, 1, 0.8)
    plant1.tot_grow()


if __name__ == "__main__":
    main()
