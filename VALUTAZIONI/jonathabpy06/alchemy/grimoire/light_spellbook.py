def light_spell_allowed_ingredients():
    return ["earth", "air", "fire", "water"]


def light_spell_record(spell_name: str, ingredients: str) -> str:
    from .light_validator import validate_ingredients
    validation = validate_ingredients(ingredients)
    if "INVALID" in validation:
        return spell_name + f" Rejected: ({validation})"
    else:
        return spell_name + f" Recorded: ({validation})"
