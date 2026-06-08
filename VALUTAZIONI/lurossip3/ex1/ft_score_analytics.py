#!./usr/bin/env python
import sys


def scores_analytics():
    print("=== Player Score Analytics ===")
    scores = []
    i = 0
    for score in sys.argv:
        try:
            if i == 0:
                i += 1
            else:
                scores.append(int(score))
        except Exception:
            print(f"Invalid parameter: '{score}'")
    if len(scores) == 0:
        print(
            "No scores provided. Usage: "
            "python3 ft_score_analytics.py <score1> <score2> ..."
        )
    else:
        print(f"Scores processed: {scores}")
        print(f"Total players: {len(scores)}")
        total = 0
        for x in scores:
            total += x
        print(f"Total score: {sum(scores)}")
        print(f"Average score: {round(total / len(scores), 1)}")
        print(f"High score: {max(scores)}")
        print(f"Low score: {min(scores)}")
        print(f"Range: {max(scores) - min(scores)}")


if __name__ == "__main__":
    scores_analytics()
