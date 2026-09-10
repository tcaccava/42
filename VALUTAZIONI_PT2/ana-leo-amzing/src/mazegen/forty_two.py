"""The "42" every maze has to carry.

The subject asks for a block of fully closed cells drawing "42"
somewhere inside the maze.

The shape is reserved before the maze is carved.
The generator builds around the 42 cells and everything it
joins stays joined.

"""

from .maze import ALL_WALLS, Cell, Cells, Maze

# Cells are (col, row), the order the rest of mazegen is written in.
# Read as a picture, the glyph is::
#
#       ####  ######
#     ####        ##
#   ####        ##
# ####        ##
# ##########  ######
#       ####
#       ####
#
# Column 5 is the gap between the 4 and the 2, so it has no entry.
PATTERN: Cells = frozenset({
    (0, 3), (0, 4),
    (1, 2), (1, 3), (1, 4),
    (2, 1), (2, 2), (2, 4),
    (3, 0), (3, 1), (3, 4), (3, 5), (3, 6),
    (4, 0), (4, 4), (4, 5), (4, 6),
    (6, 0), (6, 3), (6, 4),
    (7, 0), (7, 2), (7, 4),
    (8, 0), (8, 1), (8, 4),
})

COLS = 9
ROWS = 7

# The empty column between the 4 and the 2. It is what gets lined up
# with the middle of the maze, so the two digits sit either side of it.
GAP = 5

# A cell of corridor is kept between the 42 and the border. It is what
# keeps the corners open, which a Pac-Man level needs and the analyzer
# checks by name.
MARGIN = 1
MIN_WIDTH = COLS + 2 * MARGIN
MIN_HEIGHT = ROWS + 2 * MARGIN


def fits(width: int, height: int) -> bool:
    """Whether a maze that size has room for the 42 brand."""
    return width >= MIN_WIDTH and height >= MIN_HEIGHT


def cells(width: int, height: int) -> Cells:
    """Place the 42 in the middle of the maze.

    The gap between the two digits lands on the middle column, so the
    4 and the 2 sit either side of it, and the rows are shared out
    evenly above and below.

    The border cell wins over the middle when the two disagree, which
    only happens in a maze of exactly ``MIN_WIDTH``: there the digits
    are one column off centre rather than pressed against the edge.

    Only sound for a maze ``fits`` accepts; ``reserve`` is the one
    that asks.
    """
    col_offset = max(MARGIN, width // 2 - GAP)
    row_offset = (height - ROWS) // 2
    return frozenset((col + col_offset, row + row_offset)
                     for col, row in PATTERN)


def reserve(maze: Maze) -> Cells:
    """Close the 42's cells in ``maze``, and say which they are.

    Called before the maze is carved. A finished maze is a tree of
    joined cells, and closing twenty-six of them at the end strands
    whatever hung below; the analyzer reads those stranded cells as a
    level nobody could finish. Closed first, the algorithm builds
    around them.

    Both halves of the closing matter. The walls are built, because
    recursive division starts from a grid with none standing; and the
    cells are remembered, because a wall standing today says nothing
    about who is allowed to open it tomorrow.

    A maze with no room for the shape goes without it and gets an
    empty set back. The sizes the subject asks for are far larger; it
    is the small mazes the tests build that would otherwise have
    nowhere to put it.
    """
    if not fits(maze.width, maze.height):
        return frozenset()
    chosen = cells(maze.width, maze.height)
    for col, row in chosen:
        maze.build_wall(col, row, ALL_WALLS)
    maze.set_frozen(chosen)
    return chosen


def blocks(width: int, height: int, cell: Cell) -> bool:
    """Whether the 42 would seal ``cell`` in a maze that size.

    The glyph's cells are closed on all four sides, so an entry or an
    exit landing on one is walled in before the maze is even carved:
    the entry would have no way out, the exit no way in. Asking this
    first is what lets a caller refuse the pair with a clear message
    rather than hand back a maze nobody can cross.

    A maze with no room for the glyph carries none, so nothing is
    blocked in it.
    """
    return fits(width, height) and cell in cells(width, height)
