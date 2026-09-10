"""One module per generation algorithm."""

from .kruskal import Kruskal
from .backtracker import Backtracker
from .division import Division

__all__ = ["Kruskal", "Backtracker", "Division"]
