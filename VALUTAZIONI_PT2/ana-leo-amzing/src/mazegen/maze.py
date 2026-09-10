"""Maze data structure.

This file implements the Maze class, which holds a grid of cells and
the hexadecimal encoding of their walls, one digit per cell, the one
the subject imposes and Direction describes.

The Maze class implements the API required to build a
maze. Specifically given an existing maze it allows to build and
destroy walls at specific locations through the methods build_wall()
and destroy_wall(). The API guarantees that the maze is transformed in
a coherent way as required by the subject.

On top of this class it is possible to build the rest:

- the generation algorithm, which carves a maze by destroying walls
- the solver, which walks the maze looking for the shortest path
- the renderer, which read the maze to draw it
"""

import random
from collections import deque
from collections.abc import Iterable, Iterator, Sequence
from enum import IntFlag

# The names the whole program speaks in. A cell is where it is,
# (col, row); a path is the cells walked through in order; and a maze
# may be drawn with more than one path over it at once. An edge is the
# pair of cells a passage joins, held as a set so that it reads the
# same from either side; Cells is any group of them — a region, or the
# cells set aside for the "42".
#
# They live here, beside the Maze they describe, because everything
# else — the generator, the solver, both renderers — is written in
# terms of them, and a copy in each of those files is a copy that can
# quietly come to mean something else.
Cell = tuple[int, int]
Path = Sequence[Cell]
Paths = Sequence[Path]
Edge = frozenset[Cell]
Cells = frozenset[Cell]


class Direction(IntFlag):
    """Used to represent the side of a cell as well as a specific movement.

    The subject imposes a particular strategy in how walls are encoded.

    For each cell of the grid we have a number that goes from 0 to
    15. The lowest value, 0, means all walls are open, while the
    highest value, 15, means all walls are closed. Specifically each
    bit of the number represents a specific wall. If its value is 1,
    then it is closed, otherwise it is open.

    The specific mapping between bit value and wall position is
    described as follows, where bit positions are given starting from
    the LSB (Least significant bit):

    | bit index | wall direction |
    ------------------------------
    |         0 | North          |
    |         1 | East           |
    |         2 | South          |
    |         3 | West           |

    A few examples to make this more clear:

    - If grid[col][row] = 0

      given that 0 in bit is 0000,
      then in position (col, row) all walls are open

    - If grid[col][row] = 15

      given that 15 in bit is 1111,
      then in position (col, row) all walls are closed

    - If grid[col][row] = 7

      given that 7 in bit is 0111,
      then in position (col, row) the North, East and South
      walls are closed and the West wall is open

    - If grid[col][row] = 9

      given that 9 in bit is 1001,
      then in position (col, row) the North and West walls are
      closed and the East and South walls are open

    """

    N = 0b0001  # 1
    E = 0b0010  # 2
    S = 0b0100  # 4
    W = 0b1000  # 8

    def opposite(self) -> "Direction":
        """Return the same wall, seen from the neighbouring cell.
        Supports a single side only.
        """
        return _OPPOSITE[self]

    def step(self) -> tuple[int, int]:
        """Return the (col, row) offset towards that neighbour.
        Supports a single side only.
        """
        return _STEP[self]

    def letter(self) -> str:
        """Return the letter the output file names this step with.
        Supports a single side only.

        Tabulated rather than taken from the member's own name, so
        that renaming one cannot change what lands in the file.
        """
        return _LETTER[self]


_OPPOSITE: dict[Direction, Direction] = {
    Direction.N: Direction.S,
    Direction.E: Direction.W,
    Direction.S: Direction.N,
    Direction.W: Direction.E,
}

_STEP: dict[Direction, tuple[int, int]] = {
    Direction.N: (0, -1),
    Direction.E: (1, 0),
    Direction.S: (0, 1),
    Direction.W: (-1, 0),
}

_LETTER: dict[Direction, str] = {
    Direction.N: "N",
    Direction.E: "E",
    Direction.S: "S",
    Direction.W: "W",
}

# _STEP read backwards: which side a move goes through. Writing a
# route out means naming that side for each pair of cells in it.
_SIDE_OF: dict[tuple[int, int], Direction] = {
    step: side for side, step in _STEP.items()
}

# To encode a cell with all walls we use the number 15, which is
# written in binary as 1111. We represent this value explicitly to
# avoid the usage of magic numbers.
ALL_WALLS = Direction.N | Direction.E | Direction.S | Direction.W


