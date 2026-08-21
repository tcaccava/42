"""Exception hierarchy of the Fly-in project.

Every error the program can report inherits from :class:`FlyInError`,
so the command-line layer catches a single type.
"""


class FlyInError(Exception):
    """Base class for every error raised by the simulator."""


class MapParsingError(FlyInError):
    """Raised when a map file breaks a rule of the subject."""

    def __init__(self, line_number: int, message: str) -> None:
        """Build an error mentioning the offending line, if any.

        Args:
            line_number: 1-based line number, or 0 for file-wide errors.
            message: Explanation of what is wrong.
        """
        self.line_number = line_number
        prefix = f"line {line_number}: " if line_number else ""
        super().__init__(prefix + message)


class NoPathFoundError(FlyInError):
    """Raised when the end zone cannot be reached from the start zone."""


class SimulationError(FlyInError):
    """Raised when the fleet cannot progress any further."""


class ConfigurationError(FlyInError):
    """Raised when a command-line option holds an unusable value."""
