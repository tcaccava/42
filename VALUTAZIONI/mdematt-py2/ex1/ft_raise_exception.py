def input_temperature(temp_str: str) -> int:
    temp: int = int(temp_str)
    if temp > 40:
        raise ValueError(f"{temp}°C is way too hot for plants...")
    if temp < 0:
        raise ValueError(f"{temp}°C is way too cold for plants...")
    return temp


def test_temperature() -> None:
    temp: int = 0
    print("=== Garden Temperature Manager ===")
    print("Input data is '25'")
    print("Asking the temperature manager to change "
          "temperature to '25'")
    print("...")
    temp = input_temperature("25")
    print(f"Temperature is now: {temp}\n")
    print("Input data is 'abc'")
    print("Asking the temperature manager to change "
          "temperature to 'abc'")
    print("...")
    try:
        temp = input_temperature("abc")
    except ValueError:
        print("abc is an invalid number. Try again.\n")
    print("Input data is '100'")
    print("Asking the temperature manager to change "
          "temperature to '100'")
    print("...")
    try:
        temp = input_temperature("100")
    except ValueError as e:
        print(f"{e}\n")
    print("Input data is '-50'")
    print("Asking the temperature manager to change "
          "temperature to '-50'")
    print("...")
    try:
        temp = input_temperature("-50")
    except ValueError as e:
        print(f"{e}\n")
    print("\nIf this text is printed, the program works correctly.")


if __name__ == "__main__":
    test_temperature()
