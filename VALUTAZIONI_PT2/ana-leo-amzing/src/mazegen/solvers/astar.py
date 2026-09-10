"""Bidirectional A* aims with a heuristic, from both ends at once."""

from dataclasses import dataclass
from heapq import heappop, heappush

from ..maze import Cell, Paths
from ..solver import MazeSolver


def manhattan(a: Cell, b: Cell) -> int:
    """Steps between two cells on a grid assuming no walls.

    Used in A* as the h component of the computation

    f = g + h

    Basically it describes an estimate for the future cost of getting
    from ``a`` to ``b``.

    Walls only ever lengthen the real route, so this never creates an
    overestimation on the distance and keeps the answer a shortest
    path. Every step changes it by exactly one, so a cell settled once
    is settled for good.
    """
    return abs(a[0] - b[0]) + abs(a[1] - b[1])


# This is the approximation used to estimate distance.
h = manhattan


@dataclass
class Side:
    """All data structured required by each side of the search"""
    #
    # a min-heap is just a list that is handled by specific functions
    # such as heappop and heappush in order to have good performance
    # when extracting the min element. What is pushed is
    # (g + h, h, cell): the total estimate first, since that is what
    # the ordering is on, then h alone to break ties towards the goal.
    #
    frontier: list[tuple[int, int, Cell]]
    # Cost from this side's root, for the best route found so far.
    g: dict[Cell, int]
    # Cell -> the cell it was reached from. None marks the root.
    parent: dict[Cell, None | Cell]
    # Cells settled for good, and so never to be expanded again.
    closed: dict[Cell, None]
    # Where this side is headed: the *other* side's root.
    goal: Cell


