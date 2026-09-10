"""Breadth-first search: the shortest way out, counted in steps."""

from collections import deque

from ..maze import Cell, Paths
from ..solver import MazeSolver


class BFS(MazeSolver):

    name = "bfs"

    def get_visited(self) -> Paths:
        return (tuple(self.visited.keys()),)

    def _initstruct(self) -> None:
        # Initialize the queue with the entry cell and a distance of 0
        self.queue: deque[tuple[Cell, int]] = deque()
        self.queue.append((self.config.entry_cell, 0))
        # Keep track of visited cells. A dict is a set that remembers
        # the order things went into it: the keys answer "reached
        # already?", and reading them back gives the order the search
        # reached them, which is what the animation replays. The values
        # are never used.
        self.visited: dict[Cell, None] = {self.config.entry_cell: None}
        # Key: cell, Value: its parent, None = entry cell
        self.parent: dict[Cell, None | Cell] = {self.config.entry_cell: None}
        self._path = []

    def _step(self) -> int:
        if not self.queue:
            print("BFS search over, did not find exit cell error")
            return 1
        current_cell, distance = self.queue.popleft()
        if current_cell == self.config.exit_cell:
            # Walk the parents back from the exit. The entry is the
            # one cell with no parent, so None is what ends the walk.
            cell: Cell | None = self.config.exit_cell
            while cell is not None:
                self._path.append(cell)
                cell = self.parent[cell]
            self._path.reverse()
            return 1
        else:
            col, row = current_cell
            # passages() gives back the cell and the side it lies
            # through, and only the cell is the child here.
            for child, _side in self.maze.passages(col, row):
                if child not in self.visited:
                    # Marked as it goes in, not as it comes out: a cell
                    # waiting in the queue is not yet "visited", and
                    # every other neighbour that looked at it in the
                    # meantime would queue it a second time.
                    self.visited[child] = None
                    self.queue.append((child, distance + 1))
                    self.parent[child] = current_cell
            return 0
