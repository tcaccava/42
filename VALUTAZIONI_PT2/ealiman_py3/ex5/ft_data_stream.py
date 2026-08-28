
import random
import typing


def gen_event() -> typing.Generator[tuple[str, str], None, None]:
    players = ["Alice", "Bob", "Charlie", "Dylan"]
    actions = ["jump", "run", "attack", "defend", "sleep"]
    while True:
        player = random.choice(players)
        action = random.choice(actions)
        event = (player, action)
        yield event

def consume_event(
 list_event: list
) -> typing.Generator[tuple[str, str], None, None]:
    while list_event:
        index = random.randint(0, len(list_event) - 1)
        eliminated = list_event.pop(index)
        yield eliminated


def main() -> None:
    print("=== Game Data Stream Processor ===")
    gen = gen_event()
    for i in range(0, 1001):
        name, action = next(gen)
        print(f"Event {i}: Player {name} did action {action}")
    list_event = []
    for _ in range(10):
        list_event.append(next(gen))
    print(f"Built list of 10 events: {list_event}")
    for event in consume_event(list_event):
        print(f"Got event from list: {event}")
        print(f"Remains in list: {list_event}")


if __name__ == "__main__":
    main()
