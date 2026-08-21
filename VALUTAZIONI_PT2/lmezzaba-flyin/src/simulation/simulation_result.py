"""Outcome of a simulation run: the move log and its statistics."""

from dataclasses import dataclass
from typing import List


@dataclass(frozen=True)
class SimulationResult:
    """The moves played, turn by turn, plus a few derived metrics.

    Attributes:
        turns: One list of ``D<id>-<target>`` tokens per turn.
        nb_drones: Number of drones that were routed.
        total_cost: Sum of every move cost, in turns.
    """

    turns: List[List[str]]
    nb_drones: int
    total_cost: int

    @property
    def total_turns(self) -> int:
        """Return the number of simulation turns, i.e. the score."""
        return len(self.turns)

    @property
    def lines(self) -> List[str]:
        """Return the log in the exact output format of the subject."""
        return [" ".join(moves) for moves in self.turns]

    @property
    def moves_per_turn(self) -> float:
        """Return the average number of drone moves per turn."""
        if not self.turns:
            return 0.0
        return sum(len(moves) for moves in self.turns) / len(self.turns)

    @property
    def turns_per_drone(self) -> float:
        """Return the average number of turns spent per drone."""
        if not self.nb_drones:
            return 0.0
        return self.total_cost / self.nb_drones
