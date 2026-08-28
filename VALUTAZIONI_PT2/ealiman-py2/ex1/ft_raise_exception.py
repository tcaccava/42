
class ErrorTemperature(Exception):
    pass


def input_temperature(temp_str: str) -> int:
    temp_int = int(temp_str)
    if (temp_int < 0):
        raise ErrorTemperature(" is too cold for plants (max 0°C)")
    elif (temp_int > 40):
        raise ErrorTemperature(" is too hot for plants (max 40°C)")
    else:
        return temp_int


def test_temperature() -> None:
    temperatures = ["25", "abc", "100", "-50"]
    for temp in temperatures:
        try:
            print("Input data is '", temp, "'", sep="")
            result = input_temperature(temp)
            print("Temperature is now ", result, "°C", sep="")
            print("\n")
        except (ErrorTemperature) as e:
            print("Caught input_temperature error: ", temp, "°C", e, sep="")
            print("\n")
        except (ValueError) as e:
            print("Error", e)
            print("\n")
    print("All tests completed - program didn't crash!")


def main() -> None:
    print("=== Garden Temperature Checker ===")
    print("\n")
    test_temperature()


if __name__ == "__main__":
    main()
