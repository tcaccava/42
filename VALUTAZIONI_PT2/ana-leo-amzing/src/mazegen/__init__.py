"""Reusable maze generator."""

from . import forty_two
from .config import MazeConfig
from .generator import MazeGenerator
from .solver import MazeSolver
from .maze import Cell, Cells, Direction, Edge, Maze, Path, Paths

# Imported for the side effect: it is what registers the algorithms,
# so ``MazeGenerator.from_config`` and ``MazeSolver.from_config``
# answer to them. Last, because the modules in them import the two
# base classes from above.
from . import generators  # noqa: F401,E402  (registration, not use)
from . import solvers  # noqa: F401,E402  (registration, not use)

__all__ = ["Cell", "Cells", "Direction", "Edge", "Maze",
           "Path", "Paths",
           "MazeConfig", "MazeGenerator", "MazeSolver",
           "forty_two"]
