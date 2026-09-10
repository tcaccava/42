"""Draw a maze in the terminal. The maze is read, never written.

A WIDTH x HEIGHT maze becomes 2 * HEIGHT + 1 alternating lines::

    ┏━━━┳━━━━━━━┓   wall line: junctions and the walls above
    ┃ E │       ┃   cell line: the walls beside, and the insides
    ┗━━━┻━━━┻━━━┛   one last wall line closes the bottom

Each cell draws only its N and W sides, so no wall is drawn twice.
The sides no cell draws are exactly the border, which is drawn first.
"""

import re
import shutil
import sys
import termios
import textwrap
import time
import tty
from contextlib import contextmanager
from dataclasses import dataclass, field, replace
from typing import Iterable, Iterator, Sequence

import rainbow
from mazegen import Cell, Direction, Maze, Path, Paths

# A cell on a path: the sides it links to, and the element that drew it.
Link = tuple[int, "JoinedElement"]

# Three, because a terminal character is twice as tall as it is wide;
# at 1 a square maze comes out flattened.
CELL_WIDTH = 3


WALL_COLOURS = (
    "chambray",
    "vermilion",
    "gamboge",
    "peppermint",
    "ultramarine",
)


# Frames a second, to cancel out what a frame costs to draw: a 30x30
# takes some twenty milliseconds, a small one almost none, so without
# this the first crawled while the second went by unseen.
DEFAULT_FPS = 30
MIN_FPS = 1
MAX_FPS = 240

# One carved wall in this many is drawn: a 30x30 carve yields around a
# thousand frames, and showing every one is half a minute of watching.
CARVE_EVERY = 1

# A search touches far fewer cells than a carve takes down walls, so
# every frame of it is drawn and the spread is watched cell by cell.
SEEN_EVERY = 1


class TooSmallTerminal(Exception):
    """The window cannot hold the drawing of that maze."""


@dataclass(frozen=True)
class Element:
    """A symbol and the ink it is written in.

    ``ink`` is the body of an ANSI escape — ``"38;5;N"`` for colour N
    of the 256 palette — as ``rainbow`` builds it from a name. Empty
    means the terminal's own colour.
    """

    symbol: str = " "
    ink: str = ""


@dataclass(frozen=True)
class JoinedElement(Element):
    """An element whose symbol depends on its neighbours.

    Walls, borders and paths are lines, so the piece to use — ``─``,
    ``┐``, ``┼`` — is known only once the neighbours are. ``joints``
    holds all sixteen, indexed by the sides reached with Direction's
    bits (N=1, E=2, S=4, W=8): index 10 is E|W, a horizontal line.
    """

    joints: str = " " * 16

    def __post_init__(self) -> None:
        """Refuse a table that cannot answer every junction."""
        if len(self.joints) != 16:
            raise ValueError(
                f"joints needs one symbol per junction, that is 16, "
                f"got {len(self.joints)}"
            )

    def piece(self, sides: int) -> str:
        """What piece to use for a junction with those sides."""
        return self.joints[sides]

    def straight(self, direction: Direction) -> str:
        """The piece for a straight line running that way."""
        if direction in (Direction.N, Direction.S):
            return self.joints[Direction.N | Direction.S]
        return self.joints[Direction.E | Direction.W]


@dataclass(frozen=True)
class Frame:
    """The canvas around the maze: head, tail and sides."""

    head: int = 2
    tail: int = 3
    sides: int = 2


@dataclass(frozen=True)
class TerminalSkin:
    """A whole look: one element per thing that can be drawn."""

    floor: Element
    wall: JoinedElement
    border: JoinedElement
    entry: Element
    exit: Element
    paths: tuple[JoinedElement, ...]
    title: Element
    message: Element
    meeting: JoinedElement | None = None
    forty_two: Element | None = None
    seen: tuple[Element, ...] = ()


