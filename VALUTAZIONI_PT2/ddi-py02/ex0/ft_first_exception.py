#!/usr/bin/env python3

def input_temperature(temp_str: str) -> int:
    """Convert a temperature string into an integer"""
    return (int(temp_str))


def test_temperature() -> None:
    """filter out bad temperature data"""
    print("=== Garden Temperature ===")

    t_in_a: str = "25"
    print(f"\nInput data is '{t_in_a}'")
    try:
        t_out_a: int = input_temperature(t_in_a)
        if -273.15 < t_out_a < 1.4e32:
            print(f"Temperature is now {t_out_a}{chr(176)}C")
        else:
            raise ValueError(
                f"Caught input_temperature error: {t_out_a}{chr(176)}C")
    except ValueError as e:
        print(f"Caught input_temperature error: {e}")

    t_in_b: str = "abc"
    print(f"\nInput data is '{t_in_b}'")
    try:
        t_out_b: int = input_temperature(t_in_b)
        if -274 < int(t_out_b) < 1.4e32:
            print(f"Temperature is now {t_out_b}{chr(176)}C")
        else:
            raise ValueError

    except ValueError as e:
        print(f"Caught input_temperature error: {e}")

    print("\nAll tests completed - program didn't crash!")


if __name__ == '__main__':
    test_temperature()
