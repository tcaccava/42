import sys


def inventory_manager():
    inventory = {}
    print("=== Inventory System Analysis ===")
    for insertion in sys.argv[1:]:
        error_output = insertion
        insertion = insertion.split(":")
        if len(insertion) != 2:
            print(f"Error - invalid parameter '{error_output}'")
            continue
        key = insertion[0]
        if key in inventory.keys():
            print(f"Redundant item '{key}' - discarding")
            continue
        try:
            quantity = int(insertion[1])
        except Exception:
            print(
                f"Quantity error for {key}: invalid literal for int() "
                f"with base 10: '{insertion[1]}'"
            )
            continue
        inventory[key] = quantity
    if not inventory:
        print("No valid items provided.")
        return
    print(f"Got inventory: {inventory}")
    print(f"Item list: {list(inventory.keys())}")
    print(f"Total quantity of the {len(inventory.keys())}"
          " items: {sum(inventory.values())}")
    for key, quantity in inventory.items():
        print(
            f"Item {key} represents "
            f"{round(quantity/ sum(inventory.values())* 100, 1) }%"
        )
    biggest = None
    for item, quantity in inventory.items():
        if biggest is None:
            biggest = (item, quantity)
        elif biggest[1] < quantity:
            biggest = (item, quantity)
    print(f"Item most abundant: {biggest[0]} with quantity {biggest[1]}")
    smallest = None
    for item, quantity in inventory.items():
        if smallest is None:
            smallest = (item, quantity)
        elif smallest[1] > quantity:
            smallest = (item, quantity)
    print(f"Item least abundant: {smallest[0]} with quantity {smallest[1]}")
    inventory.update({"magic_item": 1})
    print(f"Updated inventory: {inventory}")


if __name__ == "__main__":
    if len(sys.argv) == 1:
        pass
    else:
        inventory_manager()
