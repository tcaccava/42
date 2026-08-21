"""Custom Dijkstra shortest-path search.

Implemented from scratch on the standard library's binary heap, as the
subject requires: no ``networkx``, ``graphlib`` or any other graph
library is used.
"""

import heapq
from typing import Dict, List, Optional, Set, Tuple

from src.models.network import Network

#: Returned instead of a cost when no route exists.
NO_ROUTE = -1


class Dijkstra:
    """Finds the cheapest route between two zones.

    A move costs the movement cost of the zone being *entered*, so
    ``restricted`` zones are naturally twice as expensive. Blocked zones
    are never expanded, so no returned route can cross one.

    Each heap entry carries a second key, the number of non-priority
    hops. It breaks ties in favour of routes using more ``priority``
    zones, which is how the subject's "priority zones should be
    preferred in pathfinding" requirement is honoured.

    Complexity: ``O((V + E) log V)`` time, ``O(V)`` memory.
    """

    def __init__(self, network: Network) -> None:
        """Bind the search to a network.

        Args:
            network: The graph to search in.
        """
        self._network = network

    def shortest(
        self, start: str, end: str, avoid: Optional[Set[str]] = None
    ) -> Tuple[Optional[List[str]], int]:
        """Search the cheapest route from ``start`` to ``end``.

        Args:
            start: Name of the zone to leave from.
            end: Name of the zone to reach.
            avoid: Zones the route must not use. Forbidding one zone at
                a time is how alternative routes are derived.

        Returns:
            A ``(route, cost)`` tuple, the route being the zone names
            from ``start`` to ``end`` included. When the end zone cannot
            be reached, ``(None, NO_ROUTE)`` is returned.
        """
        banned: Set[str] = avoid or set()
        if start in banned or end in banned:
            return None, NO_ROUTE

        # best[zone] = (cost so far, non-priority hops so far)
        best: Dict[str, Tuple[int, int]] = {start: (0, 0)}
        came_from: Dict[str, str] = {}
        queue: List[Tuple[int, int, str]] = [(0, 0, start)]
        settled: Set[str] = set()

        while queue:
            cost, plain_hops, name = heapq.heappop(queue)
            if name in settled:
                continue
            settled.add(name)
            if name == end:
                return self._rebuild(came_from, start, end), cost

            for link in self._network.neighbours(name):
                neighbour_name = link.other(name)
                if neighbour_name in banned or neighbour_name in settled:
                    continue
                neighbour = self._network.zones[neighbour_name]
                if not neighbour.is_traversable:
                    continue
                candidate = (
                    cost + neighbour.movement_cost,
                    plain_hops + (0 if neighbour.zone_type.is_preferred
                                  else 1),
                )
                known = best.get(neighbour_name)
                if known is None or candidate < known:
                    best[neighbour_name] = candidate
                    came_from[neighbour_name] = name
                    next_cost, next_hops = candidate
                    heapq.heappush(
                        queue, (next_cost, next_hops, neighbour_name)
                    )

        return None, NO_ROUTE

    @staticmethod
    def _rebuild(
        came_from: Dict[str, str], start: str, end: str
    ) -> List[str]:
        """Walk the predecessor map backwards to rebuild a route."""
        route = [end]
        while route[-1] != start:
            route.append(came_from[route[-1]])
        route.reverse()
        return route
