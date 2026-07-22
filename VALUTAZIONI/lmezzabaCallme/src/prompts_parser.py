# ABOUTME: Pydantic model and loader for the input file of
# ABOUTME: natural-language test prompts.
# ABOUTME: Each entry is a single {"prompt": "..."} object.

from __future__ import annotations

import json
from pathlib import Path

from pydantic import BaseModel, ValidationError


class TestPrompt(BaseModel):
    """A single natural-language request to translate into a
    function call.

    Attributes:
        prompt: The raw natural-language text typed by the user
            (e.g. "What is the sum of 2 and 3?").
    """

    prompt: str


def load_test_prompts(path: str | Path) -> list[TestPrompt]:
    """Read, parse and validate the input file containing test
    prompts.

    Args:
        path: Path to the JSON file containing an array of
            prompts.

    Returns:
        A list of validated TestPrompt objects.

    Raises:
        FileNotFoundError: If the file does not exist.
        ValueError: If the file is not valid JSON or does not
            match the expected schema.
    """
    file_path = Path(path)
    if not file_path.is_file():
        raise FileNotFoundError(
            f"input prompts file not found: {file_path}"
        )

    try:
        raw = json.loads(file_path.read_text(encoding="utf-8"))
    except json.JSONDecodeError as exc:
        raise ValueError(
            f"invalid JSON in {file_path}: {exc}"
        ) from exc

    if not isinstance(raw, list):
        raise ValueError(
            f"{file_path} must contain a JSON array of prompts"
        )

    prompts: list[TestPrompt] = []
    for idx, item in enumerate(raw):
        try:
            prompts.append(TestPrompt(**item))
        except (ValidationError, TypeError) as exc:
            raise ValueError(
                f"invalid prompt entry at index {idx}: {exc}"
            ) from exc

    return prompts
