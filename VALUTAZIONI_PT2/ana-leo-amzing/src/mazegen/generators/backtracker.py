"""Randomised backtracker: walk as far as you can, then back up."""

import random
from collections.abc import Iterator

from ..generator import MazeGenerator
from ..maze import Cell, Maze


class Backtracker(MazeGenerator):

    name = "backtracker"

    def _run(self) -> Iterator[Maze]:
        """Carve a corridor, backing up whenever it runs out of room."""

        already_visited: set[Cell] = set(self.frozen)

        current_cell = self.config.entry_cell
        already_visited.add(current_cell)
        corridor: list[Cell] = [current_cell]

        while corridor:

            current_cell = corridor[-1]
            col, row = current_cell

            # Seeding already_visited with the 42 keeps the walk from
            # stepping into the shape. The wall is checked from this
            # side as well, because the cell the walk starts from is
            # pushed on unasked: an entry inside the 42 would otherwise
            # be carved straight out of.
            unvisited_neighbours = [
                (cell, side)
                for cell, side in self.maze.neighbours(col, row)
                if cell not in already_visited
                and not self._touches_42(current_cell, side)]

            if not unvisited_neighbours:
                corridor.pop()
            else:
                next_cell, side = random.choice(unvisited_neighbours)
                self.maze.destroy_wall(col, row, side)
                already_visited.add(next_cell)
                corridor.append(next_cell)
                yield self.maze
