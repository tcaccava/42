# ABOUTME: Pydantic models and loader for the
# ABOUTME: functions_definition.json input file.
# ABOUTME: Validates function name, description, parameter types
# ABOUTME: and return type.

from __future__ import annotations

import json
from pathlib import Path

from pydantic import BaseModel, Field, ValidationError

ALLOWED_TYPES = {"number", "string", "boolean"}


class ParameterDefinition(BaseModel):
    """Declared type of a single function parameter.

    Attributes:
        type: The JSON schema type of the parameter. Must be one
            of the values in `ALLOWED_TYPES` ("number", "string",
            "boolean").
    """

    type: str


class ReturnDefinition(BaseModel):
    """Declared type of a function's return value.

    Attributes:
        type: The JSON schema type of the value the function
            returns.
    """

    type: str


class FunctionDefinition(BaseModel):
    """A single callable function, as described in
    functions_definition.json.

    Attributes:
        name: The unique identifier of the function
            (e.g. "fn_add_numbers").
        description: A natural-language explanation of what the
            function does, used to help the model choose the
            right function.
        parameters: Mapping from parameter name to its declared
            type definition. Defaults to an empty mapping if the
            function takes no arguments.
        returns: The declared type of the function's return
            value, or `None` if not specified.
    """

    name: str
    description: str
    parameters: dict[str, ParameterDefinition] = Field(
        default_factory=dict
    )
    returns: ReturnDefinition | None = None


def load_function_definitions(
    path: str | Path,
) -> list[FunctionDefinition]:
    """Read, parse and validate the functions_definition.json
    file.

    Args:
        path: Path to the JSON file describing the available
            functions.

    Returns:
        A list of validated FunctionDefinition objects.

    Raises:
        FileNotFoundError: If the file does not exist.
        ValueError: If the file is not valid JSON or does not
            match the expected schema.
    """
    file_path = Path(path)
    if not file_path.is_file():
        raise FileNotFoundError(
            f"functions definition file not found: {file_path}"
        )

    try:
        raw = json.loads(file_path.read_text(encoding="utf-8"))
    except json.JSONDecodeError as exc:
        raise ValueError(
            f"invalid JSON in {file_path}: {exc}"
        ) from exc

    if not isinstance(raw, list):
        raise ValueError(
            f"{file_path} must contain a JSON array of function "
            "definitions"
        )

    functions: list[FunctionDefinition] = []
    for idx, item in enumerate(raw):
        try:
            function_def = FunctionDefinition(**item)
        except (ValidationError, TypeError) as exc:
            raise ValueError(
                f"invalid function definition at index {idx}: "
                f"{exc}"
            ) from exc

        for param_name, param_def in function_def.parameters.items():
            if param_def.type not in ALLOWED_TYPES:
                raise ValueError(
                    f"function '{function_def.name}' parameter "
                    f"'{param_name}' "
                    f"has unsupported type '{param_def.type}'"
                )
        functions.append(function_def)

    if not functions:
        raise ValueError(f"{file_path} contains no function definitions")

    return functions
