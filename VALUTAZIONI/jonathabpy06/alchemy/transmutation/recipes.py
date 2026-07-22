from ..potions import create_air, strength_potion
from elements import create_fire


def lead_to_gold() -> str:
    return (
        "Recipe transmuting Lead to Gold: " +
        f"brew '{create_air()}' and '{strength_potion()}' " +
        f"mixed with {create_fire()}"
    )
