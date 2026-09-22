#!/usr/bin/env python3

def input_temperature(temp_str: str) -> int:
    """Convert a temperature string into a integer.
    Verify if temperature is within aproper range"""
    t = int(temp_str)

    if t > 40:
        raise ValueError(
            f"{t}{chr(176)}C is too hot for plants (max 40{chr(176)}C)")
    if t < 0:
        raise ValueError(
            f"{t}{chr(176)}C is too cold for plants (min 0{chr(176)}C)")

    return (t)


def test_temperature() -> None:
    """filter out bad temperature data"""
    print("=== Garden Temperature Checker ===")

    t_in: list[str] = ["25", "abc", "100", "-50"]
    for i in range(len(t_in)):
        print(f"\nInput data is '{t_in[i]}'")
        try:
            t_out: int = input_temperature(t_in[i])
            print(f"Temperature is now {t_out}{chr(176)}C")
        except ValueError as e:
            print(f"Caught input_temperature error: {e}")
    print("\nAll tests completed - program didn't crash!")


if __name__ == '__main__':
    test_temperature()