class BidirectionalAStar(MazeSolver):
    """Bidirectional A*: a heuristic, aimed from both ends at once."""

    name = "astar"

    def get_visited(self) -> Paths:
        return (tuple(self.fwd.closed), tuple(self.bwd.closed),)

    @staticmethod
    def _new_side(root: Cell, goal: Cell) -> Side:
        """One side, seeded with its root.
        """
        start_h = h(root, goal)
        return Side(
            frontier=[(0 + start_h, start_h, root)],
            g={root: 0},
            parent={root: None},
            closed={},
            goal=goal,
        )

    def _initstruct(self) -> None:
        entry, exit_ = self.config.entry_cell, self.config.exit_cell

        self.fwd = self._new_side(entry, exit_)
        self.bwd = self._new_side(exit_, entry)

        # Best complete route found so far. None until the two
        # searches have met anywhere at all.
        self.mu: int | None = None
        self.meeting: Cell | None = None

        # The one crossing no relaxation can turn up, because neither
        # root is ever relaxed: the two ends being the same cell.
        self._met(entry, self.fwd, self.bwd)

        self._path = []

    def _met(self, cell: Cell, side: Side, other: Side) -> None:
        """Record ``cell`` as a crossing, if both sides have reached it.

        Reached, and not settled. Each g is the cost of a route that
        side really holds, final or not, so the sum is the length of a
        real route from entry to exit and mu stays an upper bound on
        the answer -- which is all the bound in ``_step`` weighs it as.
        Waiting for both sides to settle the cell instead would leave
        mu unset through most of the search, and mu unset is what stops
        that bound from ever firing early.

        Roots are the reason this is worth having at all: a root sits
        in its own g from the start and is never relaxed, so the only
        way a crossing *at* a root is seen is the other side relaxing
        into it and finding it here.
        """
        if cell not in other.g:
            return
        through = side.g[cell] + other.g[cell]
        if self.mu is None or through < self.mu:
            self.mu = through
            self.meeting = cell

    def _step(self) -> int:
        if not self.fwd.frontier or not self.bwd.frontier:
            # Out of frontier. With a meeting already recorded that is
            # a finish and not a failure: a route was found, there was
            # simply nothing left to prove a shorter one against.
            meeting = self.meeting
            if meeting is not None:
                self._join(meeting)
                return 1
            print("Bidirectional A* search over, path does not exist")
            return 1

        # Whichever side has settled less, so the two grow together.
        # Counted on closed and not on the heap: the heap holds stale
        # entries too, since a cell reached again more cheaply is
        # pushed rather than moved, so its length measures what has
        # been queued and not how far the side has actually got. A side
        # that relaxes heavily looks big that way and gets starved --
        # on a maze small enough it never runs at all.
        if len(self.fwd.closed) < len(self.bwd.closed):
            side, other = self.fwd, self.bwd
        else:
            side, other = self.bwd, self.fwd

        self._advance(side, other)

        # Every entry left on a heap has an f no smaller than the top
        # of that heap, so that top is a floor on any route either side
        # could still turn up.
        #
        # One floor over mu is enough, and this is the whole of why:
        # while a side has not yet settled the cell it is aimed at,
        # some cell of a shortest route is sitting on its heap holding
        # the real cost so far, and f there is g + h <= the length of
        # that route. So a floor above mu means that side has already
        # settled its own goal, and settling it recorded a complete
        # route no longer than mu. Either way mu is unbeatable.
        #
        # Waiting for *both* floors is what makes each side run a
        # near-complete search, and is where docs/2026-09-04-ALGORITHM
        # -CHOICES.md 4.2 loses the cells it claims to save.
        #
        # Tested on every call and not only when something met: the
        # floors rise with each expansion, so a mu recorded long ago
        # becomes unbeatable at some later and otherwise quiet step.
        meeting, mu = self.meeting, self.mu
        if meeting is not None and mu is not None:
            # _advance may have emptied a heap, and an empty one has
            # no top to read. Leave it: the check above catches it on
            # the next call, and joins through this same meeting.
            if self.fwd.frontier and self.bwd.frontier:
                bound = max(self.fwd.frontier[0][0],
                            self.bwd.frontier[0][0])
                if mu <= bound:
                    self._join(meeting)
                    return 1

        return 0

    def _advance(self, side: Side, other: Side) -> None:
        """Settle one cell of ``side``, and note any crossing it makes.

        Knows nothing about stopping: that stays in ``_step``, in one
        piece, so the rule that is easy to get wrong is written once.
        """
        _f, _h, cell = heappop(side.frontier)
        if cell in side.closed:
            # The cell is already settled with its final cost, so
            # nothing to do.
            return None

        # At this point we know chepeast route from side entry to this cell
        side.closed[cell] = None

        col, row = cell
        # One step on from a cell whose cost is settled, so it is the
        # same number for every neighbour and is worked out once.
        through = side.g[cell] + 1
        # passages() gives back the cell and the wall it lies through,
        # and only the cell is the child here.
        for child, _wall in self.maze.passages(col, row):
            if child in side.closed:
                continue
            if child in side.g and through >= side.g[child]:
                continue
            # Either new, or reached more cheaply than before
            side.g[child] = through
            child_h = h(child, side.goal)
            side.parent[child] = cell
            heappush(side.frontier, (through + child_h, child_h, child))
            # Cheapest point to look: whichever side reaches the cell
            # second finds the first one's g already sitting there.
            self._met(child, side, other)

    def _join(self, meeting: Cell) -> None:
        """Both halves into ``self._path``, entry first."""
        # The forward parents run from the meeting back to the entry,
        # which is the wrong way round, so turn it over. The entry is
        # the one cell of that side with no parent, and None is what
        # ends the walk, exactly as in BFS.
        half: list[Cell] = []
        cell: Cell | None = meeting
        while cell is not None:
            half.append(cell)
            cell = self.fwd.parent[cell]
        half.reverse()

        # The backward parents run from the meeting out to the exit,
        # which is already the direction wanted, so this half is the
        # one needing no reversing. Start past the meeting so it is
        # not in the path twice.
        cell = self.bwd.parent[meeting]
        while cell is not None:
            half.append(cell)
            cell = self.bwd.parent[cell]

        self._path = half
