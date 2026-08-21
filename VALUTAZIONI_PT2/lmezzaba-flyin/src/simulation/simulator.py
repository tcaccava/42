"""The turn engine: moves the whole fleet, one turn at a time."""

from typing import Callable, Dict, List, Optional, Set, Tuple

from src.exceptions import SimulationError
from src.models.connection import Connection
from src.models.drone import Drone
from src.models.network import Network
from src.pathfinding.route_planner import RoutePlanner
from src.simulation.simulation_result import SimulationResult

#: Signature of the per-turn callback: ``(turn, moves, simulator)``.
TurnCallback = Callable[[int, List[str], "Simulator"], None]

#: Hard safety net; a real deadlock is caught long before this.
MAX_TURNS = 100_000


class Simulator:
    """Routes a fleet of drones from the start hub to the end hub.

    Before the first turn, :class:`~src.pathfinding.route_planner.
    RoutePlanner` computes a few diverse routes and shares the fleet
    between them. **No pathfinding ever runs again afterwards.**

    Each turn is then played in two phases: drones already in flight
    first, then drones parked in a zone, closest-to-the-goal first. The
    detailed rules behind each phase are commented in :meth:`_play_turn`
    and :meth:`_step`.

    Complexity: ``O(turns * nb_drones * R)`` with ``R`` the small,
    bounded number of candidate routes; every capacity check is a
    dictionary lookup, so memory stays ``O(V + E + nb_drones)``.
    """

    def __init__(
        self, network: Network, nb_drones: int, max_routes: int = 4
    ) -> None:
        """Plan the routes and place the fleet on the start hub.

        Args:
            network: The parsed network.
            nb_drones: Number of drones to route.
            max_routes: How many candidate routes to pre-compute.

        Raises:
            NoPathFoundError: If the end hub cannot be reached.
            ValueError: If ``nb_drones`` or ``max_routes`` is invalid.
        """
        if nb_drones < 1:
            raise ValueError("nb_drones must be a positive integer")

        self.network = network
        self._nb_drones = nb_drones
        self._planner = RoutePlanner(network, max_routes)
        self.drones = [
            Drone(index + 1, route)
            for index, route in enumerate(self._planner.assign(nb_drones))
        ]
        self._zone_load: Dict[str, int] = {name: 0 for name in network.zones}
        self._link_load: Dict[str, int] = {
            link.name: 0 for link in network.connections
        }

    @property
    def zone_load(self) -> Dict[str, int]:
        """Return how many drones occupy each zone right now.

        Exposed read-only so the visualization can render zone states
        without reaching into the engine's bookkeeping.
        """
        return dict(self._zone_load)

    # ------------------------------------------------------------------
    # Main loop
    # ------------------------------------------------------------------

    def run(
        self,
        on_turn: Optional[TurnCallback] = None,
        max_turns: int = MAX_TURNS,
    ) -> SimulationResult:
        """Play turns until every drone has been delivered.

        Args:
            on_turn: Optional callback receiving the turn number, the
                moves of that turn and this simulator.
            max_turns: Safety net against pathological maps.

        Returns:
            The complete :class:`SimulationResult`.

        Raises:
            SimulationError: On a deadlock, if the safety net trips, or
                if a route contains an edge the network does not have.
        """
        log: List[List[str]] = []
        total_cost = 0
        turn = 0
        flying = [drone for drone in self.drones if not drone.is_delivered]

        while flying:
            turn += 1
            if turn > max_turns:
                raise SimulationError(
                    f"stopped after {max_turns} turns: the map is very "
                    "likely unsolvable"
                )

            moves, cost = self._play_turn(flying)
            if not moves:
                raise SimulationError(
                    f"deadlock at turn {turn}: none of the {len(flying)} "
                    "remaining drone(s) can move; the capacities of this "
                    "map make the fleet undeliverable"
                )

            total_cost += cost
            log.append(moves)
            if on_turn is not None:
                on_turn(turn, moves, self)
            flying = [d for d in self.drones if not d.is_delivered]

        return SimulationResult(log, self._nb_drones, total_cost)

    def _play_turn(self, flying: List[Drone]) -> Tuple[List[str], int]:
        """Play one turn and return its ``(moves, cost)``.

        Raises:
            SimulationError: If a route contains a missing edge.
        """
        moves: List[str] = []
        cost = 0
        booked: Dict[str, int] = {}
        landed_on: List[str] = []

        # Closest to the goal first: they free the bottlenecks they hold
        # before the drones queueing behind them ask for the room.
        order = sorted(
            flying, key=lambda drone: (drone.remaining_steps, drone.drone_id)
        )
        acted: Set[int] = set()

        # Phase 1: drones already crossing a connection tick down and
        # land; they can never linger mid-air. Marked as having acted,
        # so they cannot also take a fresh move in the same turn.
        for drone in order:
            link_name = drone.transit_connection
            if link_name is None:
                continue                               # not in flight
            acted.add(drone.drone_id)
            cost += 1
            if drone.fly_one_turn():
                landed_on.append(link_name)
                moves.append(f"{drone.label}-{drone.current_zone}")
            else:
                moves.append(f"{drone.label}-{link_name}")

        # Phase 2: parked drones take at most one step, or wait.
        for drone in order:
            if drone.drone_id in acted or not drone.is_parked:
                continue
            move = self._step(drone, booked)
            if move is not None:
                moves.append(move)
                cost += 1

        # A connection stays busy for the whole crossing: its slot is
        # freed only once the arrival turn has been fully played.
        for link_name in landed_on:
            self._link_load[link_name] -= 1

        return moves, cost

    # ------------------------------------------------------------------
    # One drone, one step
    # ------------------------------------------------------------------

    def _step(self, drone: Drone, booked: Dict[str, int]) -> Optional[str]:
        """Move one parked drone by a single step, or let it wait.

        Args:
            drone: The drone to move.
            booked: Crossings already granted this turn, per link name.

        Returns:
            The ``D<id>-<target>`` token, or None when the drone waits.

        Raises:
            SimulationError: If the route contains a missing edge.
        """
        target = drone.next_zone
        if target is None:
            return None
        link = self.network.link(drone.current_zone, target)
        if link is None:
            raise SimulationError(
                f"{drone.label}: no connection between "
                f"'{drone.current_zone}' and '{target}'"
            )

        if not self._has_room(target, link, booked):
            detour = self._reroute(drone, booked)
            if detour is None:
                return None                    # strategic wait
            target, link = detour

        zone = self.network.zones[target]
        self._leave(drone.current_zone)
        self._occupy(target)

        if zone.movement_cost > 1:
            # Restricted zone: book the destination right away, since
            # the drone may not wait on the connection for a free slot.
            self._link_load[link.name] += 1
            drone.enter_connection(link.name, zone.movement_cost - 1)
            return f"{drone.label}-{link.name}"

        booked[link.name] = booked.get(link.name, 0) + 1
        drone.advance()
        return f"{drone.label}-{target}"

    def _reroute(
        self, drone: Drone, booked: Dict[str, int]
    ) -> Optional[Tuple[str, Connection]]:
        """Find a free first hop on another candidate route.

        Args:
            drone: The blocked drone.
            booked: Crossings already granted this turn.

        Returns:
            The ``(target_zone, connection)`` to use, the drone having
            been spliced onto that route, or None if nothing is free.
        """
        here = drone.current_zone
        for suffix in self._planner.suffixes_from(here, drone.next_zone):
            target = suffix[1]
            link = self.network.link(here, target)
            if link is not None and self._has_room(target, link, booked):
                drone.reroute(suffix)
                return target, link
        return None

    # ------------------------------------------------------------------
    # Capacity bookkeeping
    # ------------------------------------------------------------------

    def _has_room(
        self, zone_name: str, link: Connection, booked: Dict[str, int]
    ) -> bool:
        """Return True when a step can be taken right now."""
        zone_ok = self.network.zones[zone_name].accepts(
            self._zone_load[zone_name]
        )
        used = self._link_load[link.name] + booked.get(link.name, 0)
        return zone_ok and used < link.max_link_capacity

    def _leave(self, zone_name: str) -> None:
        """Free one occupancy slot of a zone."""
        if not self.network.zones[zone_name].is_hub:
            self._zone_load[zone_name] -= 1

    def _occupy(self, zone_name: str) -> None:
        """Book one occupancy slot of a zone."""
        if not self.network.zones[zone_name].is_hub:
            self._zone_load[zone_name] += 1
