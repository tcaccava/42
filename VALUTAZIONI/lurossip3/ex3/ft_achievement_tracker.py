import random


def create_player(achievements) -> set:
    player_set = set()
    for _ in range(random.randint(1, 12)):
        player_set.add(random.choice(list(achievements)))
    return player_set


def get_player_achievements():
    achievements = {
        "Crafting Genius",
        "Strategist",
        "World Savior",
        "Speed Runner",
        "Survivor",
        "Master Explorer",
        "Treasure Hunter",
        "Unstoppable",
        "First Steps",
        "Collector Supreme",
        "Untouchable",
        "Sharp Mind",
        "Boss Slayer",
    }
    alice = create_player(achievements)
    bob = create_player(achievements)
    dylan = create_player(achievements)
    charlie = create_player(achievements)
    print("=== Achievement Tracker System ===")
    print(f"Player Alice: {alice}")
    print(f"Player Bob: {bob}")
    print(f"Player Dylan: {dylan}")
    print(f"Player Charlie: {charlie}")
    print(f"All distinct achievements: {achievements}")
    print(f"\nCommon achievements: {alice.intersection(bob, charlie, dylan)}")
    print(f"\nOnly Alice has: {alice.difference(bob, dylan, charlie)}")
    print(f"Only Bob has: {bob.difference(alice, dylan, charlie)}")
    print(f"Only Dylan has: {dylan.difference(bob, alice, charlie)}")
    print(f"Only Charlie has: {charlie.difference(bob, dylan, alice)}")
    print(f"\nAlice is missing: {achievements.difference(alice)}")
    print(f"Bob is missing: {achievements.difference(bob)}")
    print(f"Dylan is missing: {achievements.difference(dylan)}")
    print(f"Charlie is missing: {achievements.difference(charlie)}")


if __name__ == "__main__":
    get_player_achievements()
