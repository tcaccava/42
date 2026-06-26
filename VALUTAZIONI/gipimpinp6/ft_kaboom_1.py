if __name__ == "__main__":
    print("=== Kaboom 1 ===")
    try:
        from alchemy.grimoire.dark_spellbook import dark_spell_record
        print(dark_spell_record("leviosa", "bats"))
    except ImportError as e:
        print("\n[LABORATORY EXPLOSION DETECTED]")
        print(f"Catch Error: {e}")
        print("kaboom!!\n")
