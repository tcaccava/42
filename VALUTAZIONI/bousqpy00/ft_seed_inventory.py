def ft_seed_inventory(seed_type: str, quantity: int, unit: str):
    print(f"{seed_type.capitalize()} ", end="")
    if unit == "packets":
        print(f"seeds: {quantity} {unit} avaible")
    elif unit == "grams":
        print(f"seeds: {quantity} {unit} total")
    elif unit == "area":
        print(f"seeds: covers {quantity} square meters")
