#!/usr/bin/env python3
"""Entry point called with python3 a_maze_ing.py config.txt"""

import random
import sys
from dataclasses import replace

from app.config import MazeConfigError, read_config
from app.terminal import (CARVE_EVERY, SEEN_EVERY, WALL_COLOURS, Drawing,
                          TooSmallTerminal, animate, apply_wall_colour,
                          cursor_hidden, read_key, redraw,
                          seen_frames, show_colour_menu)
from mazegen import MazeConfig, MazeGenerator, MazeSolver, forty_two

TITLE = "A-MAZE-ING"

ANIMATION = True


def message_for(config: MazeConfig, algorithm: str, solver: str,
                animating: bool, fps: int) -> list[str]:
    state = "on" if animating else "off"
    return [
        f"{algorithm} / {solver}  {config.width}x{config.height} {fps} fps",
        "(no room for 42)" if not forty_two.fits(config.width, config.height)
        else "",
        f"R  redraw   P  path   C  colour   A  animate ({state})   Q  quit",
    ]


def in_terminal(config: MazeConfig, generator: MazeGenerator,
                solver: MazeSolver, output_file: str, fps: int) -> int:
    """Draw the maze in the terminal, and run until the user quits."""
    global ANIMATION

    if generator.config.seed is None:
        generator.config.seed = random.randrange(1 << 30)
    maze = generator.generate()
    path = solver.solve(maze)
    maze.save(output_file, path)

    drawing = Drawing(title=TITLE)
    drawing.message = message_for(
        config, generator.name, solver.name, ANIMATION, fps)

    if not sys.stdin.isatty():
        return 0

    with cursor_hidden():
        if ANIMATION:
            animate(generator.carve(), replace(drawing, paths=()),
                    fps=fps, every=CARVE_EVERY)
            maze = generator.maze

        while True:
            drawing.message = message_for(
                config, generator.name, solver.name, ANIMATION, fps)
            redraw(maze, drawing)

            key = read_key()
            #
            # EXIT
            #
            if key in ("q", "Q", "\x03", "\x1b"):
                break
            #
            # ANIMATION TOGGLE
            #
            if key in ("a", "A"):
                ANIMATION = not ANIMATION
                if drawing.paths:
                    drawing.visited = (
                        solver.get_visited() if ANIMATION else ())
            #
            # MAZE GENERATION
            #
            if key in ("r", "R"):
                generator.config.seed = random.randrange(1 << 30)
                drawing.visited = ()
                if ANIMATION:
                    animate(generator.carve(), replace(drawing, paths=()),
                            fps=fps, every=CARVE_EVERY)
                    maze = generator.maze
                else:
                    maze = generator.generate()
                path = solver.solve(maze)
                maze.save(output_file, path)
                if drawing.paths:
                    drawing.paths = (path,)
                    drawing.visited = (
                        solver.get_visited() if ANIMATION else ())
            #
            # SHOW SHORTEST PATH
            #
            if key in ("p", "P"):
                if drawing.paths:
                    drawing.paths = ()
                    drawing.visited = ()
                else:
                    drawing.visited = (
                        solver.get_visited() if ANIMATION else ())
                    drawing.paths = (path,)
                    if ANIMATION:
                        animate(seen_frames(maze, replace(drawing,
                                                          paths=())),
                                fps=fps, every=SEEN_EVERY,
                                reveal=False)
                        animate((maze,), drawing, fps=fps)
            #
            # COLORS
            #
            if key in ("c", "C"):
                show_colour_menu(maze, drawing)
                choice = read_key()
                if choice in "12345":
                    apply_wall_colour(
                        drawing, WALL_COLOURS[int(choice) - 1])

    return 0


def main() -> int:
    """Read the config, then draw the maze in the terminal."""
    arguments = sys.argv[1:]
    if len(arguments) != 1:
        print("Usage: python3 a_maze_ing.py config.txt", file=sys.stderr)
        return 1

    try:
        config, output_file, fps = read_config(arguments[0])
        generator = MazeGenerator.from_config(config)
        solver = MazeSolver.from_config(config)
        return in_terminal(config, generator, solver, output_file, fps)
    except (MazeConfigError, ValueError, OSError, TooSmallTerminal) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    raise SystemExit(main())
