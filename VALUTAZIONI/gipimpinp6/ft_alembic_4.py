import alchemy

if __name__ == "__main__":
    try:
        alchemy.create_earth()
    except AttributeError:
        print("No access in create_earth")
