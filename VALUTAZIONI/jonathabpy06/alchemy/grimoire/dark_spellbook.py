from .dark_validator import validate_ingredients_dark


def dark_spell_allowed_ingredients():
    return ["bats", "frogs", "arsenic", "eyeball"]


def dark_spell_record(spell_name: str, ingredients: str) -> str:
    validation = validate_ingredients_dark(ingredients)
    if "INVALID" in validation:
        return spell_name + f" Rejected: ({validation})"
    else:
        return spell_name + f" Recorded: ({validation})"
