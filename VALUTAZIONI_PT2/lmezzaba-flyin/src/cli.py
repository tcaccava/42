"""Command-line interface of the Fly-in drone routing simulator."""

import argparse
import importlib.util
import os
import sys
from typing import Any, List, Optional

from src.exceptions import ConfigurationError, FlyInError, MapParsingError
from src.parsing.map_parser import MapParser
from src.simulation.simulator import Simulator
from src.visualization.terminal_renderer import TerminalRenderer

OK, ERROR, INTERRUPTED = 0, 1, 130

DEFAULT_ROUTES = 4
DEFAULT_SPEED_MS = 600.0


def build_parser() -> argparse.ArgumentParser:
    """Build the command-line parser."""
    parser = argparse.ArgumentParser(
        prog="main.py",
        description="Route a fleet of drones from the start hub to the "
                    "end hub, in the fewest possible simulation turns.",
    )
    parser.add_argument("map_file", help="path to the map description file")
    parser.add_argument(
        "--quiet", action="store_true",
        help="print only the raw turn lines, i.e. the exact output "
             "format of the subject",
    )
    parser.add_argument(
        "--no-color", action="store_true",
        help="disable colored terminal output",
    )
    parser.add_argument(
        "--max-paths", type=int, default=DEFAULT_ROUTES, metavar="N",
        help="alternative routes pre-computed to spread the fleet "
             f"(default: {DEFAULT_ROUTES})",
    )
    parser.add_argument(
        "--animate", action="store_true",
        help="open a pygame window replaying the flight (needs pygame)",
    )
    parser.add_argument(
        "--speed", type=float, default=DEFAULT_SPEED_MS, metavar="MS",
        help="milliseconds per turn in the --animate window "
             f"(default: {DEFAULT_SPEED_MS:.0f})",
    )
    return parser


def main(argv: Optional[List[str]] = None) -> int:
    """Run the simulator and return the process exit code.

    Args:
        argv: Command-line arguments; ``sys.argv[1:]`` when omitted.

    Returns:
        ``0`` on success, ``1`` on a handled error, ``130`` on Ctrl-C.
    """
    args = build_parser().parse_args(argv)
    try:
        return _simulate(args)
    except MapParsingError as error:
        print(f"Error in '{args.map_file}': {error}", file=sys.stderr)
    except FlyInError as error:
        print(f"Error: {error}", file=sys.stderr)
    except (ValueError, KeyError) as error:
        print(f"Error: {error}", file=sys.stderr)
    except BrokenPipeError:
        # The reader closed the pipe first, as in `./main.py map | head`.
        # Redirect stdout to devnull so the interpreter stays quiet too.
        os.dup2(os.open(os.devnull, os.O_WRONLY), sys.stdout.fileno())
        return OK
    except OSError as error:
        print(f"Error: cannot read '{args.map_file}': {error}",
              file=sys.stderr)
    except KeyboardInterrupt:
        print("\nInterrupted by the user.", file=sys.stderr)
        return INTERRUPTED
    except Exception as error:                     # last-resort net
        # Nothing should reach this, but a stray exception from an
        # optional third-party package (pygame, SDL) must still exit
        # with a message rather than an unhandled traceback.
        print(f"Unexpected error: {type(error).__name__}: {error}",
              file=sys.stderr)
    return ERROR


def _simulate(args: argparse.Namespace) -> int:
    """Parse the map, run the simulation and display the result.

    Raises:
        ConfigurationError: If an option holds an unusable value.
        FlyInError: On any parsing or simulation failure.
        OSError: If the map file cannot be read.
    """
    if args.max_paths < 1:
        raise ConfigurationError(
            f"--max-paths must be a positive integer, got {args.max_paths}"
        )
    if args.speed <= 0:
        raise ConfigurationError(
            f"--speed must be a positive number of milliseconds, "
            f"got {args.speed:g}"
        )

    network, nb_drones = MapParser().parse(args.map_file)
    simulator = Simulator(network, nb_drones, max_routes=args.max_paths)
    renderer = TerminalRenderer(network, use_color=not args.no_color)
    animator = _build_animator(simulator, args.animate)

    if not args.quiet:
        renderer.header(nb_drones)

    def on_turn(turn: int, moves: List[str], running: Simulator) -> None:
        """Feed each played turn to the active visualizations."""
        if not args.quiet:
            renderer.turn(turn, moves, running.zone_load)
        if animator is not None:
            animator.record(turn, moves, running)

    quiet_run = args.quiet and animator is None
    result = simulator.run(on_turn=None if quiet_run else on_turn)

    if args.quiet:
        print("\n".join(result.lines))
    else:
        renderer.summary(result)

    if animator is not None:
        animator.play(args.speed, os.path.basename(args.map_file))
    return OK


def _build_animator(simulator: Simulator, animate: bool) -> Optional[Any]:
    """Create the animator, only when ``--animate`` was used.

    pygame is looked up here rather than imported at module level, and
    *before* the simulation runs, so a missing package is reported at
    once instead of after a long run has already been played out.

    Raises:
        ConfigurationError: If pygame is not installed.
    """
    if not animate:
        return None
    if importlib.util.find_spec("pygame") is None:
        raise ConfigurationError(
            "--animate needs the 'pygame' package, which is not "
            "installed. Install it with 'make install', "
            "'uv sync --extra viz', or 'pip install pygame'. "
            "The colored terminal output works without it."
        )
    from src.visualization.pygame_animator import DroneAnimator

    return DroneAnimator(simulator.network, simulator.drones)
