*This project has been created as part of the 42 curriculum by anade-mo, ltamiano.*

# A-Maze-ing

## Description

A-Maze-ing generates mazes, draws them in the terminal, and computes
the shortest path through them.

The program reads a `KEY=VALUE` configuration file, generates a maze
of the requested size with the requested algorithm, and writes a file
containing information on the generated maze as well as the the
shortest path found. Every maze large enough to hold it carries a
block of fully closed cells spelling **42**. A maze too small to fit
the pattern is refused with a message naming the smallest size that
works.

The maze generator itself is a standalone, pip-installable package
(`mazegen`) with no dependencies, so a later project can reuse it
without taking any of this program's terminal code with it.

## Instructions

To run the project it is required to have python 3.10 or newer. Run
the following makefile commands

```bash
make run
```

You can also run the project directly as follows

```bash
. .venv/bin/activate
python3 a_maze_ing.py config.txt
```

Keyboard controls

| Key | Action |
|---|---|
| `R` | generate and draw a new maze |
| `P` | show or hide the shortest path from entry to exit |
| `C` | open the colour menu, then `1`–`5` to pick a wall colour |
| `A` | turn the generation and search animations on or off |
| `Q` | quit (`Esc` and `Ctrl-C` also work) |


## Configuration file

The configuration files supports the mandatory keys such as

  WIDTH, HEIGHT, ENTRY, EXIT, OUTPUT_FILE, PERFECT, SEED

And we also support the following optional keys

  GEN_ALGORITHM, SOLVE_ALGORITHM, FPS

## Maze Generation Algorithms

For the maze generation algorithm we have developed the following three different algorithm:

- Randomised Kruskal
- Randomised backtracker
- Recursive division

The algorithm choice was made by taking into consideration both efficiency as well as the aesthetics of the generated maze.

## Shortest Path Algorithms

For the maze generation algorithm we have developed the following three different algorithm:

- Breadth First Search (BFS)
- Bidirectional A*

The algorithm choice was made by taking into consideration both efficiency as well as the aesthetics of the generated path.

## The mazegen module

The library can be used to generate a maze structure and compute the
shortest path and it is independent from the app graphical aspect. It
can be installed as follows.

```bash
python -m pip install --quiet build
python -m build
pip3 install ./dist/mazegen-1.0.0-py3-none-any.whl
```

An example of using the library.

```python
from mazegen import MazeConfig, MazeGenerator, MazeSolver

config = MazeConfig(20, 15, (0, 0), (19, 14), seed=7,
                    gen_algorithm="kruskal", solve_algorithm="bfs")
maze = MazeGenerator.from_config(config).generate()
path = MazeSolver.from_config(config).solve(maze)

print(maze.get_dim(), maze.get_entry(), maze.get_exit(), len(path))
maze.save("maze.txt", path)
```

## Advanced features

- Rendering for both generation and solving animation
- Multiple algorithm for generation and solving

## Team and project management

The development happened through GitHub with a branch-based development where separate branches were used for features and bugfixes and pull requests with code reviews were done in order to merge the changes in the main branch.

The efforts were divided as follows:

- anade-mo was responsible for the configuration parsing, terminal rendering, helped with the maze implementation and the solver abstract class and developed the backtracker and recursive division generation algorithm as well as helped with the development of the BFS solver algorithm.

- ltamiano was responsible for developing the maze, the generator abstract class as well as the solver abstract class and developed the kruskal generation algorithmn and the BFS and the bidirectional A* solver algorithm.

Everything else required by the project was developed together.

### How AI was used

AI was used as a support tool for the following tasks

- **Algorithm visualizer**

  A browser-based tool was developed to compare generators and
  pathfinders side by side while the portfolio was being chosen
  
- **Explaining algorithms while studying them**

  AI was used to study and understand the different algorithms.
  
- **Review and checking**

  AI was used to test out the written code to make sure it was valid.

- **Aid for code writing**

  AI was used to help the writing of code for the terminal rendering portion of the Maze.

  ## Resources

- https://en.wikipedia.org/wiki/Breadth-first_search
- https://en.wikipedia.org/wiki/Bidirectional_search
- https://it.wikipedia.org/wiki/Algoritmo_di_Kruskal
- https://en.wikipedia.org/wiki/Maze_generation_algorithm