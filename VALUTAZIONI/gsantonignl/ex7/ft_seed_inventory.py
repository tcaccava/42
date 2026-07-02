def ft_seed_inventory(seed_type, quantity, unit):
	seed_type = seed_type.capitalize()
	if unit == "packets":
		print(seed_type + " seeds: " + str(quantity) + " packets available")
	elif unit == "grams":
		print(seed_type + " seeds: " + str(quantity) + " grams total")
	elif unit == "area":
		print(seed_type + " seeds: " + "covers " + str(quantity) + " square meters")
	else:
		print("Unknown unit type")