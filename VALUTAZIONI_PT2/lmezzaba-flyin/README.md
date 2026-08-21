*This project has been created as part of the 42 curriculum by lmezzaba.*

# Fly-in — Drone Fleet Routing Simulator

## Description

Fly-in routes a fleet of autonomous drones from a **start hub** to an
**end hub** across a network of connected zones, in the fewest possible
simulation turns, while respecting every constraint of the subject:

- **movement costs** driven by the zone type — `normal` and `priority`
  cost 1 turn, `restricted` costs 2 turns, `blocked` can never be
  entered;
- **zone occupancy limits** (`max_drones`, default 1), with the start
  and end hubs explicitly exempt;
- **connection capacity limits** (`max_link_capacity`, default 1);
- **simultaneous movement**, strategic waiting and conflict avoidance;
- the exact **output format** `D<id>-<zone>` / `D<id>-<connection>`,
  one line per turn, stationary drones omitted.

The whole project is written in **pure Python 3.10+ standard library**.
The graph and every algorithm are implemented from scratch: **no
`networkx`, no `graphlib`**, no external graph or pathfinding helper is
imported anywhere. The design is fully **object-oriented** (one
responsibility per class, encapsulated state exposed through read-only
properties) and fully **type-annotated** (`mypy --strict` clean).

## Instructions

Requirements: **Python 3.10 or later** and nothing else. The simulator
imports the standard library only, so a fresh clone runs immediately:

```bash
./main.py maps/easy/01_linear_path.txt
```