STANDARD_SKIN = TerminalSkin(
    floor=Element(" "),
    wall=JoinedElement(ink=rainbow.ink("chambray"),
                       joints=" ╵╶└╷│┌├╴┘─┴┐┤┬┼"),
    border=JoinedElement(ink=rainbow.ink("cloud"),
                         joints=" ╹╺┗╻┃┏┠╸┛━┷┓┨┯╋"),
    paths=(JoinedElement(ink=rainbow.ink("gamboge"),
                         joints="•╵╶╰╷│╭├╴╯─┴╮┤┬┼"),
           JoinedElement(ink=rainbow.ink("manganese"),
                         joints="•╵╶╰╷│╭├╴╯─┴╮┤┬┼")),
    meeting=JoinedElement(ink=rainbow.ink("verdigris"),
                          joints="•╵╶╰╷│╭├╴╯─┴╮┤┬┼"),
    entry=Element("⬤", ink=rainbow.ink("peppermint")),
    exit=Element("⚑", ink=rainbow.ink("madder")),
    title=Element(ink=rainbow.ink("snow")),
    message=Element(ink=rainbow.ink("stone")),
    forty_two=Element("◥", ink=rainbow.ink("robin")),
    seen=(Element("✱", ink=rainbow.ink("glacier")),
          Element("✱", ink=rainbow.ink("lilac"))),
)

FRAME = Frame()


@dataclass
class Drawing:
    """Everything a rendering needs to draw.

    ``paths`` is one sequence per route, ``visited`` one per search
    front that found them; a route is drawn over its own search. The
    two ends are the maze's, and ``render`` reads them from it.
    """

    paths: Paths = ()
    visited: Paths = ()
    title: str = ""
    message: Sequence[str] = ()
    skin: TerminalSkin | None = None
    frame: Frame = FRAME
    cell_width: int = CELL_WIDTH

    def drawn_with(self) -> TerminalSkin:
        """The skin named, or the standard one."""
        return self.skin if self.skin is not None else STANDARD_SKIN


# Direction is an IntFlag, and every & or | on one builds a fresh enum
# member — tens of thousands a frame, once over half the drawing time.
# The bit work uses these plain ints; Direction stays the edge language.
NORTH = int(Direction.N)
EAST = int(Direction.E)
SOUTH = int(Direction.S)
WEST = int(Direction.W)


def maze_wall_above(maze: Maze, col: int, row: int) -> bool:
    """A horizontal wall above (col, row)? The N and S edges always are."""
    width, height = maze.get_dim()
    if not (0 <= col < width and 0 <= row <= height):
        return False
    if row in (0, height):
        return True
    return maze.get_wall(col, row, Direction.N)


def maze_wall_left(maze: Maze, col: int, row: int) -> bool:
    """An upright wall left of (col, row)? The W and E edges always are."""
    width, height = maze.get_dim()
    if not (0 <= row < height and 0 <= col <= width):
        return False
    if col in (0, width):
        return True
    return maze.get_wall(col, row, Direction.W)


def path_sides(path: Path) -> dict[Cell, int]:
    """Per cell of the route, the sides it links to, added together.

    The same sixteen-piece index the walls are drawn with.
    """
    sides: dict[Cell, int] = {}
    for index, cell in enumerate(path):
        reaches = 0
        for other in (path[index - 1] if index else None,
                      path[index + 1] if index + 1 < len(path) else None):
            if other is None:
                continue
            for side in Direction:
                step_col, step_row = side.step()
                if (cell[0] + step_col, cell[1] + step_row) == other:
                    reaches |= side
        sides[cell] = reaches
    return sides


