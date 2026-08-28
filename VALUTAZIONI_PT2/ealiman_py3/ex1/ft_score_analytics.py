
import sys


class ErrorValue(Exception):
    def __init__(self, message: str | None = None) -> None:
        super().__init__(message)


class ErrorInput(Exception):
    def __init__(self, message: str = "No scores provided. Usage:"
                 " python3 ft_score_analytics.py "
                 "<score1> <score2> ...") -> None:
        super().__init__(message)


def valid_score(score: str) -> int:
    try:
        return int(score)
    except ValueError:
        raise ErrorValue("Invalid parameter: ")


def parse_scores() -> list[int]:
    raw_score = sys.argv[1:]
    if len(raw_score) == 0:
        raise ErrorInput
    score_list: list[int] = []
    for score in raw_score:
        try:
            score_list.append(valid_score(score))
        except ErrorValue as e:
            print(e, "'", score, "'", sep="")
    if len(score_list) == 0:
        raise ErrorInput
    return score_list
# O(1)
# 9 O(n)
# append 01
# o(n)

def ft_score_analytics() -> None:
    try:
        score_list = parse_scores()
        print("Scores processed:", score_list)
        len_scores = len(score_list)
        sum_score = sum(score_list)
        avg_score = sum_score / len_scores
        min_score = min(score_list)
        max_score = max(score_list)
        range_score = max_score - min_score
        print("Total players:", len_scores)
        print("Total score: ", sum_score)
        print("Average score:", avg_score)
        print("High score:", max_score)
        print("Low score:", min_score)
        print("Score range:", range_score)
    except ErrorInput as e:
        print(e)


def main() -> None:
    print("=== Player Score Analytics ===")
    ft_score_analytics()


if __name__ == "__main__":
    main()
