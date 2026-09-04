def ft_count_harvest_recursive() -> None:
    def count_days(current: int, last: int) -> None:
        if current > last:
            return
        print(f"Day {current}")
        count_days(current + 1, last)

    days = int(input("Days until harvest: "))
    count_days(1, days)
    print("Harvest time!")
