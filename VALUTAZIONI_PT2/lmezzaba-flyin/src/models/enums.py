"""The four zone types defined by the subject."""

from enum import Enum

#: Turns needed to enter a zone, per type. Blocked zones are absent:
#: they can never be entered.
_COSTS = {"normal": 1, "priority": 1, "restricted": 2}

#: Cost used for blocked zones, large enough to make any accidental
#: use in a sum obviously non-viable instead of silently valid.
_UNREACHABLE = 10 ** 9


class ZoneType(Enum):
    """A zone type, together with its movement semantics."""

    NORMAL = "normal"
    BLOCKED = "blocked"
    RESTRICTED = "restricted"
    PRIORITY = "priority"

    @property
    def movement_cost(self) -> int:
        """Return how many turns entering such a zone costs."""
        return _COSTS.get(self.value, _UNREACHABLE)

    @property
    def is_traversable(self) -> bool:
        """Return True when drones may enter such a zone."""
        return self is not ZoneType.BLOCKED

    @property
    def is_preferred(self) -> bool:
        """Return True when pathfinding should favour such a zone."""
        return self is ZoneType.PRIORITY

    @classmethod
    def from_string(cls, value: str) -> "ZoneType":
        """Convert a raw ``zone=`` metadata value into a member.

        Args:
            value: The string read from the map file.

        Raises:
            ValueError: If the value is not one of the four types.
        """
        try:
            return cls(value)
        except ValueError:
            valid = ", ".join(member.value for member in cls)
            raise ValueError(
                f"invalid zone type '{value}' (expected: {valid})"
            ) from None