class Maze:
    """A grid of cells, each with 0 to 4 walls, and the two ends.

    Entry and exit belong here rather than to whoever draws or solves
    the maze: the subject's own file format writes them in the footer
    beside the grid, and the solver is asked for the way between them.
    Kept in one place, the three of them cannot disagree.

    Both are optional. A maze being carved has no ends yet, and a maze
    that is only being looked at may never need any.
    """

    def __init__(self, width: int, height: int,
                 entry: Cell | None = None,
                 exit: Cell | None = None) -> None:
        """initialize each element of the grid with the value ALL_WALLS

        The ends are optional and default to nothing, so every caller
        that only wants a grid keeps working unchanged.
        """
        if width < 1 or height < 1:
            raise ValueError(
                f"a maze needs at least one cell, got {width}x{height}"
            )
        self.width = width
        self.height = height
        self.grid: list[list[Direction]] = []
        for _col in range(width):
            column: list[Direction] = []
            for _row in range(height):
                column.append(ALL_WALLS)
            self.grid.append(column)
        self._entry: Cell | None = None
        self._exit: Cell | None = None
        self._frozen: Cells = frozenset()
        self.set_entry(entry)
        self.set_exit(exit)

    def get_dim(self) -> tuple[int, int]:
        return self.width, self.height

    def get_entry(self) -> Cell | None:
        """Where the maze is entered, or None if it has no entry."""
        return self._entry

    def set_entry(self, cell: Cell | None) -> None:
        """Set where the maze is entered, or None to have no entry."""
        self._entry = self._checked_end(cell, "entry")

    def get_exit(self) -> Cell | None:
        """Where the maze is left, or None if it has no exit."""
        return self._exit

    def set_exit(self, cell: Cell | None) -> None:
        """Set where the maze is left, or None to have no exit."""
        self._exit = self._checked_end(cell, "exit")

    def get_frozen(self) -> Cells:
        """The cells nothing may open, empty if none were set aside.

        These are the fully closed cells the generator sealed before
        it started — the "42" the subject asks for. The walls alone
        cannot say it: a cell with four walls standing is one that was
        set aside here, and one that has simply not been carved yet,
        and whoever draws the maze has to tell those two apart.
        """
        return self._frozen

    def set_frozen(self, cells: Iterable[Cell]) -> None:
        """Set aside the cells nothing may open.

        Taken as any iterable and kept as a frozenset, so that a
        caller may hand over whatever it has and no one can change the
        answer afterwards by changing what they passed.

        A cell off the grid is refused for the same reason an end off
        the grid is: it is a mistake in the caller, and silently
        keeping it would leave a cell nobody can draw.
        """
        chosen = frozenset(cells)
        for cell in chosen:
            if cell not in self:
                raise IndexError(
                    f"frozen cell {cell} falls outside a "
                    f"{self.width}x{self.height} maze"
                )
        self._frozen = chosen

    def _checked_end(self, cell: Cell | None,
                     name: str) -> Cell | None:
        """Refuse an end that is not a cell of this maze.

        An end outside the grid is a mistake in the caller, and one
        worth hearing about where it is made: a solver told to start
        nowhere searches nothing, and a renderer told to mark nowhere
        draws nothing, and neither of them can say why.
        """
        if cell is None:
            return None
        if cell not in self:
            raise IndexError(
                f"{name} {cell} falls outside a "
                f"{self.width}x{self.height} maze"
            )
        return cell

    def __contains__(self, cell: Cell) -> bool:
        """Tell whether the cell belongs to the grid.

        This is the public way to ask the question that _is_inside()
        answers internally, so that it can be used by the generator,
        the solver and the renderer.
        """
        col, row = cell
        return self._is_inside(col, row)

    def _is_inside(self, col: int, row: int) -> bool:
        """Tell whether the cell belongs to the grid."""
        return 0 <= col < self.width and 0 <= row < self.height

    def _check_bounds(self, col: int, row: int) -> None:
        """Refuse a cell that falls outside the grid."""
        if not self._is_inside(col, row):
            raise IndexError(
                f"cell ({col}, {row}) falls outside a "
                f"{self.width}x{self.height} maze"
            )

    def get_wall(self, col: int, row: int, direction: Direction) -> bool:
        """Tell whether the wall on that side is closed."""
        self._check_bounds(col, row)
        #
        # To extract the wall value we use the and-bit operation. If
        # the value is 1 it means wall is up, otherwise wall is
        # down. The cast to bool() is required because otherwise we
        # would obtain a Direction value such as, for example,
        # Direction.N, which is not stricly speaking a 0/1 value.
        #
        # int() on both sides: grid holds Direction, an IntFlag, and
        # ``Direction & Direction`` builds a fresh enum member every
        # time. Plain ints answer the same question far faster, and
        # this is the most asked question in the program.
        return bool(int(self.grid[col][row]) & int(direction))

    def wall_count(self, col: int, row: int) -> int:
        """How many of the cell's four walls are closed, 0 to 4.

        One is a dead-end seen from the other side, three is a
        dead-end, and four is a cell nobody can reach — the shape the
        "42" is drawn in.
        """
        self._check_bounds(col, row)
        return sum(bool(self.grid[col][row] & side) for side in Direction)

    def build_wall(self, col: int, row: int, direction: Direction) -> None:
        """Build a wall, updating the neighbouring cell as well.

        A wall belongs to two cells at once, so both sides are set and
        the grid never says one thing on one side and another on the
        other. On the border there is no neighbour and only the cell
        itself is touched.

        Notice that direction may hold several sides at once.
        For example build_wall(col, row, ALL_WALLS) will close a
        cell completely.
        """
        self._check_bounds(col, row)
        for side in Direction:
            if not side & direction:
                # We skip this side because it is not contained in
                # direction and we go directly to the next side.
                continue
            #
            # build the wall on the requested side
            #
            self.grid[col][row] |= side
            #
            # compute position of neighbour cell to update
            #
            step_col, step_row = side.step()
            neighbour_col, neighbour_row = col + step_col, row + step_row
            #
            # add the new wall to the neighbour cell only if it sits
            # within the grid, and add it to the opposite direction of
            # the starting wall.
            #
            if self._is_inside(neighbour_col, neighbour_row):
                self.grid[neighbour_col][neighbour_row] |= side.opposite()

    def destroy_wall(self, col: int, row: int, direction: Direction) -> None:
        """Destroy a wall, updating the neighbouring cell as well.

        A wall of the external border has no cell behind it and cannot
        be destroyed: the subject requires the border to stay
        closed. Asking for it is a mistake in the caller and so it is
        refused with an exception rather than ignored.

        Exactly as in build_wall(), direction may hold several sides
        at once.

        """
        self._check_bounds(col, row)
        #
        # the sides to open, each with the neighbour cell that shares
        # the wall with (col, row)
        #
        to_open: list[tuple[Direction, int, int]] = []
        for side in Direction:
            if not side & direction:
                # This side is not part of direction, skip to the next.
                continue
            step_col, step_row = side.step()
            neighbour_col, neighbour_row = col + step_col, row + step_row
            if not self._is_inside(neighbour_col, neighbour_row):
                raise ValueError(
                    f"the {side.name} wall of cell ({col}, {row}) is on "
                    f"the border of the maze and cannot be destroyed"
                )
            to_open.append((side, neighbour_col, neighbour_row))
        #
        # nothing can fail from here on
        #
        for side, neighbour_col, neighbour_row in to_open:
            #
            # to actually remove the walls we use the NOT operator (~)
            #
            self.grid[col][row] &= ~side
            self.grid[neighbour_col][neighbour_row] &= ~side.opposite()

    def randomize(self, probability: float = 0.5) -> None:
        """Put every internal wall of the grid up or down at random.

        Each wall is decided on its own: it stands with the given
        probability and is opened otherwise. The walls of the external
        border are never touched, so the invariant that the border stays
        closed keeps holding.

        Call random.seed() beforehand for a repeatable result.
        """
        if not 0.0 <= probability <= 1.0:
            raise ValueError(
                f"probability must be between 0 and 1, got {probability}"
            )
        for (col, row), side in self.internal_walls():
            if random.random() < probability:
                self.build_wall(col, row, side)
            else:
                self.destroy_wall(col, row, side)

    def across(self, col: int, row: int, side: Direction
               ) -> Cell:
        """The cell that side leads to, even if it falls off the grid."""
        self._check_bounds(col, row)
        step_col, step_row = side.step()
        return col + step_col, row + step_row

    def neighbours(self, col: int, row: int
                   ) -> list[tuple[Cell, Direction]]:
        """The cells beside this one, each with the side it lies through.

        Walls are not looked at: these are the cells that exist next to
        this one, whether or not there is a way through.

        The side comes back with the cell because anything that wants
        to *change* the wall between them needs it — ``build_wall`` and
        ``destroy_wall`` are both asked for a direction, and working it
        back out from two cells is the sort of arithmetic that gets
        written five times and wrong once.
        """
        self._check_bounds(col, row)
        cells: list[tuple[Cell, Direction]] = []
        for side in Direction:
            neighbour = self.across(col, row, side)
            if neighbour in self:
                cells.append((neighbour, side))
        return cells

    def closed_walls(self, col: int, row: int) -> list[Direction]:
        """Internal sides of that cell that are still walled.

        The outer border is left out: there is no neighbour there.
        """
        return [side for other, side in self.neighbours(col, row)
                if self.get_wall(col, row, side)]

    def passages(self, col: int, row: int
                 ) -> list[tuple[Cell, Direction]]:
        """The neighbours this cell has a way through to.

        ``neighbours`` with the closed sides left out, so the walls are
        taken into consideration here.

        A cell of the border is never returned, since the walls of the
        external border are always closed. The grid is checked all the
        same, so that the method keeps returning cells that exist even
        if a border wall were opened by mistake.
        """
        return [(cell, side) for cell, side in self.neighbours(col, row)
                if not self.get_wall(col, row, side)]

    def internal_walls(self) -> Iterator[tuple[Cell, Direction]]:
        """Every wall between two cells, given once.

        Only the East and South sides are looked at, so that each wall
        is offered once instead of twice, once from each of the two
        cells that share it. The border is left out: it has no cell
        behind it and never opens.

        This is the order Kruskal's shuffles and the loop ``randomize``
        walks, so it is written here rather than in each of them.
        """
        for col in range(self.width):
            for row in range(self.height):
                for side in (Direction.E, Direction.S):
                    if self.across(col, row, side) in self:
                        yield (col, row), side

    def open_passages(self) -> set[Edge]:
        """Every open passage between two cells, each counted once."""
        return {frozenset(((col, row), self.across(col, row, side)))
                for (col, row), side in self.internal_walls()
                if not self.get_wall(col, row, side)}

    def dead_ends(self) -> list[Cell]:
        """Cells with one way in and out: three walls standing."""
        return [(col, row)
                for col in range(self.width)
                for row in range(self.height)
                if self.wall_count(col, row) == 3]

    # -----------------------------------------------------------------
    # Writing the maze out, in the format the subject imposes.
    #
    # The encoding is the grid's own — one hexadecimal digit a cell,
    # and the digit is the value the cell already holds — so the maze
    # writes itself rather than being handed to a writer that would
    # have to reach in here to say the same thing. The route is passed
    # in: a maze cannot walk itself, and it is the one part of the file
    # that cannot be read off the walls.

    def encode(self) -> str:
        """The grid, one hexadecimal digit a cell, a row to a line.

        The grid is held in columns, so a line is read across it.
        Lowercase, as in the subject's example; either case is read.
        """
        return "\n".join("".join(f"{int(self.grid[col][row]):x}"
                                 for col in range(self.width))
                         for row in range(self.height))

    def route_letters(self, route: Path) -> str:
        """A route written as the subject writes it: one letter a step.

        Each letter names the side that step goes through, so this is
        also where a route is checked against the walls it claims to
        cross: a step to a cell that is not next door, or through a
        wall still standing, is refused. A route of one cell or none
        has no steps and comes back empty.
        """
        letters = []
        for before, after in zip(route, route[1:]):
            step = (after[0] - before[0], after[1] - before[1])
            side = _SIDE_OF.get(step)
            if side is None:
                raise ValueError(
                    f"the route jumps from {before} to {after}, "
                    f"which are not neighbouring cells")
            if self.get_wall(before[0], before[1], side):
                raise ValueError(
                    f"the route leaves {before} to the {side.letter()}, "
                    f"where a wall is still standing")
            letters.append(side.letter())
        return "".join(letters)

    def as_text(self, route: Path) -> str:
        """The whole output file, as one string.

        The grid, an empty line, then the entry, the exit and the
        route, each on a line of its own, every line newline-ended.
        The ends are written ``x,y`` — column first, the way the
        configuration file writes them.
        """
        entry, exit_cell = self._entry, self._exit
        if entry is None or exit_cell is None:
            raise ValueError(
                "a maze written out needs both an entry and an exit")
        if route and (route[0] != entry or route[-1] != exit_cell):
            raise ValueError(
                f"the route runs from {route[0]} to {route[-1]}, not from "
                f"the entry {entry} to the exit {exit_cell}")
        return (f"{self.encode()}\n\n"
                f"{entry[0]},{entry[1]}\n"
                f"{exit_cell[0]},{exit_cell[1]}\n"
                f"{self.route_letters(route)}\n")

    def save(self, filename: str, route: Path) -> None:
        """Write the maze to that file, in the subject's format.

        The text is built before the file is opened, so a route this
        cannot describe leaves the last good file alone rather than
        emptying it. Whatever the disk refuses comes back as
        ``OSError``: only the caller knows what it was trying to do.
        """
        text = self.as_text(route)
        with open(filename, "w", encoding="utf-8") as output:
            output.write(text)

    def region_of(self, col: int, row: int,
                  banned: Edge | None = None
                  ) -> Cells:
        """Every cell reachable from this one, walls taken into account.

        ``banned`` is one passage to pretend closed, so a choke point
        can be told from an ordinary one. Breadth-first, so it is also
        the cheapest way to ask whether two cells are joined at all.
        """
        self._check_bounds(col, row)
        seen = {(col, row)}
        queue = deque([(col, row)])
        while queue:
            cell = queue.popleft()
            for other, _side in self.passages(*cell):
                if banned is not None and frozenset((cell, other)) == banned:
                    continue
                if other not in seen:
                    seen.add(other)
                    queue.append(other)
        return frozenset(seen)
