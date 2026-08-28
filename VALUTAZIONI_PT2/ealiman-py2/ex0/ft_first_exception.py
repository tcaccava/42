
def input_temperature(temp_str: str) -> int:
    temp_int = int(temp_str)
    return temp_int


def test_temperature() -> None:
    temperatures = ["42", "abc"]
    for temp in temperatures:
        try:
            temp_int = input_temperature(temp)
            print("Input data is '", temp_int, "'", sep="")
        except (ValueError) as e:
            print("Input data is '", temp_int, "'", sep="")
            print("Error", e)
            print("\n")

        else:
            print("Temperature is now ", temp_int, "°C", sep="")
            print("\n")
    print("All tests completed - program didn't crash!")


def main() -> None:
    print("=== Garden Temperature ===")
    print("\n")
    test_temperature()


if __name__ == "__main__":
    main()
