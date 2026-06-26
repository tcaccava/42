def validate_ingredients(ingredient: str) -> str:
    from .light_spellbook import light_spell_allowed_ingredients

    l_ele = light_spell_allowed_ingredients()
    if any(
            elemento.split()[0].lower()
            in ingredient.lower()
            for elemento in l_ele
           ):
        return "VALID"
    return "INVALID"


if __name__ == "__main__":
    print(validate_ingredients("e ciccio ciao"))
    print(validate_ingredients("ciccio ciao water"))
