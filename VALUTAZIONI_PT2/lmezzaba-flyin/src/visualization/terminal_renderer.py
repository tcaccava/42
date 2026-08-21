"""Prints the network, the moves and the statistics to the terminal."""

from typing import Dict, List, Optional

from src.models.network import Network
from src.simulation.simulation_result import SimulationResult
from src.visualization.colors import ColorPalette

#: One glyph per zone type, shown in the legend.
GLYPHS = {"normal": ".", "priority": "+", "restricted": "!", "blocked": "x"}

#: Zones printed per legend line.
PER_LINE = 4


class TerminalRenderer:
    """Renders a run using the colors declared in the map file."""

    def __init__(self, network: Network, use_color: bool = True) -> None:
        """Build a renderer.

        Args:
            network: The network being simulated.
            use_color: False disables every ANSI sequence.
        """
        self._network = network
        self._palette = ColorPalette(enabled=use_color)

    def header(self, nb_drones: int) -> None:
        """Print the network overview and the zone legend.

        Args:
            nb_drones: Number of drones about to be routed.
        """
        network = self._network
        start = network.start_zone
        end = network.end_zone
        if start is None or end is None:          # never, the parser
            return                                # rejects such a map
        print(self._palette.bold("=== Fly-in drone routing simulation ==="))
        print(
            f"Zones: {len(network.zones)} | "
            f"Connections: {len(network.connections)} | "
            f"Drones: {nb_drones}"
        )
        print(
            f"Start: {self._palette.paint(start.name, start.color)}"
            f"   ->   End: {self._palette.paint(end.name, end.color)}"
        )
        print(self._palette.bold(
            "Zones (. normal  + priority  ! restricted  x blocked):"
        ))
        for line in self._legend():
            print(f"  {line}")
        print()

    def _legend(self) -> List[str]:
        """Return the legend lines, a few zones each."""
        entries: List[str] = []
        for zone in self._network.zones.values():
            glyph = GLYPHS[zone.zone_type.value]
            room = "inf" if zone.is_hub else str(zone.max_drones)
            entries.append(
                self._palette.paint(f"{glyph} {zone.name}({room})", zone.color)
            )
        return [
            "  ".join(entries[start:start + PER_LINE])
            for start in range(0, len(entries), PER_LINE)
        ]

    def turn(
        self,
        number: int,
        moves: List[str],
        zone_load: Optional[Dict[str, int]] = None,
    ) -> None:
        """Print every move played during one turn, plus the zone states.

        Args:
            number: 1-based turn index.
            moves: The ``D<id>-<target>`` tokens of that turn.
            zone_load: Drones occupying each zone after the turn. When
                given, the occupied zones are shown after the moves, so
                the reader sees which ones are saturated.
        """
        if not moves:
            return
        painted = " ".join(self._paint_move(move) for move in moves)
        state = self._zone_states(zone_load)
        print(f"{self._palette.bold(f'Turn {number:>3}:')} {painted}{state}")

    def _zone_states(self, zone_load: Optional[Dict[str, int]]) -> str:
        """Return the ``[zone used/max]`` suffix of a turn line."""
        if not zone_load:
            return ""
        busy = [
            self._palette.paint(
                f"{zone.name} {zone_load.get(zone.name, 0)}/{zone.max_drones}",
                zone.color,
            )
            for zone in self._network.zones.values()
            if not zone.is_hub and zone_load.get(zone.name, 0)
        ]
        return f"   [{', '.join(busy)}]" if busy else ""

    def _paint_move(self, move: str) -> str:
        """Colorize one ``D<id>-<target>`` token with its zone color."""
        label, _, target = move.partition("-")
        zone = self._network.zones.get(target)
        if zone is None:
            # The target is a connection: use the color of the zone it
            # leads to, which is the restricted one being flown towards.
            link = self._network.link_named(target)
            zone = self._network.zones.get(link.zone_b) if link else None
        color = zone.color if zone else None
        return f"{label}-{self._palette.paint(target, color)}"

    def summary(self, result: SimulationResult) -> None:
        """Print the final statistics of a run.

        Args:
            result: The outcome of the simulation.
        """
        print()
        print(self._palette.bold("=== Summary ==="))
        print(f"Total turns (score) : {result.total_turns}")
        print(f"Drones delivered    : {result.nb_drones}")
        print(f"Total path cost     : {result.total_cost}")
        print(f"Moves per turn      : {result.moves_per_turn:.2f}")
        print(f"Average turns/drone : {result.turns_per_drone:.2f}")
