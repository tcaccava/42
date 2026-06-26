from .dark_validator import validate_ingredients as d_validator


def dark_spell_allowed_ingredients() -> list[str]:
    return ["bats", "frogs", "arsenic", "eyeball"]


def dark_spell_record(spell_name: str, ingredient: str) -> str:

    result = d_validator(ingredient)
    if result == "VALID":
        return f"Record: '{spell_name}' successfully uses '{ingredient}'"
    return f"Record Failed: '{ingredient}' is not allowed for '{spell_name}'"


if __name__ == "__main__":
    print(dark_spell_record("diobon", "bats"))
    print(dark_spell_record("diobon", "water"))
