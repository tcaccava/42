def validate_ingredients(ingredients: str) -> str:
    from .light_spellbook import light_spell_allowed_ingredients
    for allowed in light_spell_allowed_ingredients():
        if allowed in ingredients.lower():
            return ingredients + " - VALID"
    return ingredients + " - INVALID"
