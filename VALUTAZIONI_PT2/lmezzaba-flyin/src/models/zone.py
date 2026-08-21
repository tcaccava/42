"""The Zone model: a node of the network."""

from dataclasses import dataclass
from typing import Optional

from src.models.enums import ZoneType


@dataclass(frozen=True)
class Zone:
    """A zone of the network.

    The dataclass is frozen: once the parser has built a zone, nothing
    can alter it, so the graph stays a trustworthy read-only description
    of the map while the simulation runs.

    Attributes:
        name: Unique zone name (no dash, no space).
        x: Abscissa, used by the visual representation.
        y: Ordinate, used by the visual representation.
        zone_type: Movement semantics of the zone.
        color: Color declared in the map file, if any.
        max_drones: Occupancy limit, ignored on the two hubs.
        is_start: True for the unique ``start_hub`` zone.
        is_end: True for the unique ``end_hub`` zone.
    """

    name: str
    x: int
    y: int
    zone_type: ZoneType = ZoneType.NORMAL
    color: Optional[str] = None
    max_drones: int = 1
    is_start: bool = False
    is_end: bool = False

    @property
    def is_hub(self) -> bool:
        """Return True for the start and end zones.

        Those two have no occupancy limit: every drone starts in the
        first one and any number of them may be delivered to the second.
        """
        return self.is_start or self.is_end

    @property
    def movement_cost(self) -> int:
        """Return how many turns entering this zone costs."""
        return self.zone_type.movement_cost

    @property
    def is_traversable(self) -> bool:
        """Return True when drones may enter this zone."""
        return self.zone_type.is_traversable

    def accepts(self, occupancy: int) -> bool:
        """Return True when one more drone may enter this zone.

        Args:
            occupancy: Drones currently occupying or reserving the zone.
        """
        return self.is_hub or occupancy < self.max_drones