Dependencies are managed with [uv](https://docs.astral.sh/uv/);
`make install` creates `.venv` and installs the tooling, falling back to
`python3 -m venv` + `pip` when `uv` is absent. Because the program
itself has no dependencies, `make run`, `make debug` and
`make benchmark` use a plain interpreter and **never touch the
network**. Only `make lint`, `make lint-strict` and `make animate` need
installed packages.

```bash
make install                                  # flake8, mypy (+ pygame)
make run                                      # default map
make run MAP=maps/hard/01_maze_nightmare.txt
make run MAP=maps/medium/02_circular_loop.txt ARGS="--quiet"
make animate MAP=maps/hard/02_capacity_hell.txt SPEED=400
make debug MAP=maps/easy/02_simple_fork.txt   # under pdb
make lint                                     # flake8 . + mypy . (subject flags)
make lint-strict                              # flake8 . + mypy . --strict
make benchmark                                # turn count of every map
make clean                                    # remove caches
```

### Running without `make`

```bash
./main.py maps/easy/01_linear_path.txt
python3 main.py maps/easy/01_linear_path.txt --quiet
python3 main.py --max-paths 6 maps/medium/01_dead_end_trap.txt

# the animation is the only feature needing an installed package
.venv/bin/python main.py maps/hard/02_capacity_hell.txt --animate
uv run --extra viz python main.py maps/hard/02_capacity_hell.txt --animate
```

If `pygame` is missing, `--animate` says so **before** running the
simulation and exits with code `1`; the colored terminal output, which
is the visualization required by the subject, works regardless.

### Command-line options

| Option | Effect |
|---|---|
| `--quiet` | Print only the raw turn lines, i.e. the exact output format of the subject. |
| `--no-color` | Disable every ANSI escape sequence (plain terminals, log files). |
| `--max-paths N` | Alternative routes pre-computed for load distribution (default: 4). |
| `--animate` | Open a pygame window replaying the flight (requires `pygame`). |
| `--speed MS` | Milliseconds per turn in the `--animate` window (default: 600). |

The program exits with `0` on success, `1` on any handled error and
`130` on `Ctrl-C`. Every error is reported on `stderr` with a clear
message and, for parsing errors, the offending line number. The program
never terminates on an unhandled exception.

## Project structure

```
main.py                 Executable entry point (./main.py <map>)
Makefile                install / run / animate / debug / lint / benchmark / clean
pyproject.toml          uv project definition + mypy configuration
setup.cfg               flake8 configuration
src/
  cli.py                Argument parsing, error handling, wiring
  exceptions.py         FlyInError hierarchy
  models/               ZoneType, Zone, Connection, Drone, Network
  parsing/              MapParser (syntax + semantic validation)
  pathfinding/          Dijkstra, RoutePlanner (routes + fleet sharing)
  simulation/           Simulator, SimulationResult
  visualization/        ColorPalette, TerminalRenderer, DroneAnimator
maps/
  easy|medium|hard|challenger/   Maps provided with the subject
  custom/                        Extra edge-case maps
  custom/errors/                 Deliberately invalid maps
```

## Algorithm and implementation strategy

### 1. Parsing

`MapParser` reads the file with plain string operations — `partition`
and `split`, **no regular expression**. Every line has the same shape,
`keyword: body [metadata]`, so parsing is three steps: split the
keyword, peel off the optional brackets, then hand the body to one small
handler per keyword.

It refuses anything the subject forbids: a missing or duplicated
`nb_drones`, a `nb_drones` that is not the first instruction, a
missing/duplicated hub, a duplicate zone name, a duplicate connection
(`a-b` and `b-a` are the same edge), a connection referring to an
undefined zone, a dash or a space in a zone name, a non-integer
coordinate, a malformed or unknown metadata tag, an invalid zone type, a
non-positive capacity, a blocked hub, or a file that is not valid UTF-8.
Each failure raises a `MapParsingError` carrying the line number and the
cause. As the subject specifies, `max_drones` on the two hubs is parsed
but **ignored**, not rejected.

### 2. Pathfinding — custom Dijkstra

`src/pathfinding/dijkstra.py` implements Dijkstra from scratch on top of
the standard-library binary heap (`heapq`). The cost of a move is the
movement cost of the zone being *entered*, so `restricted` zones are
naturally twice as expensive; `blocked` zones are never expanded, so no
produced path can cross one. Each heap entry carries a second key, the
number of non-priority hops, used as a tie-breaker: at equal cost the
route using more `priority` zones wins, which is how the subject's
"priority zones should be preferred" requirement is satisfied.

**Complexity:** `O((V + E) log V)` time, `O(V)` memory, per path.

### 3. Route planning

`RoutePlanner` answers a single question — *which drone flies where* —
in two halves.

First it derives **alternative routes** by forbidding, one at a time, an
intermediate zone of the best route and running Dijkstra again: a
simplified variant of Yen's k-shortest-paths algorithm. It keeps the
`--max-paths` cheapest distinct routes. Complexity `O(R · (V + E) log
V)` with `R = max_paths` (4 by default), computed **once** before the
simulation starts.

Then it **shares the fleet** between them. It measures each route's
bottleneck capacity — the smallest `max_drones` / `max_link_capacity`
found along it — and hands the next drone to whichever route is
currently least used *relative to its own bottleneck*. A wide route
therefore receives proportionally more drones than a narrow one, which
maximises throughput per turn instead of queueing the whole fleet on the
single cheapest corridor.

### 4. Turn engine

`Simulator.run()` advances every active drone by **exactly one** step,
or a wait, per turn — never more. Each turn is played in two phases:

- **Phase 1 — drones already in flight.** A drone crossing a connection
  towards a `restricted` zone ticks down and lands; it can never linger
  mid-air, exactly as the subject requires. It is flagged as *having
  already acted*, so it cannot also take a fresh move later in the same
  turn. Its connection stays occupied for the **whole** two-turn
  crossing: the slot is released only once the arrival turn has been
  fully processed.
- **Phase 2 — drones parked in a zone.** Drones are processed
  *closest-to-the-goal first*, so the ones about to land free their
  bottleneck slot before their followers ask for it — this implements
  the subject's "drones moving out of a zone free up capacity for that
  same turn" rule. A move is committed only if the destination zone
  **and** the connection both have free capacity at that instant. When
  entering a `restricted` zone the destination is booked immediately,
  since the drone is not allowed to wait on the link.
- **Opportunistic rerouting.** If a drone's own next hop is blocked this
  turn, it checks — cheapest remaining cost first — whether one of the
  *other pre-computed* candidate routes offers a free step out of its
  current zone, and splices itself onto that route's suffix. Only
  already-validated Dijkstra routes are ever considered, so rerouting can
  never invent an illegal edge; a drone that finds nothing free performs
  a strategic wait. **No pathfinding is ever re-run at simulation time.**
- **Deadlock detection.** A turn in which no drone moves can only repeat
  forever, so the engine stops immediately with a clear
  `SimulationError`. A secondary `max_turns` safety net guards against
  pathological maps.

**Complexity:** `O(turns · nb_drones · P)` with `P` the small, bounded
number of candidate routes. Every capacity check is `O(1)` (dictionary
lookups; connections are indexed by their unordered endpoint pair), and
memory stays `O(V + E + nb_drones)`.

### Trade-offs

Computing routes once and rerouting only among them keeps the engine
fast and predictable, at the price of not being provably optimal: a full
time-expanded flow formulation would give the true minimum makespan but
would cost orders of magnitude more time and memory. The chosen design
meets or beats every reference target of the subject while routing 1000
drones through the hardest map in a few seconds.

## Visual representation

**Colored terminal output (default).** `TerminalRenderer` prints a
header with the network size, the start/end hubs and a **legend of every
zone**, each rendered in the color declared in the map file and prefixed
by a glyph for its type (`.` normal, `+` priority, `!` restricted, `x`
blocked) with its capacity in parentheses. Each turn is then printed on
its own line, with every destination colorized with that zone's own
color, so the reader instantly sees whether a drone is entering a fast
lane, a slow restricted zone or a bottleneck. Each turn line ends with
the **live zone states** — every occupied zone with its `used/max`
occupancy, e.g. `[bottleneck 2/2, wide_area 2/3]` — which turns the move
log into a readable picture of where the fleet is congested: a zone
sitting at `2/2` for several turns is exactly the bottleneck that sets
the total turn count. A closing summary reports the total number of
turns (the score), the total path cost, the average number of moves per
turn and the average number of turns per drone. `--no-color` strips
every escape sequence; `--quiet` prints only the raw turn lines.

**Graphical animation (`--animate`).** `DroneAnimator` records one
lightweight snapshot of the fleet per turn during the run, then replays
it in a pygame window: zones are drawn as discs at their real map
coordinates, colored with their declared color, edges show their
capacity when it is above 1, and drones move smoothly between two turns,
including *along the connection* while crossing a restricted zone. Each
drone gets a distinct, stable color and a small deterministic offset so
several drones sharing a zone stay individually readable. In the window:
`space` pauses, `up`/`down` change the speed, `r` restarts and
`esc`/`q` quits. Together the two views turn an abstract list of tokens
into something you can watch and reason about — bottlenecks, waiting
queues and route splits become immediately obvious.

`pygame` is the only third-party package used, it is **optional**, and
it is imported lazily so the simulator runs without it. `make install`
installs it on a best-effort basis (hence the `-` prefix in the
Makefile): pygame publishes binary wheels only for some Python versions,
so if it cannot be built, `make install` still succeeds and every other
rule keeps working. For the same reason `--animate` degrades instead of
failing: if the window cannot be opened it reports the problem on
`stderr`, and the simulation output is unaffected.

Text labels are loaded by a chain of three loaders, first one wins:

1. `pygame.font.SysFont` by family name — `ubuntumono`,
   `dejavusansmono`, `liberationmono`, `freemono`, the monospace fonts
   shipped with Ubuntu;
2. `pygame.font.Font` on the font *file*, for when the family lookup is
   unavailable (it goes through `pygame.sysfont`);
3. `pygame._freetype.Font` on the same file.

The third exists because of [pygame issue
#4795](https://github.com/pygame/pygame/issues/4795): on Python 3.14,
`pygame/sysfont.py` imports `Font` from `pygame.font` while
`pygame/font.py` imports from `pygame.sysfont`, and `pygame/freetype.py`
imports from `pygame.sysfont` too, so all three Python-level modules
fail with a circular `ImportError`. `pygame._freetype` is the compiled
extension that `pygame.freetype` re-exports; being C, it imports none of
them and still loads. `FreetypeFont` adapts its `render()` — which
returns a `(surface, rect)` pair — to the interface of the other two, so
the drawing code never knows which loader won. If all three fail, the
zones and drones are still drawn, withAI assistance was used as a reviewer and an accelerator, never as a
substitute for understanding — every suggestion was read, tested and
rewritten where needed, and the whole codebase can be explained line by
line. Concretely, AI was used for:

- **reviewing the turn-scheduling rules** against the subject, which is
  how two real bugs were found in an earlier version: a drone landing
  from a `restricted` transit could take a second move in the same turn,
  and the connection it was landing on was released one turn too early.
  Both silently deflated the turn count and both are fixed;
- **auditing edge cases and error handling** — the invalid maps in
  `maps/custom/errors/` and the crash cases that are now handled
  (non-UTF-8 input, non-positive `--max-paths`, `Ctrl-C`, deadlock) came
  out of that review;
- **rubber-ducking the complexity analysis** and the wording of this
  README;
- **boilerplate**: docstring phrasing and repetitive error messages.

AI was **not** used to generate the algorithms themselves: the Dijkstra
variant with its priority tie-breaker, the bottleneck-weighted fleet
distribution and the two-phase turn engine are the project's own design
choices, and the trade-offs behind them are documented above.out their names.

## Example

Input — `maps/easy/01_linear_path.txt`:

```
# Easy Level 1: Simple linear path
nb_drones: 2

start_hub: start 0 0 [color=green]
hub: waypoint1 1 0 [color=blue]
hub: waypoint2 2 0 [color=blue]
end_hub: goal 3 0 [color=red]

connection: start-waypoint1
connection: waypoint1-waypoint2
connection: waypoint2-goal
```

Command and expected output:

```
$ ./main.py maps/easy/01_linear_path.txt --quiet
D1-waypoint1
D1-waypoint2 D2-waypoint1
D1-goal D2-waypoint2
D2-goal
```

Solved in **4 turns** (target: ≤ 6). The same run with the full visual
output:

```
$ ./main.py maps/easy/01_linear_path.txt
=== Fly-in drone routing simulation ===
Zones: 4 | Connections: 3 | Drones: 2
Start: start   ->   End: goal
Zones (. normal  + priority  ! restricted  x blocked):
  . start(inf)  . waypoint1(1)  . waypoint2(1)  . goal(inf)

Turn   1: D1-waypoint1   [waypoint1 1/1]
Turn   2: D1-waypoint2 D2-waypoint1   [waypoint1 1/1, waypoint2 1/1]
Turn   3: D1-goal D2-waypoint2   [waypoint2 1/1]
Turn   4: D2-goal

=== Summary ===
Total turns (score) : 4
Drones delivered    : 2
Total path cost     : 6
Moves per turn      : 1.50
Average turns/drone : 3.00
```

A drone crossing a `restricted` zone reports the connection during the
first turn and the zone during the second, as required:

```
$ ./main.py maps/custom/03_restricted_chain.txt --quiet
D1-base-slow1 D2-base-slow1
D1-slow1 D2-slow1
D1-slow1-slow2 D2-slow1-slow2 D3-base-slow1
...
```

## Results

Every mandatory reference target of the subject is met or beaten
(`make benchmark` reproduces the whole table).

| Map | Drones | Turns | Target |
|---|:---:|:---:|:---:|
| `easy/01_linear_path` | 2 | **4** | ≤ 6 |
| `easy/02_simple_fork` | 4 | **4** | ≤ 8 |
| `easy/03_basic_capacity` | 4 | **4** | ≤ 6 |
| `medium/01_dead_end_trap` | 5 | **8** | ≤ 12 |
| `medium/02_circular_loop` | 6 | **15** | ≤ 15 |
| `medium/03_priority_puzzle` | 5 | **7** | ≤ 12 |
| `hard/01_maze_nightmare` | 8 | **13** | ≤ 30 |
| `hard/02_capacity_hell` | 12 | **16** | ≤ 35 |
| `hard/03_ultimate_challenge` | 15 | **27** | ≤ 45 |
| `challenger/01_the_impossible_dream` | 25 | 51 | ref. 45 (optional) |

`medium/02_circular_loop` lands exactly on its target because that map
funnels its six drones through a single capacity-1 `restricted` link:
15 turns is the arithmetic minimum there, so no scheduling change can
beat it. The optional challenger map is solved but does not beat the
45-turn reference record; that level does not affect the grade.

**Scalability.** The same engine routes 1000 drones through
`hard/03_ultimate_challenge` in roughly 4.5 seconds, since the routes
are computed once and every per-turn decision is a dictionary lookup.

## Extra maps

As recommended by the subject, extra map files were written on top of
the ones provided, to exercise edge cases and error handling.

`maps/custom/` holds seven edge-case maps: a single drone, a blocked
zone forcing a detour, a chain of `restricted` zones, a hard
single-slot bottleneck, a priority-versus-normal tie, very large
capacities, and a direct hub-to-hub link.

`maps/custom/errors/` holds twelve deliberately invalid maps, one per
documented error case: missing `nb_drones`, invalid zone type, missing
`start_hub`, missing `end_hub`, invalid capacity, duplicate zone,
duplicate connection, unknown zone in a connection, dash in a zone
name, disconnected graph, malformed metadata and invalid link capacity.
Each stops the program with a clear message and the offending line
number.

```bash
for m in maps/custom/errors/*.txt; do ./main.py "$m" --quiet; done
```

## Resources

- *Introduction to Algorithms* (Cormen, Leiserson, Rivest, Stein) —
  chapters on graph representations, shortest paths (Dijkstra) and
  network flow, used as the theoretical basis for the pathfinding and
  the capacity-aware scheduling.
- Jin Y. Yen, *Finding the K Shortest Loopless Paths in a Network* —
  inspiration for the simplified alternative-route search in
  `RoutePlanner`.
- Python documentation:
  [`heapq`](https://docs.python.org/3/library/heapq.html) for the
  priority queue,
  [`argparse`](https://docs.python.org/3/library/argparse.html) for the
  CLI, [`enum`](https://docs.python.org/3/library/enum.html) and
  [`typing`](https://docs.python.org/3/library/typing.html) for the
  type-safe object model.
- [PEP 8](https://peps.python.org/pep-0008/) and
  [PEP 257](https://peps.python.org/pep-0257/) for the coding and
  docstring conventions enforced by `flake8`.
- [mypy](https://mypy.readthedocs.io/),
  [uv](https://docs.astral.sh/uv/) and
  [pygame](https://www.pygame.org/docs/) documentation.

### AI usage disclosure

- Explaining Dijkstra
- Writing README.md file
- Writing the docstrings
- formatting for mypy and flak8 compilance
- solving pygames compatibility issues
- writing the Makefile
