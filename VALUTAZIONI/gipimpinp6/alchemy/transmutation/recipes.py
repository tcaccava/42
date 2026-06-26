from elements import create_fire

from .. import potions, elements as alch_elements


def lead_to_gold() -> str:
    return f"Gold: brew {alch_elements.create_air()} and\
 {potions.strength_potion()} mixed with {create_fire()}"


if __name__ == "__main__":
    gold = lead_to_gold()
    print(f"{gold}")
