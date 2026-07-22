import alchemy


if __name__ == "__main__":
    print("=== ALEMBIC 4 ===")
    print(f"Creating air...\n{alchemy.create_air()}")
    print("Not all modules can be reached:")  # This will raise an error
    print(f"{alchemy.create_earth()}")
