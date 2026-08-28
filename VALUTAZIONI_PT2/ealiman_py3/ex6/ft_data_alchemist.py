
import random


players = ['Alice', 'bob', 'Charlie', 'dylan', 'Emma',
           'Gregory', 'john', 'kevin', 'Liam']
cap_list = [val.capitalize() for val in players]
only_cap = [val for val in players if val == val.capitalize()]
score_dict = {value: random.randint(0, 1000) for value in cap_list}
score_sum = 0
length = 0
for key in score_dict:
    score_sum += score_dict[key]
    length += 1
score_avg = round(score_sum / length)
high_score = {player: score for player,
              score in score_dict.items() if score > score_avg}


def main() -> None:
    print("=== Game Data Alchemist ===\n")
    print("Initial list of players:", players)
    print("New list with all names capitalized:", cap_list)
    print("New list of capitalized names only:", only_cap)
    print("Score dict:", score_dict)
    print("Score average is", score_avg)
    print("High scores:", high_score)


if __name__ == "__main__":
    main()
