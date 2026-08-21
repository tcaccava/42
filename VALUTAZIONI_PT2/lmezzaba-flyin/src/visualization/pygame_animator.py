"""Optional pygame animation of the flight.

Imported only when ``--animate`` is passed, so pygame stays a purely
optional dependency. One lightweight snapshot of the fleet is recorded
per turn, then replayed in a window with the drones interpolated
between two consecutive turns. No simulation logic is duplicated here.
"""

import math
import os
import sys
import warnings
from typing import Any, Dict, List, Optional, Tuple, Type

from src.models.drone import Drone
from src.models.network import Network
from src.models.zone import Zone

#: Where one drone is during one turn: a zone name, or a zone pair with
#: how far along the connection the drone has flown.
Spot = Tuple[str, Optional[str], float]
#: One snapshot of the whole fleet: drone id -> :data:`Spot`.
Frame = Dict[int, Spot]

#: Monospace families shipped with Ubuntu, best match first. SysFont
#: takes a comma-separated list and falls back to the default font when
#: none of them is installed, so this is safe on any distribution.
FONT_FAMILIES = "ubuntumono,dejavusansmono,liberationmono,freemono"

#: The same fonts as plain files. Resolving a family name needs
#: ``pygame.sysfont``, one of the modules broken by pygame issue #4795;
#: opening a file by path needs none of them.
FONT_FILES = (
    "/usr/share/fonts/truetype/ubuntu/UbuntuMono-R.ttf",
    "/usr/share/fonts/truetype/ubuntu-font-family/UbuntuMono-R.ttf",
    "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",
    "/usr/share/fonts/truetype/liberation/LiberationMono-Regular.ttf",
    "/usr/share/fonts/truetype/freefont/FreeMono.ttf",
)

#: The three text sizes the animation uses, in pixels.
FONT_SIZES = (("small", 12), ("body", 15), ("title", 19))

MARGIN, CELL = 80, 130
MIN_SIZE, MAX_SIZE = (760, 560), (1500, 950)
ZONE_R, DRONE_R = 24, 8

BACKGROUND = (16, 18, 24)
EDGE = (80, 84, 98)
TEXT = (228, 229, 235)
MUTED = (140, 144, 156)

#: Fallback color per zone type, when the map declares none.
BY_TYPE = {
    "normal": (95, 100, 118),
    "priority": (214, 178, 60),
    "restricted": (196, 90, 90),
    "blocked": (46, 40, 44),
}
#: The color names of the maps, as RGB.
BY_NAME = {
    "black": (40, 40, 46), "gray": (150, 150, 158), "grey": (150, 150, 158),
    "red": (214, 73, 73), "darkred": (150, 44, 44), "maroon": (128, 48, 60),
    "crimson": (206, 60, 84), "green": (78, 178, 108), "lime": (162, 214, 74),
    "yellow": (219, 196, 71), "gold": (220, 176, 60), "orange": (222, 148, 63),
    "brown": (150, 106, 76), "blue": (86, 130, 217), "cyan": (72, 190, 199),
    "magenta": (196, 84, 196), "purple": (150, 96, 199),
    "violet": (170, 120, 220), "rainbow": (200, 120, 220),
    "pink": (222, 128, 168), "white": (232, 232, 236),
}


def zone_color(zone: Zone) -> Tuple[int, int, int]:
    """Return the RGB color a zone is drawn with."""
    if zone.color and zone.color.lower() in BY_NAME:
        return BY_NAME[zone.color.lower()]
    return BY_TYPE[zone.zone_type.value]


def drone_color(drone_id: int) -> Tuple[int, int, int]:
    """Return a stable, well-separated RGB color for a drone id."""
    hue = (drone_id * 0.618034) % 1.0    # golden ratio spreads the hues
    red, green, blue = (
        int(min(1.0, max(0.35, abs(((hue + shift) % 1.0) * 6 - 3) - 1)) * 235)
        for shift in (0.0, 2 / 3, 1 / 3)
    )
    return red, green, blue


def drone_offset(drone_id: int) -> Tuple[float, float]:
    """Return a small offset so drones sharing a zone stay readable."""
    angle = drone_id * 2.399963          # golden angle spreads the ring
    radius = 5 + 3 * (drone_id % 4)
    return math.cos(angle) * radius, math.sin(angle) * radius


