"""One module per pathfinding algorithm.

Importing this package is what puts the solvers in the registry, so
``MazeSolver.from_config`` can find them. Adding one is a module here
and a line below.

BFS is the one that writes the output file, and the ground truth the
others are checked against: same route length, on every maze, for
every seed -- ``tests/test_astar.py`` is that check for A*. Which
algorithms were picked, and why, is set out in
``docs/2026-09-04-ALGORITHM-CHOICES.md``.
"""

from .bfs import BFS
from .astar import BidirectionalAStar

__all__ = ["BFS", "BidirectionalAStar"]
