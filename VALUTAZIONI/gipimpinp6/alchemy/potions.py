from .elements import create_earth, create_air

from elements import create_fire, create_water


def healing_potion() -> str:
    return f"Healing potion brewed with {create_earth()}\
 and {create_air()}"


def strength_potion() -> str:
    return f"Strength potion brewed with {create_fire()}\
 and {create_water()}"


if __name__ == "__main__":
    print(healing_potion())
    print(strength_potion())
