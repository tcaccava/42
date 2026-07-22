from .dark_spellbook import dark_spell_allowed_ingredients


def validate_ingredients_dark(ingredients: str) -> str:
    for allowed in dark_spell_allowed_ingredients():
        if allowed in ingredients.lower():
            return ingredients + "- VALID"
    return ingredients + "- INVALID"
