#!/usr/bin/env python3
"""Entry point of the Fly-in drone routing simulator.

Usage:
    ./main.py maps/easy/01_linear_path.txt
    python3 main.py maps/hard/01_maze_nightmare.txt --quiet
"""

import sys

from src.cli import main

if __name__ == "__main__":
    sys.exit(main())
