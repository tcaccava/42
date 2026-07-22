import alchemy.grimoire


if __name__ == "__main__":
    print("=== Kaboom 0 ===")
    print("Using grimoire module directly")
    print("Recording light spell:")
    light = alchemy.grimoire.light_spellbook.light_spell_record(
        "Fantasy",
        "Earth, wind and fire")
    print(light)
