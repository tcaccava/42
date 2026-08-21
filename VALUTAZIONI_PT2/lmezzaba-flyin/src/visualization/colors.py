"""Turns the free-form color names of a map file into ANSI codes."""

from typing import Optional

#: Colour name -> ANSI foreground code. The subject allows any
#: single-word name, so an unknown one is simply rendered plain.
ANSI = {
    "black": "90", "gray": "90", "grey": "90",
    "red": "31", "darkred": "31", "maroon": "31", "crimson": "31",
    "green": "32", "lime": "92",
    "yellow": "33", "gold": "33", "orange": "33", "brown": "33",
    "blue": "34",
    "magenta": "35", "purple": "35", "violet": "35",
    "pink": "95", "rainbow": "95",
    "cyan": "36",
    "white": "37",
}

RESET = "\033[0m"
BOLD = "\033[1m"


class ColorPalette:
    """Wraps text in ANSI sequences, or leaves it alone."""

    def __init__(self, enabled: bool = True) -> None:
        """Build a palette.

        Args:
            enabled: When False every method returns the plain text,
                which is what ``--no-color`` relies on.
        """
        self._enabled = enabled

    def paint(self, text: str, color: Optional[str]) -> str:
        """Colorize text with a color name coming from the map file.

        Args:
            text: The text to render.
            color: The declared color, if any.

        Returns:
            The text, wrapped in ANSI codes when the color is known.
        """
        code = ANSI.get(color.lower()) if color else None
        if not self._enabled or code is None:
            return text
        return f"\033[{code}m{text}{RESET}"

    def bold(self, text: str) -> str:
        """Render text in bold, when colors are enabled."""
        return f"{BOLD}{text}{RESET}" if self._enabled else text
