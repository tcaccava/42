def ft_harvest_total() -> None:
    total = 0
    for day in [0, 1, 2]:
        total += int(input(f"Day {day+1} harvest: "))
    print("Total harvest:", total)
