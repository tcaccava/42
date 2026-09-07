"""Utility functions for CLI parsing, file loading, and prompt building."""

import argparse
import json
from typing import Any


def get_argv() -> argparse.Namespace:
    """Parse and return command-line arguments.

    Returns:
        argparse.Namespace: Parsed command-line arguments containing:
            functions_definition: Path to functions-definition JSON file.
            input: Path to the input prompts JSON file.
            output: Path (file or directory) for the results.
    """
    parser = argparse.ArgumentParser(description="Function calling in LLMs")
    parser.add_argument(
        "--functions_definition",
        default="data/input/functions_definition.json",
        help="Path to functions definition file",
    )
    parser.add_argument(
        "--input",
        default="data/input/function_calling_tests.json",
        help="Path to input file",
    )
    parser.add_argument(
        "--output", default="data/output",
        help="Path of the function to be return"
    )
    return parser.parse_args()


def get_json_from_file(name: str) -> Any:
    """Read and parse a JSON file.

    Args:
        name: Path to the JSON file to read.

    Returns:
        The deserialised JSON content (dict, list, etc.).
    """
    with open(name, "r", encoding="utf-8") as f:
        return json.load(f)


def get_line_from_file(name: str) -> dict[tuple[str, str], int]:
    """Read a BPE merges file and return the merge table.

    The first line of the file is treated as a header and skipped.
    Each subsequent line is expected to contain two space-separated
    token strings representing a merge pair.

    Args:
        name: Path to the merges file (e.g. ``merges.txt``).

    Returns:
        A dictionary mapping ``(token_a, token_b)`` pairs to their
        zero-based merge rank.
    """
    with open(name, "r", encoding="utf-8") as f:
        lines = f.readlines()
    res = {}
    for rank, line in enumerate(lines[1:]):
        part = line.split()
        res[(part[0], part[1])] = rank
    return res


def build_prompt(user_prompt: str, functions_text: str) -> str:
    """Build a chat-ML formatted prompt for function-call generation.

    Constructs a prompt string using the ``<|im_start|>`` /
    ``<|im_end|>`` chat-ML delimiters.  The *system* turn describes
    the assistant role and lists the available functions, while the
    *user* turn contains the actual user query.

    Args:
        user_prompt: The natural-language request from the user.
        functions_text: A JSON-formatted string describing the
            available functions and their parameters.

    Returns:
        The fully assembled prompt string ready to be tokenised and
        fed to the language model.
    """
    return (
        "<|im_start|>system\n"
        "You are a function calling assistant.\n"
        f"Available functions:\n{functions_text}<|im_end|>\n"
        f"<|im_start|>user\n{user_prompt}\n"
        "Respond only with a JSON object with keys 'name' "
        "and 'parameters'.<|im_end|>\n"
        "<|im_start|>assistant\n"
    )
