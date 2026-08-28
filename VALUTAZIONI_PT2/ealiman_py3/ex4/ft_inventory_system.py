
import sys


def insert_item() -> dict:
    items = sys.argv[1:]
    dict_items = dict()
    for i in items:
        pair = i.split(":")
        if len(pair) != 2:
            print(f"Error - Invalid parameter '{i}'")
            continue
        if pair[0] in dict_items:
            print(f"Redundant item '{pair[0]}' - discarding")
            continue
        try:
            qty = int(pair[1])
        except ValueError as e:
            print(f"Quantity error for {pair[1]}:", e)
            continue
        dict_items[pair[0]] = qty
    return dict_items


# lista dei nomi degli items
def inventory_list(inventory: dict) -> list:
    item = []
    for key in inventory:
        item.append(key)
    return item


# calcolare e ritornare il totale degli items nell'inventario
def tot_items(inventory: dict) -> None:
    tot_items = 0
    tot_qty = 0
    for value in inventory.values():
        tot_items += 1
        tot_qty += value
    print(f"Total quantity of the {tot_items} items: {tot_qty}")


# calcolare la percentuale di un item dentro l'inventario
def item_perc(item: str, inventory: dict) -> None:
    tot_items = sum(inventory.values())
    perc = (inventory[item] / tot_items) * 100
    print(f"Item {item} represents {perc:.1f}%")


def perc_list(inv_list: list, inventory: dict) -> None:
    for item in inv_list:
        item_perc(item, inventory)


# torvare l'item con quantita maggiore e quello con quantita minore
def max_and_min(inventory: dict) -> None:
    first = True
    for key in inventory:
        value = inventory[key]
        if first:
            max_key = key
            min_key = key
            max_val = value
            min_val = value
            first = False
        else:
            if value > max_val:
                max_val = value
                max_key = key
            if value < min_val:
                min_val = value
                min_key = key
    print(f"Item most abundant: {max_key} with quantity {max_val}")
    print(f"Item least abundant: {min_key} with quantity {min_val}")


def main() -> None:
    print("=== Inventory System Analysis ===\n")
    inventory = insert_item()
    if inventory:
        print("Got inventory:", inventory)
        inv_list = inventory_list(inventory)
        print("Item list:", inv_list)
        tot_items(inventory)
        perc_list(inv_list, inventory)
        max_and_min(inventory)
        inventory["magic_item"] = 1
        print("Updated inventory:", inventory)
    else:
        print("Inventory is empty")


if __name__ == "__main__":
    main()
