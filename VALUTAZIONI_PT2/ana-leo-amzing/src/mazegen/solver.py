"""Shortest path between two cells."""
from abc import ABC, abstractmethod
from typing import Any

from .config import MazeConfig
from .maze import Cell, Maze, Path, Paths


class MazeSolver(ABC):

    algorithms: dict[str, type["MazeSolver"]] = {}
    name: str = ""
    default: str = "bfs"
    maze: Maze
    _path: list[Cell] = []

    # -----------------------------------------------------------------
    # Constructors and class lifecycle

    def __init__(self, config: MazeConfig) -> None:
        self.config = config

    def __init_subclass__(cls) -> None:
        if cls.name:
            MazeSolver.algorithms[cls.name] = cls

    @abstractmethod
    def get_visited(self) -> Paths:
        """The sequences of cells an algorithm explored."""
        pass

    def get_shortest_path(self) -> Path:
        """The shortest path from the entry to the exit, if any."""
        return self._path

    @classmethod
    def from_config(cls, config: MazeConfig) -> "MazeSolver":
        chosen = (config.solve_algorithm if config.solve_algorithm is not None
                  else cls.default)
        chosen = chosen.strip().lower()
        if chosen not in cls.algorithms:
            known = ", ".join(sorted(cls.algorithms)) or "none registered"
            raise ValueError(
                f"no solver algorithm called {config.solve_algorithm!r}; "
                f"there is {known}"
            )
        return cls.algorithms[chosen](config)

    def solve(self, maze: Maze) -> Path:
        self.maze = maze
        self._initstruct()
        while self._step() == 0:
            pass
        return self._path

    @abstractmethod
    def _initstruct(self) -> None:
        """Initialize the data structures needed for the algorithm."""

    @abstractmethod
    def _step(self) -> Any | Path:
        """Perform one step of the algorithm, yielding the curent state."""