def merge_paths(paths: Paths, elements: Sequence[JoinedElement],
                meeting: JoinedElement | None = None,
                goal: Cell | None = None) -> dict[Cell, Link]:
    """Merge several paths into one link per cell.

    Each path takes its own element, cycling if there are more paths
    than elements; shared cells add their sides together. Once solved
    every cell takes ``meeting`` instead, so a bidirectional search
    goes two colours to the moment it meets and one from there on.
    Solved means a shared cell, or a path that reached ``goal``.
    """
    sides_of: dict[Cell, int] = {}
    walked_by: dict[Cell, set[int]] = {}
    for index, path in enumerate(paths):
        for cell, sides in path_sides(path).items():
            sides_of[cell] = sides_of.get(cell, 0) | sides
            walked_by.setdefault(cell, set()).add(index)

    solved = (any(len(walkers) > 1 for walkers in walked_by.values())
              or (goal is not None and goal in sides_of))
    links: dict[Cell, Link] = {}
    for cell, sides in sides_of.items():
        if solved and meeting is not None:
            element = meeting
        else:
            element = elements[min(walked_by[cell]) % len(elements)]
        links[cell] = (sides, element)
    return links


def paint(text: str, element: Element) -> str:
    """Wrap the text in its element's colour."""
    if not text:
        return text
    return f"\033[{element.ink}m{text}\033[0m" if element.ink else text


# ---------------------------------------------------------------------
# The maze itself: one wall line and one cell line per row.


def maze_junction(maze: Maze, col: int, row: int,
                  skin: TerminalSkin) -> tuple[str, Element]:
    """The piece belonging where four walls could meet."""
    width, height = maze.get_dim()
    sides = 0
    if maze_wall_left(maze, col, row - 1):
        sides |= NORTH
    if maze_wall_left(maze, col, row):
        sides |= SOUTH
    if maze_wall_above(maze, col, row):
        sides |= EAST
    if maze_wall_above(maze, col - 1, row):
        sides |= WEST
    on_frame = col in (0, width) or row in (0, height)
    element = skin.border if on_frame else skin.wall
    return element.piece(sides), element


def _wall_line(maze: Maze, row: int, plan: Drawing,
               links: dict[Cell, Link]) -> str:
    """The horizontal line above row ``row``: junctions and the walls.

    One more of these than there are rows — row 0 is the top, row
    ``height`` the bottom. An opening here is crossed north to south.
    """
    skin, cell_width = plan.drawn_with(), plan.cell_width
    width, height = maze.get_dim()
    out = []
    for col in range(width + 1):
        symbol, element = maze_junction(maze, col, row, skin)
        out.append(paint(symbol, element))
        if col == width:
            break
        if maze_wall_above(maze, col, row):
            element = skin.border if row in (0, height) else skin.wall
            out.append(paint(element.straight(Direction.E) * cell_width,
                             element))
        else:
            out.append(_wall_opening(
                skin, cell_width, Direction.N,
                _path_through(links, (col, row), Direction.N)))
    return "".join(out)


def _cell_line(maze: Maze, row: int, plan: Drawing,
               links: dict[Cell, Link],
               marks: dict[Cell, Element],
               seen: dict[Cell, Element]) -> str:
    """The line through a row: the wall beside each cell, then inside it.

    The wall here is upright, so an opening is crossed east to west.
    """
    skin = plan.drawn_with()
    width, _ = maze.get_dim()
    out = []
    for col in range(width + 1):
        if maze_wall_left(maze, col, row):
            element = skin.border if col in (0, width) else skin.wall
            out.append(paint(element.straight(Direction.N), element))
        else:
            out.append(_wall_opening(
                skin, 1, Direction.W,
                _path_through(links, (col, row), Direction.W)))
        if col == width:
            break
        out.append(_cell_body(col, row, plan, links, marks, seen))
    return "".join(out)


def _cell_body(col: int, row: int, plan: Drawing,
               links: dict[Cell, Link],
               marks: dict[Cell, Element],
               seen: dict[Cell, Element]) -> str:
    """The inside of one cell: everything that is not a wall.

    A middle where a mark or the path stands, and a run either side
    of it reaching towards the cells it links to::

        "   "  empty    " E "  the entry
        "───"  straight through    "─╮ "  in from the west, turning south
    """
    skin, cell_width = plan.drawn_with(), plan.cell_width
    cell = (col, row)
    middle = cell_width // 2
    sides, element = links.get(cell, (0, None))
    return (_path_run(middle, skin, element if sides & Direction.W else None)
            + _cell_centre(skin, cell, marks, links, seen)
            + _path_run(cell_width - middle - 1, skin,
                        element if sides & Direction.E else None))


