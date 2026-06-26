from elements import create_fire as fire, create_water as water

from alchemy import create_air as air

from alchemy.elements import create_earth as earth


def light_spell_allowed_ingredients() -> list[str]:
    return [fire(), air(), water(), earth()]


def light_spell_record(spell_name: str, ingredient: str) -> str:

    from .light_validator import validate_ingredients as validator

    result = validator(ingredient)
    if result == "VALID":
        return f"Record: '{spell_name}' successfully uses '{ingredient}'"
    return f"Record Failed: '{ingredient}' is not allowed for '{spell_name}'"


if __name__ == "__main__":
    print(light_spell_record("diobon", "eath"))
    print(light_spell_record("diobon", "water"))
