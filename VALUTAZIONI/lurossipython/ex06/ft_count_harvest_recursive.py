def ft_count_harvest_recursive(days=None, i=1):
    if days is None:
        days = input("Days until harvest: ")
    if i <= int(days):
        print(f"Day {i}")
        ft_count_harvest_recursive(days, i + 1)
    else:
        print("Harvest time!")
