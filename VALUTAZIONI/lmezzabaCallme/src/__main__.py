# ABOUTME: CLI entry point. Wires together argument parsing, input loading,
# ABOUTME: model loading, constrained generation and output writing.

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

from llm_sdk import Small_LLM_Model

from .engine import load_vocab, run_all
from .functions_parser import load_function_definitions
from .prompts_parser import load_test_prompts

DEFAULT_FUNCTIONS = "data/input/functions_definition.json"
DEFAULT_INPUT = "data/input/function_calling_tests.json"
DEFAULT_OUTPUT = "data/output/function_calling_results.json"


def parse_args(argv: list[str] | None = None) -> argparse.Namespace:
    """Parse command-line arguments, applying the documented defaults."""
    parser = argparse.ArgumentParser(
        prog="function-calling-tool",
        description=(
            "Translate promts in function calls."
        ),
    )

    parser.add_argument(
        "--functions_definition",
        default=DEFAULT_FUNCTIONS,
        help="Path to the functions_definition.json file.",
    )

    parser.add_argument(
        "--input",
        default=DEFAULT_INPUT,
        help="Path to the file containing the natural-language test prompts.",
    )

    parser.add_argument(
        "--output",
        default=DEFAULT_OUTPUT,
        help="Path where the resulting JSON file will be written.",
    )

    return parser.parse_args(argv)


def write_output(
    output_path: Path,
    results: list[dict[str, object]],
) -> None:
    """Write results to output_path as pretty-printed JSON."""
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(
        json.dumps(results, indent=2),
        encoding="utf-8",
    )


def main(argv: list[str] | None = None) -> int:
    """Run the full pipeline and return the process exit code."""
    args = parse_args(argv)
    output_path = Path(args.output)

    try:
        function_defs = load_function_definitions(
            args.functions_definition
        )
        test_prompts = load_test_prompts(args.input)
    except (FileNotFoundError, ValueError) as exc:
        print(f"Error: {exc}", file=sys.stderr)
        return 1

    if not test_prompts:
        print("No prompts to process; writing an empty result file.")
        try:
            write_output(output_path, [])
        except OSError as exc:
            print(
                f"Error: could not write output file: {exc}",
                file=sys.stderr,
            )
            return 1
        return 0

    try:
        llm = Small_LLM_Model()
    except Exception as exc:
        print(
            f"Error: could not load the language model: {exc}",
            file=sys.stderr,
        )
        return 1

    try:
        vocab = load_vocab(llm)
    except Exception as exc:  # poi tutto qui
        print(
            f"Error: could not load the tokenizer vocabulary: {exc}",
            file=sys.stderr,
        )
        return 1

    results = run_all(
        vocab,
        llm,
        function_defs,
        test_prompts,
    )

    try:
        write_output(output_path, results)
    except OSError as exc:
        print(
            f"Error: could not write output file: {exc}",
            file=sys.stderr,
        )
        return 1

    print(
        f"Processed {len(results)}/{len(test_prompts)} prompt(s). "
        f"Output written to {output_path}"
    )

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