def _cell_centre(skin: TerminalSkin, cell: Cell,
                 marks: dict[Cell, Element],
                 links: dict[Cell, Link],
                 seen: dict[Cell, Element]) -> str:
    """The character in the middle of a cell.

    Mark beats path beats searched beats floor. The order matters:
    every cell of a route was searched first, so drawing the search
    last would rub the route out.
    """
    if cell in marks:
        return paint(marks[cell].symbol, marks[cell])
    if cell in links:
        sides, element = links[cell]
        return paint(element.piece(sides), element)
    if cell in seen:
        return paint(seen[cell].symbol, seen[cell])
    return paint(skin.floor.symbol, skin.floor)


def _path_run(span: int, skin: TerminalSkin,
              path: JoinedElement | None) -> str:
    """The path running out of a cell, or the floor."""
    if path is None:
        return paint(skin.floor.symbol * span, skin.floor)
    return paint(path.straight(Direction.E) * span, path)


def _wall_opening(skin: TerminalSkin, span: int, through: Direction,
                  crossing: JoinedElement | None) -> str:
    """What fills a missing wall: the path crossing it, or the floor.

    ``through`` is the way it is crossed — north for a gap in a wall
    running across, west for an upright one. The span cannot tell
    them apart, so the caller says which.
    """
    if crossing is None:
        return paint(skin.floor.symbol * span, skin.floor)
    piece = crossing.straight(through)
    middle = span // 2
    return (paint(skin.floor.symbol * middle, skin.floor)
            + paint(piece, crossing)
            + paint(skin.floor.symbol * (span - middle - 1), skin.floor))


def _path_through(links: dict[Cell, Link], cell: Cell,
                  side: Direction) -> JoinedElement | None:
    """The path crossing the opening on that side, if any.

    Asked from both cells: either may link towards the other, and
    both mean the same crossing.
    """
    step_col, step_row = side.step()
    other = (cell[0] + step_col, cell[1] + step_row)
    sides, element = links.get(cell, (0, None))
    if sides & side:
        return element
    sides, element = links.get(other, (0, None))
    if sides & side.opposite():
        return element
    return None


def wrap(text: str, width: int) -> list[str]:
    """Break a message into as many lines as it takes to keep all of it.

    One that fits comes back untouched, spacing and all — the action
    bar spaces its keys on purpose. Only a message that does not fit
    spends that spacing.
    """
    if width < 1 or len(text) <= width:
        return [text]
    return textwrap.wrap(text, width, break_long_words=True,
                         break_on_hyphens=False) or [""]


def message_lines(message: Sequence[str], canvas: int,
                  frame: Frame) -> list[str]:
    """The lines under the maze, wrapped to the canvas.

    ``frame.tail`` is a floor, not a count: fewer are padded with
    blanks so nothing jumps between frames, more make the tail
    taller rather than being cut.
    """
    lines = [line for text in message for line in wrap(text, canvas)]
    return lines + [""] * (frame.tail - len(lines))


def band(text: str, width: int, element: Element) -> str:
    """One line of canvas, with the text centred on it."""
    text = text[:width]
    left = (width - len(text)) // 2
    return (" " * left + paint(text, element)
            + " " * (width - left - len(text)))


# ---------------------------------------------------------------------
# The canvas around it, and the render function itself.


def drawing_size(width: int, height: int, cell_width: int = CELL_WIDTH,
                 frame: Frame = FRAME) -> tuple[int, int]:
    """The columns and lines a drawing of that maze would take.

    Each cell costs its width plus the wall beside it, and one more
    closes the right; each row costs a cell line and a wall line,
    and one more closes the bottom. The frame is added to both.
    """
    return (width * (cell_width + 1) + 1 + 2 * frame.sides,
            2 * height + 1 + frame.head + frame.tail)


