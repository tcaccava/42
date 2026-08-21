"""Plans the routes of the whole fleet, once, before the first turn.

Two halves of the same question, "which drone flies where":
:meth:`RoutePlanner._find_routes` collects a handful of diverse
start-to-end routes, :meth:`RoutePlanner.assign` spreads the drones
over them.
"""

from typing import List, Optional, Set, Tuple

from src.exceptions import NoPathFoundError
from src.models.network import Network
from src.pathfinding.dijkstra import Dijkstra

#: Capacity used for zones and links that have no meaningful limit.
_UNLIMITED = 10 ** 6


class RoutePlanner:
    """Finds candidate routes and shares the fleet between them.

    Alternatives are derived by forbidding, one at a time, an
    intermediate zone of the best route and running Dijkstra again — a
    simplified variant of Yen's k-shortest-paths algorithm needing no
    external library.

    Complexity: ``O(R * (V + E) log V)`` with ``R = max_routes``,
    computed once before the simulation starts.
    """

    def __init__(self, network: Network, max_routes: int = 4) -> None:
        """Compute the candidate routes of a network.

        Args:
            network: The parsed network.
            max_routes: How many candidate routes to keep at most.

        Raises:
            NoPathFoundError: If the end hub cannot be reached.
            ValueError: If ``max_routes`` is not strictly positive.
            KeyError: If the network has no start or end hub.
        """
        if max_routes < 1:
            raise ValueError("max_routes must be a positive integer")
        if network.start_zone is None or network.end_zone is None:
            raise KeyError("the network has no start or end hub")

        self._network = network
        self._max_routes = max_routes
        self._start = network.start_zone.name
        self._end = network.end_zone.name
        self.routes: List[List[str]] = self._find_routes()

    def _find_routes(self) -> List[List[str]]:
        """Return the candidate routes, cheapest first.

        Raises:
            NoPathFoundError: If the end hub cannot be reached, which
                also covers disconnected graphs and graphs whose only
                routes cross a blocked zone.
        """
        dijkstra = Dijkstra(self._network)
        best, best_cost = dijkstra.shortest(self._start, self._end)
        if best is None:
            raise NoPathFoundError(
                f"no route exists between '{self._start}' and "
                f"'{self._end}' (disconnected or blocked graph)"
            )

        found: List[Tuple[int, List[str]]] = [(best_cost, best)]
        seen: Set[Tuple[str, ...]] = {tuple(best)}
        for zone_name in best[1:-1]:
            if len(found) >= self._max_routes:
                break
            route, cost = dijkstra.shortest(
                self._start, self._end, avoid={zone_name}
            )
            if route is None or tuple(route) in seen:
                continue
            seen.add(tuple(route))
            found.append((cost, route))

        found.sort(key=lambda entry: (entry[0], len(entry[1])))
        return [route for _, route in found[: self._max_routes]]

    def capacity_of(self, route: List[str]) -> int:
        """Return how many drones a route can carry in parallel.

        This is the route's bottleneck: the smallest ``max_drones`` or
        ``max_link_capacity`` found along it. The two hubs are skipped,
        since the subject exempts them from occupancy limits.

        Args:
            route: Zone names from the start hub to the end hub.
        """
        capacity = _UNLIMITED
        for zone_name in route[1:-1]:
            capacity = min(capacity, self._network.zones[zone_name].max_drones)
        for zone_a, zone_b in zip(route, route[1:]):
            link = self._network.link(zone_a, zone_b)
            if link is not None:
                capacity = min(capacity, link.max_link_capacity)
        return capacity

    def assign(self, nb_drones: int) -> List[List[str]]:
        """Give every drone one of the candidate routes.

        Drones are shared out with a weighted round-robin: each turn the
        route currently least used *relative to its own bottleneck*
        takes the next drone. A wide route therefore receives
        proportionally more drones than a narrow one, which is what
        maximises the number of drones moving per turn.

        Args:
            nb_drones: Number of drones to place.

        Returns:
            One route per drone, in drone-id order.
        """
        weights = [max(1, self.capacity_of(route)) for route in self.routes]
        used = [0] * len(self.routes)
        fleet: List[List[str]] = []

        for _ in range(nb_drones):
            index = min(
                range(len(self.routes)),
                key=lambda i: (used[i] + 1) / weights[i],
            )
            used[index] += 1
            fleet.append(list(self.routes[index]))
        return fleet

    def suffixes_from(
        self, zone_name: str, excluding: Optional[str]
    ) -> List[List[str]]:
        """Return the candidate routes leaving a zone, cheapest first.

        Used by the simulator when a drone's own next hop is busy: it
        may slip onto another *already validated* route instead of
        waiting, so rerouting can never invent an illegal edge.

        Args:
            zone_name: The zone the drone currently sits in.
            excluding: A first hop to skip, namely the one that is busy.

        Returns:
            Route suffixes starting at ``zone_name``, ordered by
            cheapest remaining cost.
        """
        options: List[Tuple[int, List[str]]] = []
        for route in self.routes:
            if zone_name not in route:
                continue
            index = route.index(zone_name)
            suffix = route[index:]
            if len(suffix) < 2 or suffix[1] == excluding:
                continue
            remaining = sum(
                self._network.zones[name].movement_cost
                for name in suffix[1:]
            )
            options.append((remaining, suffix))

        options.sort(key=lambda option: option[0])
        return [suffix for _, suffix in options]
