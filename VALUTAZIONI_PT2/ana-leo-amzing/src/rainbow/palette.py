"""The 256 colours a terminal knows, and a name for every one of them.

A terminal's palette is three ranges, and each is built differently:

- **0-15** the sixteen colours terminals have always had. They are the
  only ones with real names, and they are the only ones a user can
  change in their terminal settings — so the same number can look
  different on someone else's screen.
- **16-231** a 6x6x6 cube: every mix of six levels of red, six of
  green and six of blue. The levels are not evenly spaced; they run
  0, 95, 135, 175, 215, 255, which is why the darkest step is a big
  jump and the rest are gentle.
- **232-255** a grey ramp, twenty-four steps from near-black to
  near-white, finer than the greys the cube can make.

The names here are worked out from the colours themselves rather than
copied from a list: each cube colour is placed in a family by its hue,
and numbered within that family from the one that puts out the least
light to the one that puts out the most. That way there is no table to
keep correct, and "blue1 is the darkest blue" is true by construction.
"""

import colorsys

# The six levels a cube colour is mixed from.
LEVELS = (0, 95, 135, 175, 215, 255)

# The only colours with names of their own, in their usual order.
BASIC = (
    "black", "maroon", "green", "olive", "navy", "purple", "teal",
    "silver", "grey", "red", "lime", "yellow", "blue", "fuchsia",
    "aqua", "white",
)

# Where a hue falls, in degrees, and what that family is called.
FAMILIES = (
    (16, "red"), (45, "orange"), (70, "gold"), (100, "chartreuse"),
    (140, "green"), (170, "spring"), (200, "cyan"), (230, "azure"),
    (260, "blue"), (290, "violet"), (320, "magenta"), (345, "pink"),
    (361, "red"),
)

# A name for every colour in the cube, borrowed from the pigments and
# paints these colours are: the earths, the minerals, the dyes, and the
# plain words painters have always used for them. One list per family,
# in the order the family runs — the vivid ones first, then the ones
# with grey or white worked into them.
#
# A family with more colours than names falls back to its family name
# and a number, so the palette is never left with a colour it cannot
# be asked for. Nothing here is guessed at run time: these are the
# names, and the order they sit in is the order the family is sorted
# in, which is why a list can simply be written down beside it.
PIGMENTS: dict[str, tuple[str, ...]] = {
    "red": (
        "vermilion", "scarlet", "oxblood", "carmine", "madder",
        "rosemadder", "salmon", "garnet", "brick", "terracotta",
        "clay", "blush", "shellpink", "russet", "dustyrose",
        "ashrose", "petal",
    ),
    "orange": (
        "minium", "tangerine", "marigold", "persimmon", "sienna",
        "umber", "rust", "apricot", "buff", "adobe", "camel",
        "sandstone", "peach", "melon", "linen",
    ),
    "gold": (
        "citron", "gamboge", "cadmium", "ochre", "orpiment", "bronze",
        "amber", "brass", "saffron", "lemon", "drab", "khaki", "flax",
        "straw", "butter", "vanilla", "moss", "hemp", "parchment",
        "cream",
    ),
    "chartreuse": (
        "chartreuse", "springbud", "citrine", "grass", "apple", "pear",
        "sapgreen", "fern", "olivine", "sprout", "lemongrass",
        "lichen", "meadow", "willow", "sage", "pistachio", "honeydew",
        "linden",
    ),
    "green": (
        "malachite", "emerald", "forest", "shamrock", "leaf", "jade",
        "lawn", "pine", "laurel", "clover", "eucalyptus", "peppermint",
        "seafoam", "terreverte", "artichoke", "celadon", "frost",
    ),
    "spring": (
        "verdigris", "jadeite", "mint", "beryl", "viridian",
        "seagreen", "hooker", "kelly", "phthalo", "spearmint",
        "glacier", "sea", "peridot", "lagoon", "wintergreen",
        "sorbet", "mist", "seaglass",
    ),
    "cyan": (
        "cerulean", "sky", "turquoise", "caribbean", "manganese",
        "peacock", "prussian", "spruce", "petrol", "aegean", "arctic",
        "iceberg", "abyss", "harbour", "delft", "shallows", "seamist",
        "powder", "glass", "vapour", "marsh", "fog", "duckegg", "hoar",
    ),
    "azure": (
        "ultramarine", "azure", "cobalt", "lapis", "sapphire",
        "cornflower", "robin", "denim", "steel", "chambray",
        "periwinkle", "cloud",
    ),
    "blue": (
        "indanthrone", "smalt", "midnight", "admiral", "iris",
        "hyacinth", "deepsea", "dusk", "bluebell", "wisteria", "lupin",
        "thistle", "slate", "pewter", "haze", "moonstone",
    ),
    "violet": (
        "violet", "amethyst", "heliotrope", "dioxazine", "grape",
        "orchid", "aubergine", "royal", "byzantium", "lilac", "crocus",
        "mauve", "veronica", "foxglove", "lavender", "campanula",
        "opal", "chiffon",
    ),
    "magenta": (
        "magenta", "purpurin", "quinacridone", "mauveine", "tyrian",
        "phlox", "mulberry", "damson", "beetroot", "plum", "hibiscus",
        "flamingo", "blackberry", "heather", "azalea", "peony",
        "candy", "bonbon", "prune", "mallow", "tulle", "blossom",
    ),
    "pink": (
        "rose", "cerise", "raspberry", "cyclamen", "burgundy",
        "watermelon", "rosewood", "strawberry", "carnation",
        "camellia", "bubblegum", "sweetpea", "ballet",
    ),
    "stone": (
        "lampblack", "graphite", "zinc", "dove", "chalk", "titanium",
    ),
}

