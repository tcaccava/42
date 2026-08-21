"""The Drone model: one drone flying along its assigned route."""

from typing import List, Optional


class Drone:
    """A drone, and the route it is following.

    The whole state is derived from three private fields, so no two
    pieces of information can ever disagree:

    * ``_path``   - the zone names from the start hub to the end hub;
    * ``_index``  - where the drone currently stands inside that path;
    * ``_transit``- the connection being crossed, or None when parked.
    """

    def __init__(self, drone_id: int, path: List[str]) -> None:
        """Place a drone at the beginning of its route.

        Args:
            drone_id: 1-based identifier shown in the output (``D<id>``).
            path: Zone names from the start hub to the end hub.
        """
        self._drone_id = drone_id
        self._path = list(path)
        self._index = 0
        self._transit: Optional[str] = None
        self._turns_left = 0

    # -- identity ------------------------------------------------------

    @property
    def drone_id(self) -> int:
        """Return the numeric identifier of the drone."""
        return self._drone_id

    @property
    def label(self) -> str:
        """Return the ``D<id>`` label used by the output format."""
        return f"D{self._drone_id}"

    # -- position ------------------------------------------------------

    @property
    def current_zone(self) -> str:
        """Return the zone occupied, or departed from when in flight."""
        return self._path[self._index]

    @property
    def next_zone(self) -> Optional[str]:
        """Return the next zone of the route, or None at the end."""
        if self._index + 1 >= len(self._path):
            return None
        return self._path[self._index + 1]

    @property
    def remaining_steps(self) -> int:
        """Return how many hops still separate the drone from the goal."""
        return len(self._path) - self._index

    # -- state ---------------------------------------------------------

    @property
    def is_in_transit(self) -> bool:
        """Return True while the drone is crossing a connection."""
        return self._transit is not None

    @property
    def is_delivered(self) -> bool:
        """Return True once the drone stands on the end hub."""
        return self._transit is None and self._index == len(self._path) - 1

    @property
    def is_parked(self) -> bool:
        """Return True while the drone waits inside an intermediate zone."""
        return not self.is_in_transit and not self.is_delivered

    @property
    def transit_connection(self) -> Optional[str]:
        """Return the connection being crossed, or None when parked."""
        return self._transit

    @property
    def transit_turns_left(self) -> int:
        """Return how many turns remain before the drone lands."""
        return self._turns_left

    # -- movement ------------------------------------------------------

    def reroute(self, suffix: List[str]) -> None:
        """Replace the rest of the route with an equivalent one.

        Args:
            suffix: A route starting at the drone's current zone and
                ending at the end hub. It always comes from the
                pre-computed candidates, so it is a valid, loop-free
                start-to-end route.
        """
        self._path = self._path[: self._index] + list(suffix)

    def enter_connection(self, connection_name: str, turns: int) -> None:
        """Send the drone onto a connection for a multi-turn move.

        Args:
            connection_name: Connection reported in the output.
            turns: Turns to spend in flight before landing.
        """
        self._transit = connection_name
        self._turns_left = turns

    def advance(self) -> None:
        """Land on the next zone of the route."""
        self._index += 1
        self._transit = None
        self._turns_left = 0

    def fly_one_turn(self) -> bool:
        """Spend one turn in flight.

        Returns:
            True when the drone lands this turn, False when it is still
            crossing the connection.
        """
        self._turns_left -= 1
        if self._turns_left > 0:
            return False
        self.advance()
        return True
