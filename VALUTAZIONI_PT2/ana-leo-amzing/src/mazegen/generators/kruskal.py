"""Randomised Kruskal's: join cells until they are all one maze."""

import random
from collections.abc import Iterator

from ..generator import MazeGenerator
from ..maze import Cell, Maze


class _Forest:
    """Union-find: the groups of already-joined cells, one tree each.

    Kruskal's asks one question over and over: can these two cells
    already reach each other? Each group is held as a tree, and a cell
    answers with the root of its tree, so two cells are joined exactly
    when they answer the same.
    """

    def __init__(self) -> None:
        self._parent: dict[Cell, Cell] = {}
        self._size: dict[Cell, int] = {}

    def _root(self, cell: Cell) -> Cell:
        """The cell that stands for the group this one is in.

        Every cell passed on the way up is re-pointed straight at the
        root, so the trees stay shallow however many joins happen.
        """
        self._parent.setdefault(cell, cell)
        self._size.setdefault(cell, 1)

        root = cell
        while self._parent[root] != root:
            root = self._parent[root]
        while self._parent[cell] != root:
            self._parent[cell], cell = root, self._parent[cell]
        return root

    def join(self, one: Cell, other: Cell) -> bool:
        """Join the two groups, and say whether they were apart.

        This method returns False if the cells could already reach
        each other. It returns True if they could not.
        """

        # If they have the same root, they are in the same group, and
        # therefore they were able to reach eachothers before this call
        left, right = self._root(one), self._root(other)
        if left == right:
            return False

        # The smaller group hangs off the larger
        if self._size[left] < self._size[right]:
            left, right = right, left
        self._parent[right] = left
        self._size[left] += self._size[right]

        return True


class Kruskal(MazeGenerator):
    """Kruskal's algorithm, with the walls taken in a random order.

    Every wall in the maze is considered once, shuffled. A wall comes
    down when the two cells it stands between cannot yet reach each
    other.

    The result is a spanning tree where every cell is reachable, and
    between any two cells there exists exactly one path. This is what
    PERFECT=True asks for.
    """

    name = "kruskal"

    def _run(self) -> Iterator[Maze]:
        """Take the walls in a random order, opening the ones that join."""
        joined = _Forest()

        # Not every wall: the ones around the "42" were set aside
        # before this ran, and opening one would let the maze into a
        # cell that has to stay sealed.
        walls = self._free_walls()

        # Kruskal needs a specific order to look at the walls. Given
        # that all walls are equal in the context of a Maze, we can
        # shuffle it to get a random permutation of walls and use that.
        random.shuffle(walls)

        # Lets iterate over all the walls of the maze
        for cell, side in walls:

            # We consider the current cell and the cell across the
            # wall that we're currently analyzing
            col, row = cell
            cell_across = self.maze.across(col, row, side)

            # if it was not possible to reach from one cell the other
            # cell, then we break the wall in order to make it
            # possible.
            if joined.join(cell, cell_across):
                self.maze.destroy_wall(col, row, side)

                # everytime we break a wall we yield the new updated
                # maze. This is used for animation purposes in the
                # renderer
                yield self.maze