def mark_cells(maze: Maze, plan: Drawing,
               skin: TerminalSkin) -> dict[Cell, Element]:
    """What stands on a cell instead of the floor, in drawing order.

    The "42" first, so an end standing on one of its cells is drawn
    over the filling; then entry and exit, which always win.
    """
    marks: dict[Cell, Element] = {}
    if skin.forty_two is not None:
        for filled in maze.get_frozen():
            marks[filled] = skin.forty_two
    for cell, element in ((maze.get_entry(), skin.entry),
                          (maze.get_exit(), skin.exit)):
        if cell is not None and element is not None:
            marks[cell] = element
    return marks


def seen_cells(plan: Drawing, skin: TerminalSkin) -> dict[Cell, Element]:
    """What a cell the search went through looks like.

    One element per front, so the two halves of a bidirectional
    search are told apart the way two routes are. A cell claimed
    twice keeps the front that reached it first.
    """
    seen: dict[Cell, Element] = {}
    if not skin.seen:
        return seen
    for index, front in enumerate(plan.visited):
        element = skin.seen[index % len(skin.seen)]
        for cell in front:
            seen.setdefault(cell, element)
    return seen


def canvas(body: Sequence[str], plan: Drawing, skin: TerminalSkin,
           columns: int) -> str:
    """Set the drawn maze in its canvas: title above, messages below."""
    frame = plan.frame
    side = " " * frame.sides
    out = [band("", columns, skin.title) for _ in range(frame.head)]
    if plan.title and frame.head:
        out[frame.head // 2] = band(plan.title, columns, skin.title)
    out.extend(side + drawn + side for drawn in body)
    for text in message_lines(plan.message, columns, frame):
        out.append(band(text, columns, skin.message))
    return "\n".join(out)


def too_small(needed: tuple[int, int],
              space: tuple[int, int]) -> TooSmallTerminal:
    """The refusal, said so the reader can act on it.

    A terminal is made bigger by making its text smaller, which is
    not the first thing that comes to mind, so it is said out loud.
    """
    short = []
    if needed[0] > space[0]:
        short.append(f"{needed[0] - space[0]} columns")
    if needed[1] > space[1]:
        short.append(f"{needed[1] - space[1]} lines")
    return TooSmallTerminal(
        f"this maze needs {needed[0]} columns x {needed[1]} lines, and "
        f"the window is {space[0]} x {space[1]} — {' and '.join(short)} "
        f"short. Press ctrl and minus to make the text smaller: the "
        f"window then holds more of both."
    )


def render(maze: Maze, drawing: Drawing | None = None) -> str:
    """Draw a maze and return it as one string, ready to print.

    The path in every cell, what stands on a cell, the maze drawn
    line by line from both, then the canvas around it.

    Raises ``TooSmallTerminal`` rather than shrinking to fit: half a
    maze is not an answer.
    """
    plan = drawing or Drawing()
    skin = plan.drawn_with()
    width, height = maze.get_dim()
    columns, lines = drawing_size(width, height, plan.cell_width, plan.frame)
    space = shutil.get_terminal_size()
    if columns > space.columns or lines > space.lines:
        raise too_small((columns, lines), (space.columns, space.lines))

    links = merge_paths(plan.paths, skin.paths, skin.meeting, maze.get_exit())
    marks = mark_cells(maze, plan, skin)
    seen = seen_cells(plan, skin)
    # Lines alternate: even ones are walls, odd ones are cells.
    # divmod gives both answers at once — which kind, and which row.
    body = []
    for char_row in range(2 * height + 1):
        row, kind = divmod(char_row, 2)
        body.append(_cell_line(maze, row, plan, links, marks, seen) if kind
                    else _wall_line(maze, row, plan, links))
    return canvas(body, plan, skin, columns)


def redraw(maze: Maze, drawing: Drawing | None = None) -> None:
    """Draw the maze over the last one, in a single write.

    Never wiped first — that shows as a flicker. Instead the cursor
    goes home, each line erases the rest of its row (``\033[K``), and
    one erase at the end clears rows a shorter drawing left behind
    (``\033[J``).

    No trailing newline: a full-height frame would scroll the window
    by a row and the next would start one row high.
    """
    frame = render(maze, drawing).replace("\n", "\033[K\n")
    sys.stdout.write("\033[H" + frame + "\033[K\033[J")
    sys.stdout.flush()


def read_key() -> str:
    """Wait for one key, without requiring Enter."""
    fd = sys.stdin.fileno()
    old = termios.tcgetattr(fd)
    try:
        tty.setraw(fd)
        return sys.stdin.read(1)
    finally:
        termios.tcsetattr(fd, termios.TCSADRAIN, old)


def colour_menu(maze: Maze, drawing: Drawing) -> str:
    """Numbered swatches, centred on the same canvas as the messages."""
    parts: list[str] = []
    for index, name in enumerate(WALL_COLOURS, start=1):
        swatch = rainbow.write("█", ink=name)
        parts.append(f"{index} {swatch} {name}")
    text = "  ".join(parts)
    width, height = maze.get_dim()
    columns, _ = drawing_size(width, height, drawing.cell_width,
                              drawing.frame)
    visible = len(re.sub(r"\033\[[0-9;]*m", "", text))
    left = max(0, (columns - visible) // 2)
    right = max(0, columns - left - visible)
    return " " * left + text + " " * right


def show_colour_menu(maze: Maze, drawing: Drawing) -> None:
    """Put the swatches on the canvas's last line, in place.

    ``redraw`` parks the cursor at the end of the line it drew, so
    printing the menu would start it there and run off to the right.
    That last line is blank padding, so the menu costs no height.
    """
    width, height = maze.get_dim()
    columns, lines = drawing_size(width, height, drawing.cell_width,
                                  drawing.frame)
    sys.stdout.write(f"\033[{lines};1H" + colour_menu(maze, drawing)
                     + "\033[K")
    sys.stdout.flush()


def apply_wall_colour(drawing: Drawing, colour: str) -> None:
    """Repaint interior walls and the border in that colour."""
    base = drawing.drawn_with()
    ink = rainbow.ink(colour)
    drawing.skin = replace(
        base,
        wall=replace(base.wall, ink=ink),
        border=replace(base.border, ink=ink),
    )


# ---------------------------------------------------------------------
# Animation: the same drawing, again, over itself.


class FrameClock:
    """Holds an animation to a fixed rate, whatever a frame costs.

    ``tick`` waits out the frame's slot, or returns True if the frame
    already overran it — the caller's cue to drop a source frame. A
    slow terminal shows less of the animation rather than taking
    longer over it. ``fps`` of None does not pace at all.
    """

    def __init__(self, fps: float | None = DEFAULT_FPS) -> None:
        self.interval = 1.0 / fps if fps else 0.0
        self.deadline: float | None = None

    def tick(self) -> bool:
        """Wait out this frame's slot. True if it had already ended."""
        if not self.interval:
            return False
        now = time.perf_counter()
        if self.deadline is None:
            self.deadline = now + self.interval
            return False
        if now < self.deadline:
            time.sleep(self.deadline - now)
            self.deadline += self.interval
            return False
        # Late: skip the deadline past every missed slot, so catching
        # up is never attempted by drawing faster than asked.
        missed = int((now - self.deadline) // self.interval) + 1
        self.deadline += self.interval * missed
        return True


@dataclass
class AnimationFrame:
    """One snapshot: a maze state and the drawing that goes with it."""

    maze: Maze
    drawing: Drawing = field(default_factory=Drawing)


def _growing(tracks: Paths) -> Iterator[Paths]:
    """Every track grown by one cell a frame, all of them together.

    Together, not one after another, so a route or search running
    from both ends is seen closing on itself.
    """
    length = max((len(track) for track in tracks), default=0)
    for end in range(length + 1):
        yield tuple(track[:end] for track in tracks)


def path_frames(maze: Maze, drawing: Drawing) -> Iterable[AnimationFrame]:
    """Snapshots revealing every route a cell at a time."""
    return (AnimationFrame(maze, replace(drawing, paths=grown))
            for grown in _growing(drawing.paths))


def seen_frames(maze: Maze, drawing: Drawing) -> Iterable[AnimationFrame]:
    """Snapshots spreading the search a cell at a time.

    Replayed in the solver's own order, as ``get_visited`` gave it.
    """
    return (AnimationFrame(maze, replace(drawing, visited=grown))
            for grown in _growing(drawing.visited))


def clear_screen() -> None:
    """Send the cursor home and wipe what is on the screen."""
    sys.stdout.write("\033[H\033[J")


def hide_cursor() -> None:
    """Take the cursor off the screen.

    It parks at the end of the last line drawn, blinking, and wanders
    if the screen scrolls. Neither is part of the picture.
    """
    sys.stdout.write("\033[?25l")
    sys.stdout.flush()


def show_cursor() -> None:
    """Put the cursor back. Always paired with ``hide_cursor``."""
    sys.stdout.write("\033[?25h")
    sys.stdout.flush()


@contextmanager
def cursor_hidden() -> Iterator[None]:
    """Keep the cursor hidden for as long as the block runs.

    Held across a whole session, not one animation: put back between
    two of them it shows for a frame, and that reads as a blink.

    Restored on the way out however that happens, ctrl-c included —
    a terminal left cursorless is one the next program has to fix.
    """
    hide_cursor()
    try:
        yield
    finally:
        show_cursor()


def _play(frame: Maze | AnimationFrame, shared: Drawing,
          clock: FrameClock, reveal: bool) -> bool:
    """Draw one frame, growing its paths a cell at a time if asked.

    True if any of it overran its slot, so the caller knows the
    animation is behind the rate asked for.
    """
    if isinstance(frame, AnimationFrame):
        maze, plan = frame.maze, frame.drawing
    else:
        maze, plan = frame, shared
    snapshots = (path_frames(maze, plan)
                 if reveal and plan.paths
                 else (AnimationFrame(maze, plan),))
    late = False
    for snapshot in snapshots:
        redraw(snapshot.maze, snapshot.drawing)
        late = clock.tick() or late
    return late


def animate(frames: Iterable[Maze | AnimationFrame],
            drawing: Drawing | None = None,
            fps: float | None = DEFAULT_FPS, reveal: bool = True,
            every: int = 1) -> None:
    """Draw one maze after another in place, as a moving picture.

    ``frames`` are maze states, or ``AnimationFrame`` snapshots that
    carry their own drawing; a generator yielding after each change
    animates directly, as ``animate(generator.carve())``.

    ``reveal`` grows a drawing's paths a cell at a time; False holds a
    finished route instead of growing it again from nothing.

    ``every`` draws one frame in that many. A carve yields after each
    wall and drawing costs far more than carving, so what makes a
    long one watchable is showing less of it. The last frame is
    always drawn — it is the finished picture.

    ``fps`` paces what is left, via ``FrameClock``.

    The screen is wiped once, then every frame overwrites in place.
    The cursor is the caller's to hide, with ``cursor_hidden``.
    """
    if every < 1:
        raise ValueError(f"a frame in every {every} is not a number of "
                         f"frames; the least is 1")
    shared = drawing or Drawing()
    clear_screen()
    clock = FrameClock(fps)
    skipped: Maze | AnimationFrame | None = None
    behind = False
    for number, frame in enumerate(frames):
        if number % every or behind:
            skipped = frame
            behind = False
            continue
        skipped = None
        behind = _play(frame, shared, clock, reveal)
    if skipped is not None:
        _play(skipped, shared, clock, reveal)
