"""Reads a map file and turns it into a :class:`Network`.

Every line is shaped as ``keyword: body [metadata]``, so parsing is
three steps: split the keyword, peel off the optional brackets, then
hand the body to one small handler per keyword. Plain string operations
(``partition``, ``split``) only, no regular expression.
"""

from typing import Dict, FrozenSet, Optional, Tuple

from src.exceptions import MapParsingError
from src.models.connection import Connection
from src.models.enums import ZoneType
from src.models.network import Network
from src.models.zone import Zone

ZONE_KEYWORDS = ("start_hub", "end_hub", "hub")
ZONE_TAGS = frozenset({"zone", "color", "max_drones"})
LINK_TAGS = frozenset({"max_link_capacity"})

#: Characters a zone name may never contain. Spaces are impossible
#: anyway, since a name is one whitespace-separated token.
BANNED_IN_NAMES = "-[]#"


class MapParser:
    """Turns the lines of a map file into a network and a drone count."""

    def parse(self, filepath: str) -> Tuple[Network, int]:
        """Read and validate a map file.

        Args:
            filepath: Path to the map description file.

        Returns:
            A ``(network, nb_drones)`` tuple.

        Raises:
            MapParsingError: If the file breaks a rule of the subject.
            OSError: If the file cannot be opened or read.
        """
        try:
            with open(filepath, "r", encoding="utf-8") as handle:
                lines = handle.readlines()
        except UnicodeDecodeError:
            raise MapParsingError(
                0, "the file is not valid UTF-8 text (binary file?)"
            ) from None

        self._network = Network()
        self._nb_drones = 0
        self._count_line = 0

        for number, raw in enumerate(lines, start=1):
            statement = self._read_line(raw, number)
            if statement is None:
                continue
            keyword, body, tags = statement
            if keyword == "nb_drones":
                self._read_drone_count(body, tags, number)
            elif keyword == "connection":
                self._read_connection(body, tags, number)
            elif keyword in ZONE_KEYWORDS:
                self._read_zone(keyword, body, tags, number)
            else:
                raise MapParsingError(
                    number, f"unknown instruction '{keyword}:'"
                )

        self._check_completeness()
        return self._network, self._nb_drones

    # ------------------------------------------------------------------
    # Steps 1 and 2: shape of a line
    # ------------------------------------------------------------------

    def _read_line(
        self, raw: str, number: int
    ) -> Optional[Tuple[str, str, str]]:
        """Split one raw line into keyword, body and raw metadata.

        Args:
            raw: The line as read from the file.
            number: Its 1-based line number.

        Returns:
            A ``(keyword, body, raw_metadata)`` tuple, or None when the
            line is blank or a comment and must simply be skipped.

        Raises:
            MapParsingError: If the line has no ``keyword:`` prefix.
        """
        line = raw.strip()
        if not line or line.startswith("#"):
            return None
        keyword, colon, rest = line.partition(":")
        if not colon:
            raise MapParsingError(
                number,
                f"'{line}' is not an instruction "
                "(expected '<keyword>: <value>')",
            )
        body, tags = self._split_metadata(rest.strip(), number)
        return keyword.strip(), body, tags

    @staticmethod
    def _split_metadata(rest: str, number: int) -> Tuple[str, str]:
        """Peel the optional ``[...]`` block off the end of a line.

        Args:
            rest: Everything that follows the ``keyword:`` prefix.
            number: Line number, used for error reporting.

        Returns:
            A ``(body, raw_metadata)`` tuple; the metadata is empty when
            no bracket block is present.

        Raises:
            MapParsingError: If the brackets are unbalanced or misplaced.
        """
        if "[" not in rest and "]" not in rest:
            return rest, ""
        body, bracket, tags = rest.partition("[")
        if not bracket or not tags.endswith("]"):
            raise MapParsingError(
                number, f"malformed metadata block in '{rest}'"
            )
        tags = tags[:-1]
        if "[" in tags or "]" in tags:
            raise MapParsingError(
                number, f"nested brackets in the metadata of '{rest}'"
            )
        return body.strip(), tags

    @staticmethod
    def _read_tags(
        raw_tags: str, allowed: FrozenSet[str], number: int
    ) -> Dict[str, str]:
        """Parse a metadata block into a dictionary.

        Tags may appear in any order, as the subject allows.

        Args:
            raw_tags: The text found between the square brackets.
            allowed: The tag names accepted in this context.
            number: Line number, used for error reporting.

        Raises:
            MapParsingError: If a tag is malformed, repeated or unknown.
        """
        tags: Dict[str, str] = {}
        for token in raw_tags.split():
            key, equals, value = token.partition("=")
            if not equals or not key or not value:
                raise MapParsingError(
                    number, f"invalid tag '{token}' (expected 'key=value')"
                )
            if key in tags:
                raise MapParsingError(number, f"repeated tag '{key}'")
            if key not in allowed:
                expected = ", ".join(sorted(allowed))
                raise MapParsingError(
                    number, f"unknown tag '{key}' (expected: {expected})"
                )
            tags[key] = value
        return tags

    # ------------------------------------------------------------------
    # Step 3: one handler per keyword
    # ------------------------------------------------------------------

    def _read_drone_count(
        self, body: str, raw_tags: str, number: int
    ) -> None:
        """Read the ``nb_drones:`` declaration.

        Raises:
            MapParsingError: If it is malformed or declared twice.
        """
        if raw_tags:
            raise MapParsingError(number, "'nb_drones:' takes no metadata")
        if self._count_line:
            raise MapParsingError(
                number,
                "'nb_drones:' is declared twice "
                f"(already on line {self._count_line})",
            )
        self._nb_drones = self._positive_int(body, "nb_drones", number)
        self._count_line = number

    def _read_zone(
        self, keyword: str, body: str, raw_tags: str, number: int
    ) -> None:
        """Read a ``start_hub:``, ``end_hub:`` or ``hub:`` declaration.

        Raises:
            MapParsingError: If it is malformed or duplicated.
        """
        self._require_drone_count(number)
        fields = body.split()
        if len(fields) != 3:
            raise MapParsingError(
                number,
                f"'{keyword}: {body}' should be "
                f"'{keyword}: <name> <x> <y>'",
            )
        name, x_text, y_text = fields
        self._check_zone_name(name, number)
        if name in self._network.zones:
            raise MapParsingError(number, f"duplicate zone name '{name}'")

        is_start = keyword == "start_hub"
        is_end = keyword == "end_hub"
        if is_start and self._network.start_zone is not None:
            raise MapParsingError(number, "several 'start_hub:' zones")
        if is_end and self._network.end_zone is not None:
            raise MapParsingError(number, "several 'end_hub:' zones")

        tags = self._read_tags(raw_tags, ZONE_TAGS, number)
        try:
            zone_type = ZoneType.from_string(tags.get("zone", "normal"))
        except ValueError as error:
            raise MapParsingError(number, str(error)) from None

        # max_drones must be well formed even on the two hubs, where the
        # subject says it is ignored rather than rejected.
        max_drones = 1
        if "max_drones" in tags:
            max_drones = self._positive_int(
                tags["max_drones"], "max_drones", number
            )

        self._network.add_zone(
            Zone(
                name=name,
                x=self._integer(x_text, "x", number),
                y=self._integer(y_text, "y", number),
                zone_type=zone_type,
                color=tags.get("color"),
                max_drones=max_drones,
                is_start=is_start,
                is_end=is_end,
            )
        )

    def _read_connection(
        self, body: str, raw_tags: str, number: int
    ) -> None:
        """Read a ``connection:`` declaration.

        Raises:
            MapParsingError: If it is malformed, duplicated, or refers
                to a zone that has not been defined above it.
        """
        self._require_drone_count(number)
        zone_a, dash, zone_b = body.partition("-")
        if not dash or not zone_a or not zone_b:
            raise MapParsingError(
                number,
                f"'connection: {body}' should be "
                "'connection: <zone1>-<zone2>'",
            )
        if zone_a == zone_b:
            raise MapParsingError(
                number, f"a zone cannot connect to itself ('{zone_a}')"
            )
        for zone_name in (zone_a, zone_b):
            if zone_name not in self._network.zones:
                raise MapParsingError(
                    number,
                    f"unknown zone '{zone_name}': a connection may only "
                    "link zones defined above it",
                )
        if self._network.link(zone_a, zone_b) is not None:
            raise MapParsingError(
                number,
                f"duplicate connection between '{zone_a}' and '{zone_b}'",
            )

        tags = self._read_tags(raw_tags, LINK_TAGS, number)
        capacity = 1
        if "max_link_capacity" in tags:
            capacity = self._positive_int(
                tags["max_link_capacity"], "max_link_capacity", number
            )
        self._network.add_connection(Connection(zone_a, zone_b, capacity))

    # ------------------------------------------------------------------
    # Shared checks
    # ------------------------------------------------------------------

    def _require_drone_count(self, number: int) -> None:
        """Ensure ``nb_drones:`` came first, as the subject demands.

        Raises:
            MapParsingError: If it has not been declared yet.
        """
        if not self._count_line:
            raise MapParsingError(
                number,
                "the file must start with "
                "'nb_drones: <positive_integer>'",
            )

    def _check_completeness(self) -> None:
        """Run the checks that only make sense on the whole file.

        Raises:
            MapParsingError: If a mandatory element is missing or if a
                hub zone would be unusable.
        """
        if not self._count_line:
            raise MapParsingError(
                0, "missing 'nb_drones: <positive_integer>' line"
            )
        for zone, keyword in (
            (self._network.start_zone, "start_hub"),
            (self._network.end_zone, "end_hub"),
        ):
            if zone is None:
                raise MapParsingError(0, f"no '{keyword}:' zone is defined")
            if not zone.is_traversable:
                raise MapParsingError(
                    0, f"the {keyword} zone '{zone.name}' cannot be blocked"
                )

    @staticmethod
    def _check_zone_name(name: str, number: int) -> None:
        """Reject a zone name using a forbidden character.

        Raises:
            MapParsingError: If the name contains one of them.
        """
        for char in BANNED_IN_NAMES:
            if char in name:
                raise MapParsingError(
                    number,
                    f"invalid zone name '{name}': "
                    f"'{BANNED_IN_NAMES}' and spaces are forbidden",
                )

    @staticmethod
    def _integer(text: str, field: str, number: int) -> int:
        """Parse a possibly negative integer field.

        Raises:
            MapParsingError: If the text is not an integer.
        """
        digits = text[1:] if text.startswith("-") else text
        if not digits.isdigit():
            raise MapParsingError(
                number, f"{field} must be an integer, got '{text}'"
            )
        return int(text)

    @staticmethod
    def _positive_int(text: str, field: str, number: int) -> int:
        """Parse a strictly positive integer field.

        Raises:
            MapParsingError: If the text is not a positive integer.
        """
        if not text.isdigit() or int(text) <= 0:
            raise MapParsingError(
                number,
                f"{field} must be a positive integer, got '{text}'",
            )
        return int(text)
