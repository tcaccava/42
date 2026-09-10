"""Prints the palette of colors, kind by kind. After installation can
be excecuted with

   $ python3 -m rainbow.

The colors are printed in five groups: the vivid colours, the dusty
ones, the pale ones, the tinted greys and the greys, each running
through the spectrum: red, orange, yellow, green, turquoise, blue,
violet, magenta.

Useful when needing to pick a color.
"""

from . import name, write
from .palette import BASIC, GROUPS, groups, spectrum

WHAT = {
    "saturated": "vivid — a hue at full strength, dark ones included",
    "muted": "dusty — a hue with grey stirred through it",
    "clear": "pale — a hue with white stirred through it",
    "tinted": "tinted greys — a grey with a hint of a colour in it",
    "grey": "no hue at all, from near-black to near-white",
}


def main() -> None:
    """Print each kind of colour as its own ribbon."""
    kinds = groups()
    for kind in GROUPS:
        _title(f"{kind} · {WHAT[kind]}")
        for number in spectrum(kinds[kind]):
            _line(number, name(number))

    _title("the sixteen the terminal owns · whatever the theme says "
           "they are, so they may look different on another screen")
    for number, label in enumerate(BASIC):
        _line(number, label)
    print()


def _title(text: str) -> None:
    """Print a heading between the ribbons."""
    print(write(f"\n  {text}\n", ink="snow"))


def _line(number: int, label: str) -> None:
    """Print one colour: a stripe of it, its name, its number."""
    print(f"   {write('          ', paper=number)}  {label:<13}{number:>4}")


if __name__ == "__main__":
    main()
