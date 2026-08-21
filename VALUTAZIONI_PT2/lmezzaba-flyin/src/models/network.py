"""The Network model: the custom graph of zones and connections.

The graph is built from scratch with plain dictionaries, as the subject
requires: no ``networkx``, ``graphlib`` or any other graph library is
imported anywhere in the project.
"""

from typing import Dict, FrozenSet, List, Optional

from src.models.connection import Connection
from src.models.zone import Zone


class Network:
    """An undirected graph of zones joined by connections.

    Three dictionaries back the graph, each answering one question in
    constant time:

    * ``zones``          - which zone carries this name?
    * ``_neighbours``    - which connections touch this zone?
    * ``_by_endpoints``  - which connection joins these two zones?
    """

    def __init__(self) -> None:
        """Build an empty network."""
        self.zones: Dict[str, Zone] = {}
        self.connections: List[Connection] = []
        self._neighbours: Dict[str, List[Connection]] = {}
        self._by_endpoints: Dict[FrozenSet[str], Connection] = {}
        self._by_name: Dict[str, Connection] = {}
        self.start_zone: Optional[Zone] = None
        self.end_zone: Optional[Zone] = None

    def add_zone(self, zone: Zone) -> None:
        """Insert a zone; the parser guarantees its name is unique."""
        self.zones[zone.name] = zone
        self._neighbours.setdefault(zone.name, [])
        if zone.is_start:
            self.start_zone = zone
        if zone.is_end:
            self.end_zone = zone

    def add_connection(self, connection: Connection) -> None:
        """Insert an edge; the parser guarantees both ends exist."""
        self.connections.append(connection)
        self._neighbours[connection.zone_a].append(connection)
        self._neighbours[connection.zone_b].append(connection)
        self._by_endpoints[
            frozenset((connection.zone_a, connection.zone_b))
        ] = connection
        self._by_name[connection.name] = connection

    def neighbours(self, zone_name: str) -> List[Connection]:
        """Return every connection touching a zone."""
        return self._neighbours.get(zone_name, [])

    def link(self, zone_a: str, zone_b: str) -> Optional[Connection]:
        """Return the connection joining two zones, or None."""
        return self._by_endpoints.get(frozenset((zone_a, zone_b)))

    def link_named(self, name: str) -> Optional[Connection]:
        """Return the connection with that ``zone_a-zone_b`` name."""
        return self._by_name.get(name)
