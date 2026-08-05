def ft_count_harvest_recursive() -> None:
    days = int(input("Days until harvest: "))
    ft_count_days(days)


def ft_count_days(days: int, current: int = 1) -> None:
    if current > days:
        print("Harvest time!")
    else:
        print("Day", current)
        ft_count_days(days, current + 1)
