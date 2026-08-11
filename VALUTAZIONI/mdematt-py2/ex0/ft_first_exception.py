def input_temperature(temp_str: str) -> int:
    temp: int = int(temp_str)
    return temp


def test_temperature() -> None:
    temp: int = 0
    print("=== Garden Temperature Manager ===")
    print("Input data is '25'")
    print("Asking the temperature manager to change"
          "temperature to '25'")
    print("...")
    temp = input_temperature("25")
    print(f"Temperature is now: {temp}")
    print("Input data is 'abc'")
    print("Asking the temperature manager to change "
          "temperature to 'abc'")
    print("...")
    try:
        temp = input_temperature("abc")
    except ValueError:
        print("abc is an invalid number. Try again.")
    print("If this text is printed, the program works correctly.")


if __name__ == "__main__":
    test_temperature()
