
import random


def gen_player_achievements() -> set:
    achivements = ['Crafting Genius', 'Strategist',
                   'World Savior', 'Speed Runner',
                   'Survivor', 'Master Explorer',
                   'Treasure Hunter', 'Unstoppable',
                   'First Steps', 'Collector Supreme',
                   'Untouchable', 'Sharp Mind',
                   'Boss Slayer', 'Hidden Path Finder']
    num_achiv = random.randint(0, 14)
    achiv = []
    i = 0
    while i < num_achiv:
        achiv.append(achivements[random.randint(0, 13)])
        i += 1
    set_achiv = set(achiv)
    return set_achiv


def all_achievements(player1: set, player2: set,
                     player3: set, player4: set) -> set:
    a = player1.union(player2)
    b = player3.union(player4)
    return a.union(b)


def shared_achievements(player1: set, player2: set,
                        player3: set, player4: set) -> set:
    a = player1.intersection(player2)
    b = player3.intersection(player4)
    return a.intersection(b)


def unique_achievemnt(player: set, others: list) -> set:
    return player.difference(*others)


def missing_achiv(player: set, tot_achiv: set) -> set:
    return tot_achiv.difference(player)


def main() -> None:
    print("=== Achievement Tracker System ===\n")
    alice = gen_player_achievements()
    bob = gen_player_achievements()
    charlie = gen_player_achievements()
    dylan = gen_player_achievements()
    print("Player Alice:", alice)
    print("Player Bob:", bob)
    print("Player Charlie:", charlie)
    print("Player Dylan:", dylan)
    tot_achiev = all_achievements(alice, bob, charlie, dylan)
    print("\nAll distinct achievements:", tot_achiev)
    common_achiev = shared_achievements(alice, bob, charlie, dylan)
    print("\nCommon achievements:", common_achiev)
    print("\nOnly Alice has:", alice.difference(bob, charlie, dylan))
    print("Only Bob has:", bob.difference(alice, charlie, dylan))
    print("Only Charlie has:", charlie.difference(alice, bob, dylan))
    print("Only Dylan has:", dylan.difference(alice, bob, charlie))
    print("\nAlice is missing:", missing_achiv(alice, tot_achiev))
    print("\nBob is missing:", missing_achiv(bob, tot_achiev))
    print("\nCharlie is missing:", missing_achiv(charlie, tot_achiev))
    print("\nDylan is missing:", missing_achiv(dylan, tot_achiev))


if __name__ == "__main__":
    main()
