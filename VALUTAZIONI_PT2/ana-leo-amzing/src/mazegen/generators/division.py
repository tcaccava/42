"""Recursive division: cut a room in two, and leave a gap in the wall.

Chosen in ``docs/2026-09-04-ALGORITHM-CHOICES.md`` §2.3 as the third
paradigm — splitting space, where the other two carve a path and join
regions. Divide and conquer: cut the rectangle with a wall, punch a
gap in it, recurse into both halves.

Two things make it unlike the other two:

* it starts from an open grid and puts walls up, so it is the only
  generator that calls ``build_wall``;
* its recursion depth is ~log2 of the size rather than the number of
  cells, so it can stay recursive without risking Python's limit.

The "42" is what makes it more than that. Its cells are sealed before
the algorithm runs, so a region can arrive already in two pieces, and
the gap in the wall that made it cannot join them. ``_run`` divides
first and rejoins those pieces afterwards.
"""
import random
from collections.abc import Iterator

from ..generator import MazeGenerator
from ..maze import Maze, Cell, Direction


class Division(MazeGenerator):
    """Divide the field in two, over and over, until it is a maze.

    The longer side of a region is the one that gets cut, so the
    halves stay roughly square instead of drawing themselves out
    into corridors. That is where the long straight walls this
    algorithm is recognised by come from.
    """

    name = "division"

    def _blank(self) -> Maze:
        """Return a maze with no walls built"""
        maze = super()._blank()
        for (col, row), side in maze.internal_walls():
            maze.destroy_wall(col, row, side)
        return maze

    def _divide(self, min_col: int, max_col: int,
                min_row: int, max_row: int) -> Iterator[Maze]:
        """Put one wall across this region, then divide both halves.

        The region is given as the cells it holds, both ends
        included. A wall needs two cells to stand between, so a
        region that is a single cell each way is where the
        recursion stops.
        """
        width = max_col - min_col + 1
        height = max_row - min_row + 1

        if width < 2 and height < 2:
            return

        line: list[tuple[Cell, Direction]]
        if width > height:
            # if the rectangle is wider we cut it with a vertical wall
            wall_col = random.choice(range(min_col, max_col))
            line = [((wall_col, row), Direction.E)
                    for row in range(min_row, max_row + 1)]
            halves = ((min_col, wall_col, min_row, max_row),
                      (wall_col + 1, max_col, min_row, max_row))
        else:
            # if the rectangle is taller, or square, we cut it with a
            # horizontal wall
            wall_row = random.choice(range(min_row, max_row))
            line = [((col, wall_row), Direction.S)
                    for col in range(min_col, max_col + 1)]
            halves = ((min_col, max_col, min_row, wall_row),
                      (min_col, max_col, wall_row + 1, max_row))

        # Pick a random place in the wall to leave a gap, but not one that
        # touches the "42" shape.
        free_walls = [(cell, side) for cell, side in line
                      if not self._touches_42(cell, side)]
        gap = random.choice(free_walls) if free_walls else None

        # Build the wall, leaving a gap, and yield the maze after each
        # wall is built.
        for place in free_walls:
            if place == gap:
                continue
            (col, row), side = place
            self.maze.build_wall(col, row, side)
            yield self.maze

        for first_col, last_col, first_row, last_row in halves:
            yield from self._divide(first_col, last_col,
                                    first_row, last_row)

    def _run(self) -> Iterator[Maze]:
        """Divide the field, then rejoin what the 42 cut off."""
        yield from self._divide(0, self.maze.width - 1,
                                0, self.maze.height - 1)

        # To deal with the "42" shape, we need to create a set of
        # available cells that are not frozen, and then pick a random
        # wall to destroy that is not touching the "42" shape. This will
        # ensure that the maze is fully connected and that there is a
        # path from the entry cell to the exit cell.
        free_cells = frozenset((col, row)
                               for col in range(self.maze.width)
                               for row in range(self.maze.height)
                               if (col, row) not in self.frozen)
        # Each wall opened stands between a piece already reached and
        # one not reached yet, so this can never close a loop.
        while free_cells:
            start_col, start_row = min(free_cells)
            near = self.maze.region_of(start_col, start_row)
            if free_cells <= near:
                break
            across = self._closed_across(near)
            if not across:
                break
            (col, row), side = random.choice(across)
            self.maze.destroy_wall(col, row, side)
            yield self.maze
