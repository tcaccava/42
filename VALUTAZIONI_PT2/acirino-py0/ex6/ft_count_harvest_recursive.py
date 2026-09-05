def ft_count_harvest_recursive(i: int = -1, days: int = 0) -> None:
    if i == -1:
        days = int(input("Days until harvest: "))
        ft_count_harvest_recursive(1, days)
        print("Harvest time!")
    elif i <= days:
        print("Day", i)
        ft_count_harvest_recursive(i + 1, days)
