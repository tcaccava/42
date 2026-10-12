# lemipc

## Summary

`lemipc` is a small battle game in which each player is a separate process.
Players belong to numbered teams and fight on a shared 2D board. A player dies
when two players from the same enemy team stand next to it, diagonals included.
The last team left on the board wins.

The game exists to show how processes can talk to each other through
**System V IPC**. The board lives in a **shared memory** segment. Every access
to the board is guarded by a **semaphore**. Teammates agree on whom to attack by
sending messages through a **message queue**. Players don't use threads or
`fork`: you start one process per player.

## How to run

### Build

libft is a git submodule, so fetch it first:

```sh
git clone --recursive <repo-url> lem-ipc
# or, in an existing clone:
git submodule update --init

make
```

The Makefile also has the usual `clean`, `fclean` and `re` rules.

### Play

Open one terminal for the viewer and another for the players:

```sh
# terminal 1: watch the board (read-only)
./lemipc -v

# terminal 2: one process per player, the argument is the team (1-99)
./lemipc 1 & ./lemipc 1 & ./lemipc 1 &
./lemipc 2 & ./lemipc 2 & ./lemipc 2 &
```

The fight starts 3 seconds after the board holds at least two teams, one of
which has two or more players. Each player prints a line when it joins, gets
killed, wins or leaves. Press `Ctrl-C` to make a player leave the game cleanly.

### Quick test

`test.sh` does all of the above in one terminal. It builds the project,
starts the players and the viewer, and when the game ends prints each
player's log and checks that no IPC object was left behind.

```sh
./test.sh                  # 3 vs 3 (teams 1 and 2), with the viewer
./test.sh 1 1 2 2 3 3      # any list of team numbers, one per player
./test.sh -q 1 1 2 2       # no viewer, just run the game
```

`Ctrl-C` stops every player cleanly, and the summary is still printed. The
script exits with status 1 if a game is already running or if IPC objects
were left behind.

### Check that nothing is left behind

When the last player exits, it removes every IPC object. You can confirm this
with:

```sh
ipcs | grep -i 4c454d49     # should print nothing
```

If processes were killed with `kill -9` and something is left over, remove it
by hand:

```sh
ipcrm -M 0x4c454d49 -S 0x4c454d49 -Q 0x4c454d49
```

## Game rules

- The board is 20 × 12 tiles, and only one player can stand on a tile.
- On each turn a player moves one tile up, down, left or right. Diagonal moves
  are not allowed.
- A player dies when at least two players **of the same enemy team** stand on
  the 8 tiles around it. It checks this at the start of its own turn, before
  moving, and then leaves the board.
- A team wins when it is the only team left on the board.
- If no team on the board has two players any more, nobody can kill, so the
  game ends in a draw.
- There is no fixed turn order. The semaphore decides who plays next, so a
  player can sometimes play twice in a row.

## How it works

All three IPC objects use the same key, `0x4c454d49`.

| Object | Purpose |
| --- | --- |
| Shared memory | The board (`0` means an empty tile, otherwise the tile holds a team number), plus the game state: countdown, running flag and winner. |
| Semaphore 0 | A mutex around the board. A player locks it, plays one turn and unlocks it. |
| Semaphore 1 | The number of players currently in the game. |
| Message queue | One message per team, carrying the position of the enemy that team is attacking. |

**Creating and joining.** The first process to create the semaphore set opens
the game. New semaphores start at 0, so anyone who arrives at the same moment
waits until the board is ready.

**Leaving.** A player locks the board, clears its tile and lowers the player
count. If the count reaches 0, that player was the last one, and it removes the
shared memory, the semaphores and the message queue.

**Crash safety.** Both semaphores use `SEM_UNDO`. If a player is killed while
holding the lock, the kernel releases the lock and corrects the player count.
If a new player finds a game with a player count of 0, it treats the board as
leftovers and resets it.

**Viewer.** `./lemipc -v` attaches to the shared memory in read-only mode. It
never creates a game and never changes one. Because players write to the board
while it reads, it still takes the lock to copy each frame.

## Team AI

The message queue keeps each team's shared target:

1. On its turn, a player reads its team's message to learn the target. If no
   target exists yet, or the old one is gone, it picks the closest enemy
   instead.
2. It then posts the target back so the next teammate can read it, because
   reading a message removes it from the queue.

Two attackers are enough to kill, so only the two teammates closest to the
target chase it. The others go after the nearest other enemy.

Players find their way with a breadth-first search. They avoid tiles where they
would be surrounded, and they hold their position once they are next to their
target.

## Configuration

All tuning values are `#define`s in [includes/lemipc.h](includes/lemipc.h):

| Define | Default | Meaning |
| --- | --- | --- |
| `BOARD_W`, `BOARD_H` | 20, 12 | Board size |
| `TEAM_MAX` | 99 | Highest team number allowed |
| `TURN_US` | 150000 | Pause between two turns of a player (µs) |
| `START_DELAY` | 3 | Countdown before the fight starts (s) |
| `VIEW_US` | 100000 | Viewer refresh interval (µs) |
| `IPC_KEY` | `0x4c454d49` | Key shared by all IPC objects |

Run `make re` after changing any of them.

## Project structure

```
includes/lemipc.h   defines, shared structures, prototypes
srcs/main.c         argument parsing, signals, player/viewer dispatch
srcs/ipc.c          create/join/leave the game, lock, cleanup
srcs/board.c        board helpers, death check, team stats, game phase
srcs/comms.c        team target protocol over the message queue
srcs/ai.c           target choice and BFS movement
srcs/player.c       player turn loop
srcs/viewer.c       read-only ANSI viewer
srcs/utils.c        output buffer and error messages
libft/              libft (git submodule)
test.sh             quick test: viewer + players + IPC cleanup check
```

## Known limitations

- A player killed with `kill -9` leaves its tile on the board, and no process
  can remove it. If that team would otherwise lose, the game never ends. Stop
  the remaining players with `Ctrl-C`.
- A game with only one player per team never starts, because no one could ever
  kill.
- The project targets Linux. It relies on new semaphores starting at 0, which
  Linux guarantees but POSIX does not.

## How to contribute

1. Fork the repository and create a branch for your change.
2. Keep the build warning-free: the project compiles with
   `-Wall -Wextra -Werror`.
3. Before opening a pull request, check that:
   - a full game runs to a winner with the viewer open;
   - `ipcs` shows nothing left over afterwards;
   - `valgrind --leak-check=full ./lemipc <team>` reports no leaks or errors;
   - `Ctrl-C` during a game still cleans everything up.
4. Open a pull request that explains what you changed and how you tested it.
