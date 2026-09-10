"""Core abstractions required to generate mazes.

MazeGenerator is an abstract class that implements the core structure
that is shared across all maze generators. To implement a specific
generator you need to extend the abstract class and implement the
_blank() and _run() methods.
"""

import random
from abc import ABC, abstractmethod
from collections.abc import Iterator

from . import forty_two
from .config import MazeConfig
from .maze import Cell, Cells, Direction, Edge, Maze


class MazeGenerator(ABC):
    """Generates mazes. Public class of the package.

    The class implements a generator for mazes with the following core
    methods:

    - carve()

      yields the maze after every update of the algorithm used. This
      can be used to render an animation of the maze frame by
      frame. Notice that the maze is changed in-place, which means
      that when we generate the next version the previous one no
      longer exists.

    - generate()

      gives directly the final maze by consuming all updates of
      carve() until we reach the end of the generator. The last maze
      is the final one.

    This class should then be extended by different subclasses, where
    each subclass should implement a single algorithm. In practice
    this is done by implementing the following abstract methods:

    - _blank()

       The initial state of the grid the algorithm starts from. For
       some algorithms it will be no walls, for others it will be full
       walls.

    - _run()

       The actual algorithm itself.

    """

    # Every subclass that names itself is selectable by that name, so
    # adding an algorithm is writing a class and nothing else.
    algorithms: dict[str, type["MazeGenerator"]] = {}
    name: str = ""

    # Kruskal's while it is the only one written; the backtracker takes
    # this back when it lands, being the one that animates best.
    default: str = "kruskal"

    # This set contains the cells that belong to the 42 pattern and
    # that must not be modified by the generation algorithm. The walls
    # along side these cells must always be up.
    frozen: Cells = frozenset()

    # The maze being built. This one we just declare as it doesn't
    # exist yet. It is created during the call to carve().
    maze: Maze

    # -----------------------------------------------------------------
    # Constructors and class lifecycle

    def __init__(self, config: MazeConfig) -> None:
        self.config = config

    def __init_subclass__(cls) -> None:
        """This is called when a subclass of MazeGenerator is
        defined. That is, we don't need to istantiate the
        subclass. When the python interpreter reads the definition of
        the subclass, this method will execute.

        If the subclass sets a class attribute called `name`, then
        such value is stored into the shared dictionary algorithms.
        """
        if cls.name:
            MazeGenerator.algorithms[cls.name] = cls

    @classmethod
    def from_config(cls, config: MazeConfig) -> "MazeGenerator":
        """Factory that builds a specific MazeGeneration.

        If algorithm is None then the default generator is built. This
        is decided by the value of the `default` class attribute.
        """
        chosen = (config.gen_algorithm if config.gen_algorithm is not None
                  else cls.default)
        chosen = chosen.strip().lower()
        if chosen not in cls.algorithms:
            known = ", ".join(sorted(cls.algorithms)) or "none registered"
            raise ValueError(
                f"no generation algorithm called {config.gen_algorithm!r}; "
                f"there is {known}"
            )
        return cls.algorithms[chosen](config)

    # -----------------------------------------------------------------
    # Public APIs

    def generate(self) -> Maze:
        """exhaust all the generation steps in order to arrive at the
        final maze and return it
        """
        for _ in self.carve():
            pass
        return self.maze

    def carve(self) -> Iterator[Maze]:
        """Yield the maze after each change, from blank to finished.
        This is useful when animating the generation.
        """
        if self.config.seed is not None:
            random.seed(self.config.seed)

        self.maze = self._blank()

        # select the frozen cells that must not be modified by the
        # algorithm and that form the 42 pattern. This is done BEFORE
        # the algorithm execution itself.
        self.frozen = forty_two.reserve(self.maze)

        yield self.maze
        yield from self._run()
        if not self.config.perfect:
            yield from self._braid()
            yield from self._loosen()
        self.maze.set_entry(self.config.entry_cell)
        self.maze.set_exit(self.config.exit_cell)
        yield self.maze

    # -----------------------------------------------------------------
    # These are the methods that the subclasses will implement. It's
    # where the different algorithms comes in.

    @abstractmethod
    def _run(self) -> Iterator[Maze]:
        """The algorithm itself, yielding after each wall it changes."""

    def _blank(self) -> Maze:
        """The grid the algorithm starts from.

        Most subclasses will inherit a starting poting where every
        wall is standing. Recursive division instead overrides this
        default behavior. It will start from zero walls built.
        """
        return Maze(self.config.width, self.config.height)

    def _free_walls(self) -> list[tuple[Cell, Direction]]:
        """The internal walls an algorithm is allowed to open.

        ``internal_walls`` with the "42" left out. Every generator
        wants this rather than the full list, so it is written here
        once instead of in each of them.
        """
        return [(cell, side) for cell, side in self.maze.internal_walls()
                if not self._touches_42(cell, side)]

    def _touches_42(self, cell: Cell, side: Direction) -> bool:
        """Does that wall have a cell of the "42" on either side of it?

        Asked from both sides, because a wall belongs to two cells and
        only one of them needs to be part of the shape for the wall to
        be one that has to stay standing.
        """
        col, row = cell
        if cell in self.frozen:
            return True
        other = self.maze.across(col, row, side)
        return other in self.maze and other in self.frozen

    # -----------------------------------------------------------------
    # These two methods are used when PERFECT=false.
    #
    # - _braid() opens dead-ends and create new loops
    #
    # - _loosen() makes sure there are at least two paths from entry to exit.

    def _braid(self) -> Iterator[Maze]:
        """Open a wall at each dead-end, so the maze has loops."""
        for cell in self.maze.dead_ends():
            col, row = cell
            shut = [side for side in self.maze.closed_walls(col, row)
                    if not self._touches_42(cell, side)
                    and not self._opens_a_room(cell, side)]
            if shut:
                self.maze.destroy_wall(col, row, random.choice(shut))
                yield self.maze

    def _loosen(self) -> Iterator[Maze]:
        """Open walls until no single one can cut the entry from the exit."""
        while True:
            chokes = self._choke_points()
            if not chokes:
                return
            entry_col, entry_row = self.config.entry_cell
            near = self.maze.region_of(
                entry_col, entry_row, banned=chokes[0])
            across = self._closed_across(near)
            if not across:
                return  # walled in by the border: nothing to open
            safe = [(cell, side) for cell, side in across
                    if not self._opens_a_room(cell, side)]
            across = safe or across
            (col, row), side = random.choice(across)
            self.maze.destroy_wall(col, row, side)
            yield self.maze

    # -----------------------------------------------------------------
    # Questions about the planned ends

    def _choke_points(self) -> list[Edge]:
        """Passages every route from entry to exit has to use.

        The bridges of the open-passage graph that separate the two
        ends, found in one depth-first walk (Tarjan). """
        maze = self.maze
        entry = self.config.entry_cell
        exit_cell = self.config.exit_cell
        if entry == exit_cell:
            return []

        # disc: when a cell was first reached. low: the earliest disc
        # anything in its subtree can climb back to, along one passage
        # that is not the one it came down.
        disc: dict[Cell, int] = {entry: 0}
        low: dict[Cell, int] = {entry: 0}
        parent: dict[Cell, Cell | None] = {entry: None}
        clock = 1
        chokes: list[Edge] = []
        # Iterative, so a long corridor cannot blow the stack. Each
        # frame keeps the neighbours left to try for that cell.
        stack: list[tuple[Cell, Iterator[tuple[Cell, Direction]]]] = [
            (entry, iter(maze.passages(*entry))),
        ]

        while stack:
            cell, neighbours = stack[-1]
            try:
                other, _side = next(neighbours)
            except StopIteration:
                stack.pop()
                above = parent[cell]
                if above is None:
                    continue
                low[above] = min(low[above], low[cell])
                # Nothing under `cell` reaches above `above`, so the
                # passage between them is a bridge; and everything
                # discovered while `cell` was open is its subtree, so
                # the exit being in that window means the bridge cuts
                # the entry from the exit rather than something else.
                if (low[cell] > disc[above] and exit_cell in disc
                        and disc[cell] <= disc[exit_cell] < clock):
                    chokes.append(frozenset((above, cell)))
                continue
            if other == parent[cell]:
                continue
            if other in disc:
                low[cell] = min(low[cell], disc[other])
                continue
            parent[other] = cell
            disc[other] = low[other] = clock
            clock += 1
            stack.append((other, iter(maze.passages(*other))))
        return chokes

    def _room_at(self, col: int, row: int, gap: Edge) -> bool:
        """Is the 3x3 patch at that corner open throughout?

        ``gap`` is one standing wall to read as though it were already
        open, so a wall can be judged before it is taken down.
        """
        if (col < 0 or row < 0
                or col + 2 >= self.maze.width or row + 2 >= self.maze.height):
            return False
        for step_col in range(3):
            for step_row in range(3):
                here = (col + step_col, row + step_row)
                east = frozenset((here, (here[0] + 1, here[1])))
                south = frozenset((here, (here[0], here[1] + 1)))
                if (step_col < 2 and gap != east
                        and self.maze.get_wall(*here, Direction.E)):
                    return False
                if (step_row < 2 and gap != south
                        and self.maze.get_wall(*here, Direction.S)):
                    return False
        return True

    def _opens_a_room(self, cell: Cell, side: Direction) -> bool:
        """Would opening that wall leave a 3x3 zone with none inside?

        The subject allows no open zone that large. A wall joins two
        cells and nothing else, so the only patches that could turn
        into one are those holding both.
        """
        other = self.maze.across(cell[0], cell[1], side)
        if other not in self.maze:
            return False
        gap = frozenset((cell, other))
        cols = (cell[0], other[0])
        rows = (cell[1], other[1])
        for col in range(max(cols) - 2, min(cols) + 1):
            for row in range(max(rows) - 2, min(rows) + 1):
                if self._room_at(col, row, gap):
                    return True
        return False

    def _closed_across(self, near: Cells
                       ) -> list[tuple[Cell, Direction]]:
        """Walls standing between the cells in ``near`` and the rest."""
        return [((col, row), side)
                for col in range(self.maze.width)
                for row in range(self.maze.height)
                for other, side in self.maze.neighbours(col, row)
                if self.maze.get_wall(col, row, side)
                and not self._touches_42((col, row), side)
                and ((col, row) in near) != (other in near)]
