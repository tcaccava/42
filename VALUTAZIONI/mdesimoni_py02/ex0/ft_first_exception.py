#!/bin/python3
def input_temperature(temp_str: str) -> int:
    temp_int = int(temp_str)
    return (temp_int)


def test_temperature() -> None:
    temp_strings: list[str] = ["25", "abc"]
    print("=== Garden Temperature ===\n")
    for temp_str in temp_strings:
        print(f"Input data is '{temp_str}'")
        try:
            T: int = input_temperature(temp_str)
            print(f"Temperature is now {T}°C")
        except Exception as ex:
            print("Caught input_temperature error: ", ex)
        print("\n")
    print("All tests completed - program didn't crash!")


if __name__ == "__main__":
    test_temperature()
