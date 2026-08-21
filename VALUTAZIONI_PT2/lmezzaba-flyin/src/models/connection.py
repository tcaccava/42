"""The Connection model: a bidirectional edge between two zones."""

from dataclasses import dataclass


@dataclass(frozen=True)
class Connection:
    """A connection between two zones.

    Like :class:`~src.models.zone.Zone`, it is frozen: the graph never
    changes once parsed.

    Attributes:
        zone_a: First endpoint, as written in the map file.
        zone_b: Second endpoint, as written in the map file.
        max_link_capacity: Drones allowed to cross it during one turn.
    """

    zone_a: str
    zone_b: str
    max_link_capacity: int = 1

    @property
    def name(self) -> str:
        """Return the identifier used by the output format."""
        return f"{self.zone_a}-{self.zone_b}"

    def other(self, zone_name: str) -> str:
        """Return the zone at the other end of the connection.

        Args:
            zone_name: One of the two endpoints.

        Raises:
            ValueError: If the zone is not an endpoint.
        """
        if zone_name == self.zone_a:
            return self.zone_b
        if zone_name == self.zone_b:
            return self.zone_a
        raise ValueError(f"'{zone_name}' is not an end of {self.name}")
