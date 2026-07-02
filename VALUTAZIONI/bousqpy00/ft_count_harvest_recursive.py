def ft_count_harvest_recursive(d: int | None = None, i=1):
    if d is None:
        d = int(input("Days until harvest: "))
    if d < i:
        print("Harvest time")
        return
    print(f"Day {i}")
    ft_count_harvest_recursive(d, i + 1)
