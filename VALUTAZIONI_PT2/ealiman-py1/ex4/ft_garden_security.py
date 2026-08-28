
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
        print("Created: " + self._name.capitalize() + ": ",
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
            print("=== Day ", i, "===")
            self.age(increment_age)
            self.grow(increment_grow)
            self.show()

    def tot_grow(self) -> None:
        growth = round(self._height - self._initial_height, 1)
        print("Growth this week: ", growth, "cm", sep="")


plant = Plant("Rose", 15.0, 10)


def main() -> None:
    print("=== Garden Security System ===")
    plant.show()
    print("\n")
    plant.set_height(25.0)
    plant.set_age(30)
    print("\n")
    plant.set_height(-25.0)
    plant.set_age(-30)
    print("\n")
    print("Current state: " + plant.get_name() + ": ",
          plant.get_height(), "cm, ", plant.get_age(), " days old", sep="")


if __name__ == "__main__":
    main()
