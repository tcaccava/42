# rainbow

Colour for the terminal, chosen by name instead of by number.

A terminal can draw in 256 colours, and it knows them all as numbers.
`38;5;196` is a fine thing for a terminal to read and a poor thing for
a person to write: nothing about `196` says *red*, and nothing about
`58` warns you it is the dark olive that will vanish into a dark
background. This package gives every one of the 256 a name taken from
the pigment it is — `vermilion`, `ochre`, `terreverte`, `drab` — and
lets you ask for that.

It is the second package under `src/`, beside `mazegen`. The maze
renderer draws with it. It is kept in the repository rather than
installed from elsewhere so that the project stands on its own:
`pip install -e .` is still all anyone needs.

## Seeing the colours

```bash
PYTHONPATH=src python3 -m rainbow
```

Prints the whole palette as five ribbons — a stripe of each colour,
its name, its number. This is the command to reach for when picking a
colour, which is most of what anyone wants from a palette.

`PYTHONPATH=src` is needed only when running from a checkout that has
not been installed; after `pip install -e .` the bare `python3 -m
rainbow` works.

## Using it

```python
>>> from rainbow import ink, paper, write
>>> ink("vermilion")
'38;5;196'
>>> paper("blackberry")
'48;5;53'
>>> print(write("careful", ink="vermilion", paper="soot"))
```

`ink` colours the symbol, `paper` colours the ground behind it, and
both return the bare escape body so they can be built into a longer
sequence. `write` is the one that returns finished text, wrapped in
the escape and closed with a reset.

Neither colour is required. `write("plain")` comes back untouched, so
it can be called on everything without asking first whether there is a
colour to apply.

Numbers still work everywhere a name does: `ink(196)` is `ink("vermilion")`.

A name that is not in the palette is an error rather than a silent
default, and the message suggests the nearest spellings — a colour
asked for by name is usually a colour typed from memory:

```python
>>> ink("vermillion")
UnknownColour: no colour called 'vermillion', did you mean vermilion or watermelon?
```

Names are read forgivingly: case, spaces, hyphens and underscores are
all ignored, and `gray` is read as `grey`.

## Going the other way

```python
>>> from rainbow import nearest, name, rgb_of
>>> name(nearest(18, 52, 86))
'basalt'
>>> rgb_of(196)
(255, 0, 0)
```

`nearest` is for when a colour is known as a value rather than as a
name — taken from a design, an image, a website. The distance is
weighted the way the eye weighs red, green and blue rather than as
plain arithmetic, so the answer is the colour that *looks* closest.
The sixteen the terminal owns are skipped, for the reason below.

## What the 256 actually are

Three ranges, built three different ways:

| numbers | what they are |
| --- | --- |
| 0–15 | the sixteen colours terminals have always had |
| 16–231 | a 6x6x6 cube: six levels each of red, green and blue |
| 232–255 | a grey ramp, 24 steps from near-black to near-white |

The **sixteen are the ones to be careful with**. They are the only
ones a user can change in their own terminal settings, so `red` may
not be red on someone else's screen. They are left out of `nearest`
and out of the five ribbons for that reason, and printed at the end of
`python3 -m rainbow` under a heading that says so.

The cube's six levels are not evenly spaced — they run 0, 95, 135,
175, 215, 255 — which is why the step from the darkest is a jump and
the rest are gentle.

## Where the names come from

They are worked out from the colours themselves, not copied from a
list. Each cube colour is placed in a family by its hue, then ordered
within that family from the one that puts out the least light to the
one that puts out the most, and the names for that family are written
down beside it in that order. There is no table to keep correct, and
"blue1 is the darkest blue" is true by construction.

Thirteen families: red, orange, gold, chartreuse, green, spring, cyan,
azure, blue, violet, magenta, pink, stone. A family with more colours
than names falls back to its own name and a number, so the palette is
never left with a colour that cannot be asked for — as it happens
every colour currently has a real name, and none of the fallbacks are
in use.

## The five kinds

`python3 -m rainbow` groups the palette before showing it, because a
single ribbon of 240 colours sorted by hue reads as mud. Every colour
is sorted by the three parts it is mixed from — the strongest, the
weakest, and the gap between them:

| kind | count | what it is |
| --- | --- | --- |
| `saturated` | 90 | the hue at full strength, dark ones included |
| `muted` | 54 | a hue with grey stirred through it |
| `clear` | 42 | a hue with white stirred through it — the pastels |
| `tinted` | 24 | a grey with a hint of a colour in it |
| `grey` | 30 | no hue at all |

`tinted` is kept apart deliberately: `(175, 135, 135)` is not a red,
it is a warm grey, and left among the reds it is what makes a ribbon
look muddled.

Within each kind the colours run in the order of a rainbow — red,
orange, yellow, green, turquoise, blue, violet, and round through
magenta to red again — which is the order people look for a colour in.
Sorting by hue only reads well *because* the kinds are separated
first: within one kind the colours at a given hue differ by little
else, so the list fades instead of jumping.

## The whole surface

```python
from rainbow import (
    ink, paper, write,          # colour some text
    number, name, nearest,      # names, numbers, and the way back
    rgb_of,                     # what a colour is made of
    NAMES, NUMBERS,             # the two lookups, both directions
    RESET, UnknownColour,       # what ends a run, what a bad name raises
)
```

`NAMES` maps name to number, `NUMBERS` maps number to name, and both
hold all 256.

`rainbow.palette` holds the machinery underneath: `groups`,
`group_of`, `spectrum`, `distance`, `cube_families`, and the constants
`GROUPS`, `BASIC`, `LEVELS`, `FAMILIES`, `PIGMENTS`.
