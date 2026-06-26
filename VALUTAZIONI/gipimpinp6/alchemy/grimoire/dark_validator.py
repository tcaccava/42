from .dark_spellbook import dark_spell_allowed_ingredients


def validate_ingredients(ingredient: str) -> str:
    l_ele = dark_spell_allowed_ingredients()
    if any(
            elemento.split()[0].lower()
            in ingredient.lower()
            for elemento in l_ele
            ):
        return "VALID"
    return "INVALID"


if __name__ == "__main__":
    print(validate_ingredients("e ciccio bats"))
    print(validate_ingredients("ciccio ciao water"))