def _font_path() -> Optional[str]:
    """Return the first installed font of :data:`FONT_FILES`.

    ``None`` means pygame's own bundled font, which every loader
    accepts, and is what happens on a system shipping none of them.
    """
    return next((name for name in FONT_FILES if os.path.exists(name)), None)


class FreetypeFont:
    """Adapts a ``pygame.freetype`` font to the ``pygame.font`` API.

    ``pygame.font`` is unusable on some builds — pygame 2.6 on Python
    3.14 fails to import it — while ``pygame.freetype`` still works.
    Wrapping the second one behind the interface of the first keeps the
    drawing code unaware of which of the two is in use.
    """

    def __init__(self, font: Any) -> None:
        """Wrap one ``pygame.freetype.Font``.

        Args:
            font: The freetype font to delegate rendering to.
        """
        self._font = font

    def render(
        self, text: str, antialias: bool, color: Tuple[int, int, int]
    ) -> Any:
        """Render text to a surface, like ``pygame.font.Font.render``.

        Args:
            text: The string to draw.
            antialias: Ignored; freetype antialiases by default.
            color: Foreground color.
        """
        del antialias
        surface, _rect = self._font.render(text, fgcolor=color)
        return surface


class DroneAnimator:
    """Records the fleet turn by turn, then replays it in a window."""

    def __init__(self, network: Network, drones: List[Drone]) -> None:
        """Capture the initial state of the fleet.

        Args:
            network: The network being simulated.
            drones: The fleet, before the first turn is played.
        """
        self._network = network
        self._frames = [self._snapshot(drones)]
        self._pygame: Any = None
        self._screen: Any = None
        self._fonts: Dict[str, Any] = {}
        self._spots: Dict[str, Tuple[float, float]] = {}

    # -- recording -----------------------------------------------------

    def record(self, turn: int, moves: List[str], simulator: Any) -> None:
        """Store a snapshot; matches the simulator's ``on_turn``.

        Args:
            turn: Turn index, unused but part of the callback shape.
            moves: Moves of that turn, likewise unused.
            simulator: The running simulator, read for drone positions.
        """
        del turn, moves
        self._frames.append(self._snapshot(simulator.drones))

    def _snapshot(self, drones: List[Drone]) -> Frame:
        """Build one frame out of the current fleet positions."""
        frame: Frame = {}
        for drone in drones:
            target = drone.next_zone
            if not drone.is_in_transit or target is None:
                frame[drone.drone_id] = (drone.current_zone, None, 0.0)
                continue
            total = self._network.zones[target].movement_cost
            flown = (total - drone.transit_turns_left) / total
            frame[drone.drone_id] = (drone.current_zone, target, flown)
        return frame

    def _position(self, spot: Spot) -> Tuple[float, float]:
        """Return the pixel position described by one spot."""
        origin, target, flown = spot
        start_x, start_y = self._spots[origin]
        if target is None:
            return start_x, start_y
        end_x, end_y = self._spots[target]
        return (
            start_x + (end_x - start_x) * flown,
            start_y + (end_y - start_y) * flown,
        )

    # -- playback ------------------------------------------------------

    def play(self, ms_per_turn: float, map_name: str = "") -> None:
        """Open a window and replay the recorded frames.

        Args:
            ms_per_turn: Playback speed, in milliseconds per turn.
            map_name: Name shown in the window title.

        Raises:
            SystemExit: If pygame is not installed.
        """
        try:
            import pygame
        except ImportError:  # pragma: no cover - optional dependency
            raise SystemExit(
                "--animate needs the 'pygame' package. Install it with "
                "'make install' or 'pip install pygame'."
            ) from None

        self._pygame = pygame
        try:
            pygame.init()
            width, height = self._window_size()
            self._screen = pygame.display.set_mode((width, height))
            title = f"Fly-in - {map_name}" if map_name else "Fly-in"
            pygame.display.set_caption(title)
        except (pygame.error, NotImplementedError, OSError) as error:
            # No display, no SDL driver, broken install: the mandatory
            # terminal visualization has already been printed, so say
            # what happened and return instead of crashing.
            print(f"--animate: cannot open a window ({error}).",
                  file=sys.stderr)
            return
        self._fonts = self._load_fonts()
        self._spots = self._layout(width, height)

        clock = pygame.time.Clock()
        last = len(self._frames) - 1
        turn, elapsed, speed, paused = 0, 0.0, max(ms_per_turn, 16.0), False

        try:
            while True:
                delta = clock.tick(60)
                action, speed, paused = self._events(speed, paused)
                if action == "quit":
                    break
                if action == "restart":
                    turn, elapsed = 0, 0.0
                if not paused and turn < last:
                    elapsed += delta
                    while elapsed >= speed and turn < last:
                        elapsed -= speed
                        turn += 1
                fraction = min(elapsed / speed, 1.0) if turn < last else 0.0
                self._draw(turn, last, fraction, speed, paused, title)
                pygame.display.flip()
        except (pygame.error, NotImplementedError) as error:
            print(f"--animate: playback stopped ({error}).", file=sys.stderr)
        finally:
            pygame.quit()

    def _load_fonts(self) -> Dict[str, Any]:
        """Return the label fonts, or an empty mapping if none loads.

        ``pygame.font`` is tried first, then ``pygame.freetype``, which
        is a separate module and still works on the builds where the
        first one is broken. Labels are a nicety, not the animation
        itself, so if both fail the discs are simply drawn without their
        names rather than the program stopping.
        """
        broken: Tuple[Type[BaseException], ...] = (
            self._pygame.error, NotImplementedError, ImportError,
            AttributeError, OSError,
        )
        last: Optional[BaseException] = None
        loaders = (self._sys_fonts, self._file_fonts, self._c_freetype_fonts)
        for loader in loaders:
            try:
                with warnings.catch_warnings():
                    # A failing loader warns loudly before raising; the
                    # message we print below says it better.
                    warnings.simplefilter("ignore")
                    return loader()
            except broken as error:
                last = error
        print(f"--animate: text labels disabled ({last}).", file=sys.stderr)
        return {}

    def _sys_fonts(self) -> Dict[str, Any]:
        """Load the fonts by family name, through ``pygame.font``."""
        return {
            size: self._pygame.font.SysFont(FONT_FAMILIES, pixels)
            for size, pixels in FONT_SIZES
        }

    def _file_fonts(self) -> Dict[str, Any]:
        """Load the fonts from a file, through ``pygame.font``.

        Used when ``pygame.font`` itself imported but the family lookup
        did not, since that one goes through ``pygame.sysfont``.
        """
        return {
            size: self._pygame.font.Font(_font_path(), pixels)
            for size, pixels in FONT_SIZES
        }

    def _c_freetype_fonts(self) -> Dict[str, Any]:
        """Load the fonts through the ``pygame._freetype`` C extension.

        pygame issue #4795: on Python 3.14, ``pygame.font``,
        ``pygame.sysfont`` and ``pygame.freetype`` form a circular
        import and all three fail. ``pygame._freetype`` is the compiled
        module the third one re-exports; being C, it imports none of
        them, so it still loads. This is the path that keeps the labels
        alive on an otherwise font-less pygame.
        """
        import pygame._freetype as freetype

        freetype.init()
        return {
            size: FreetypeFont(freetype.Font(_font_path(), pixels))
            for size, pixels in FONT_SIZES
        }

    def _events(self, speed: float, paused: bool) -> Tuple[str, float, bool]:
        """Process one frame of keyboard and window events.

        Returns:
            An ``(action, speed, paused)`` tuple, the action being
            ``"quit"``, ``"restart"`` or ``"none"``.
        """
        pygame = self._pygame
        action = "none"
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                action = "quit"
            elif event.type != pygame.KEYDOWN:
                continue
            elif event.key in (pygame.K_ESCAPE, pygame.K_q):
                action = "quit"
            elif event.key == pygame.K_SPACE:
                paused = not paused
            elif event.key in (pygame.K_UP, pygame.K_EQUALS, pygame.K_PLUS):
                speed = max(60.0, speed * 0.8)
            elif event.key in (pygame.K_DOWN, pygame.K_MINUS):
                speed = min(4000.0, speed * 1.25)
            elif event.key == pygame.K_r:
                action = "restart"
        return action, speed, paused

    # -- layout --------------------------------------------------------

    def _window_size(self) -> Tuple[int, int]:
        """Return a window size fitting the map without being huge."""
        columns = len({zone.x for zone in self._network.zones.values()})
        rows = len({zone.y for zone in self._network.zones.values()})
        return (
            max(MIN_SIZE[0], min(2 * MARGIN + max(columns - 1, 1) * CELL,
                                 MAX_SIZE[0])),
            max(MIN_SIZE[1], min(2 * MARGIN + max(rows - 1, 1) * CELL,
                                 MAX_SIZE[1])),
        )

    def _layout(
        self, width: int, height: int
    ) -> Dict[str, Tuple[float, float]]:
        """Map the zone coordinates of the file onto window pixels."""
        zones = self._network.zones.values()
        min_x, max_x = min(z.x for z in zones), max(z.x for z in zones)
        min_y, max_y = min(z.y for z in zones), max(z.y for z in zones)
        span_x, span_y = max(max_x - min_x, 1), max(max_y - min_y, 1)
        return {
            zone.name: (
                MARGIN + (zone.x - min_x) / span_x * (width - 2 * MARGIN),
                height - MARGIN
                - (zone.y - min_y) / span_y * (height - 2 * MARGIN),
            )
            for zone in zones
        }

    # -- drawing -------------------------------------------------------

    def _draw(
        self,
        turn: int,
        last: int,
        fraction: float,
        speed: float,
        paused: bool,
        title: str,
    ) -> None:
        """Draw one animation frame."""
        self._screen.fill(BACKGROUND)
        self._draw_links()
        self._draw_zones()
        self._draw_drones(
            self._frames[turn], self._frames[min(turn + 1, last)], fraction
        )
        self._draw_hud(turn, last, speed, paused, title)

    def _label(self, text: str, font: str, color: Tuple[int, int, int],
               x: float, y: float, centered: bool = False) -> None:
        """Blit one piece of text, when a font could be loaded."""
        if font not in self._fonts:
            return
        surface = self._fonts[font].render(text, True, color)
        left = x - surface.get_width() / 2 if centered else x
        self._screen.blit(surface, (left, y))

    def _draw_links(self) -> None:
        """Draw every edge, labelling the ones with extra capacity."""
        for link in self._network.connections:
            start = self._spots[link.zone_a]
            end = self._spots[link.zone_b]
            self._pygame.draw.line(self._screen, EDGE, start, end, 3)
            if link.max_link_capacity > 1:
                self._label(
                    str(link.max_link_capacity), "small", MUTED,
                    (start[0] + end[0]) / 2, (start[1] + end[1]) / 2 - 8,
                    centered=True,
                )

    def _draw_zones(self) -> None:
        """Draw every zone as a disc with its name and capacity."""
        draw = self._pygame.draw
        for zone in self._network.zones.values():
            x, y = self._spots[zone.name]
            center = (int(x), int(y))
            draw.circle(self._screen, zone_color(zone), center, ZONE_R)
            border = (255, 255, 255) if zone.is_hub else BACKGROUND
            draw.circle(self._screen, border, center, ZONE_R, 2)
            self._label(zone.name, "small", TEXT, x, y + ZONE_R + 3, True)
            if zone.max_drones != 1 and not zone.is_hub:
                self._label(
                    f"x{zone.max_drones}", "small", MUTED, x, y - 7, True
                )

    def _draw_drones(
        self, previous: Frame, following: Frame, fraction: float
    ) -> None:
        """Draw every drone, interpolated between two frames."""
        draw = self._pygame.draw
        for drone_id, spot in following.items():
            offset_x, offset_y = drone_offset(drone_id)
            from_x, from_y = self._position(previous.get(drone_id, spot))
            to_x, to_y = self._position(spot)
            x = from_x + (to_x - from_x) * fraction + offset_x
            y = from_y + (to_y - from_y) * fraction + offset_y
            center = (int(x), int(y))
            draw.circle(self._screen, drone_color(drone_id), center, DRONE_R)
            draw.circle(self._screen, BACKGROUND, center, DRONE_R, 1)
            self._label(f"D{drone_id}", "small", TEXT, x, y - DRONE_R - 14,
                        True)

    def _draw_hud(
        self, turn: int, last: int, speed: float, paused: bool, title: str
    ) -> None:
        """Draw the title, the turn counter and the key bindings."""
        state = "PAUSED" if paused else f"{speed:.0f} ms/turn"
        self._label(title, "title", TEXT, 14, 10)
        self._label(f"Turn {turn}/{last}   {state}", "body", MUTED, 14, 34)
        self._label(
            "space: pause   up/down: speed   r: restart   esc: quit",
            "small", MUTED, 14, self._screen.get_height() - 22,
        )
