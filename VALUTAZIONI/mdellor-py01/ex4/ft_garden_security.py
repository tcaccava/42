class Plant:
    def __init__(self, name: str = "Unknown", height: float = 0.0,
                 age: int = 0) -> None:
        self.name = name
        self._height = 0.0
        self._age = 0
        self.set_height(height)
        self.set_age(age)

    def show(self) -> None:
        print(f"{self.name}: {round(self._height, 1)}cm, {self._age} days old")

    def get_height(self) -> float:
        return self._height

    def get_age(self) -> int:
        return self._age

    def set_height(self, value: float) -> None:
        if value < 0:
            print(f"{self.name}: Error, height can't be negative")
            print("Height update rejected")
        else:
            self._height = value

    def set_age(self, value: int) -> None:
        if value < 0:
            print(f"{self.name}: Error, age can't be negative")
            print("Age update rejected")
        else:
            self._age = value


if __name__ == "__main__":
    print("=== Garden Security System ===")
    rose = Plant("Rose", 15.0, 10)
    print(f"Plant created: {rose.name}: {rose.get_height()}cm, "
          f"{rose.get_age()} days old")
    print()

    rose.set_height(25.0)
    print(f"Height updated: {rose.get_height()}cm")
    rose.set_age(30)
    print(f"Age updated: {rose.get_age()} days")
    print()

    rose.set_height(-5)
    print("Height update rejected")
    rose.set_age(-10)
    print("Age update rejected")
    print()

    print("Current state:", end=" ")
    rose.show()
