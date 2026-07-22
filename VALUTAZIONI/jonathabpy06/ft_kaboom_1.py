if __name__ == "__main__":
    print("=== KABOOM 1 ===")
    print("Accessing dark spellbook")
    print("Testing import now - THIS WILL CAUSE AN ERROR:")
    try:
        import alchemy.grimoire.dark_spellbook as dark_spellbook
        print(dark_spellbook.dark_spell_record("dark spell", "toad, frogs"))
    except Exception as e:
        print(f"Error importing with circular dependency: {e}")