# The twenty-four steps of the grey ramp, darkest first. Painters have
# more words for grey than for anything else, which is lucky.
GREYS: tuple[str, ...] = (
    "cinder", "soot", "charcoal", "onyx", "ironblack", "basalt",
    "gunmetal", "granite", "shadow", "anthracite", "flint", "ash",
    "smoke", "stone", "concrete", "putty", "mushroom", "oyster",
    "pearl", "bone", "porcelain", "alabaster", "ivory", "snow",
)


def rgb_of(number: int) -> tuple[int, int, int]:
    """Return the red, green and blue a colour number is made of.

    0-255 each, as the terminal will draw them. The sixteen basic
    colours are the terminal's own and can be changed by whoever is
    using it; the values here are the usual ones.
    """
    if not 0 <= number <= 255:
        raise ValueError(f"a colour is 0 to 255, got {number}")
    if number < 16:
        return _BASIC_RGB[number]
    if number < 232:
        rest = number - 16
        return (LEVELS[rest // 36], LEVELS[rest // 6 % 6], LEVELS[rest % 6])
    return (8 + (number - 232) * 10,) * 3


_BASIC_RGB = (
    (0, 0, 0), (128, 0, 0), (0, 128, 0), (128, 128, 0),
    (0, 0, 128), (128, 0, 128), (0, 128, 128), (192, 192, 192),
    (128, 128, 128), (255, 0, 0), (0, 255, 0), (255, 255, 0),
    (0, 0, 255), (255, 0, 255), (0, 255, 255), (255, 255, 255),
)


def _family(number: int) -> str:
    """Return the family a cube colour belongs to, by its hue."""
    red, green, blue = rgb_of(number)
    hue, lightness, saturation = colorsys.rgb_to_hls(
        red / 255, green / 255, blue / 255
    )
    if saturation < 0.15:
        # Too little colour to belong anywhere: it is a grey.
        return "stone"
    degrees = hue * 360
    for edge, name in FAMILIES:
        if degrees < edge:
            return name
    return "red"


def _light(number: int) -> int:
    """Return how much light a colour puts out, 0 to 765.

    The three parts added up — the plainest measure there is, and
    enough to put colours of one purity in order from dark to pale.
    """
    return sum(rgb_of(number))


def _purity(number: int) -> int:
    """Return how far a colour is from grey, in five steps.

    The distance between its strongest part and its weakest: a colour
    with a part at 255 and a part at 0 is as pure as the palette gets,
    and one whose three parts are equal is a grey with no colour left
    in it at all. Rounding to five steps groups the colours that are
    about as vivid as each other, so ``_light`` can then order those
    from dark to pale.
    """
    red, green, blue = rgb_of(number)
    return round((max(red, green, blue) - min(red, green, blue)) / 255 * 5)


def _shade(number: int) -> tuple[int, int]:
    """Return where a colour belongs within its family.

    Purest first, and among colours of the same purity, darkest first.
    So a family runs from vivid to washed out, and the greyed and pale
    ones gather at the end where they are easy to skip past.
    """
    return -_purity(number), _light(number)


# The families in the order a rainbow runs through them. Names and
# printing both follow this, so the two can never disagree.
FAMILY_ORDER = ("red", "orange", "gold", "chartreuse", "green", "spring",
                "cyan", "azure", "blue", "violet", "magenta", "pink")


def cube_families() -> dict[str, list[int]]:
    """Group the cube by family, each family sorted vivid to washed out.

    The single place a colour is put in a family, so that the name it
    gets and the place it prints in are worked out the same way. A
    colour with too little hue to belong anywhere lands in "stone".
    """
    families: dict[str, list[int]] = {}
    for number in range(16, 232):
        families.setdefault(_family(number), []).append(number)
    for members in families.values():
        members.sort(key=_shade)
    return families


def _build() -> dict[str, int]:
    """Work out a name for every colour, once, when this file loads."""
    names: dict[str, int] = {name: number
                             for number, name in enumerate(BASIC)}
    for family, members in cube_families().items():
        pigments = PIGMENTS.get(family, ())
        for index, number in enumerate(members):
            names[pigments[index] if index < len(pigments)
                  else f"{family}{index + 1}"] = number
    for index, number in enumerate(range(232, 256)):
        names[GREYS[index] if index < len(GREYS)
              else f"grey{index}"] = number
    return names


NAMES: dict[str, int] = _build()

NUMBERS: dict[int, str] = {number: name for name, number in NAMES.items()}


def distance(one: tuple[int, int, int],
             two: tuple[int, int, int]) -> float:
    """Return how different two colours look, roughly.

    Plain distance between red, green and blue treats a change in
    green as no more important than a change in blue, and the eye does
    not. This weights the three the way the eye weighs them, more or
    less: green counts most, blue least, and red more when both
    colours are already reddish. It is the "redmean" approximation,
    and it is good enough for picking one colour out of 256 — the
    alternative is a proper colour space, which is a great deal of
    arithmetic for the same answer.
    """
    red_one, green_one, blue_one = one
    red_two, green_two, blue_two = two
    mean_red = (red_one + red_two) / 2
    return (
        (2 + mean_red / 256) * (red_one - red_two) ** 2
        + 4 * (green_one - green_two) ** 2
        + (2 + (255 - mean_red) / 256) * (blue_one - blue_two) ** 2
    )


# The five kinds of colour the palette holds, in the order they are
# worth looking through: the vivid ones first, then the dusty ones,
# then the pale ones, then the ones that are barely colours at all,
# and last the greys.
GROUPS = ("saturated", "muted", "clear", "tinted", "grey")


def group_of(number: int) -> str:
    """Return which kind of colour this is.

    Everything is decided from the three parts a colour is mixed from:
    the strongest, the weakest, and the gap between them.

    - **grey** no gap at all: no colour left to speak of.
    - **tinted** a gap so small against the strength of the colour
      that what shows is a grey with a hint of something in it —
      ``(175, 135, 135)`` is not a red, it is a warm grey. These sit
      apart because they are the ones that make a ribbon of proper
      colours look muddled.
    - **saturated** most of the strongest part missing from the
      weakest: the hue at full strength, dark ones included, since
      ``(0, 0, 135)`` is a deep navy and not a dusty one.
    - **clear** plenty of light in the weakest part — that is white
      stirred through the mix — so, the pastels.
    - **muted** what is left: a hue with grey through it.
    """
    red, green, blue = rgb_of(number)
    top, bottom = max(red, green, blue), min(red, green, blue)
    gap = top - bottom
    if gap == 0:
        return "grey"
    if gap / top < 0.30:
        return "tinted"
    if gap / top >= 0.65:
        return "saturated"
    return "clear" if bottom >= 135 else "muted"


def groups() -> dict[str, list[int]]:
    """Return every colour of the palette, sorted into its kind.

    The sixteen the terminal owns are left out: their real colours
    depend on whoever's terminal it is.
    """
    kinds: dict[str, list[int]] = {kind: [] for kind in GROUPS}
    for number in range(16, 256):
        kinds[group_of(number)].append(number)
    return kinds


def spectrum(numbers: list[int] | None = None) -> list[int]:
    """Return those colours in the order of the spectrum.

    Red first, then orange, yellow, green, turquoise, blue, violet,
    and round through magenta to red again — the order a rainbow runs
    in, which is the order people look for a colour in. Colours of the
    same hue follow each other from dark to light.

    Sorting by hue only reads well because the kinds are kept apart
    first (see ``groups``): within one kind the colours at a given hue
    differ by little else, so the list fades instead of jumping.
    """
    chosen = list(range(16, 256) if numbers is None else numbers)
    return sorted(chosen, key=lambda number: (_hue(number),
                                              _light(number)))


def _hue(number: int) -> float:
    """Return where a colour falls on the wheel, red at the start.

    Greys have no hue at all — the answer would be meaningless, so
    they are given 0 and sorted by lightness alone.
    """
    red, green, blue = rgb_of(number)
    if max(red, green, blue) == min(red, green, blue):
        return 0.0
    return colorsys.rgb_to_hls(red / 255, green / 255, blue / 255)[0]
