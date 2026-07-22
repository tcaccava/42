# ABOUTME: Constrained-decoding engine: forces the LLM to produce
# ABOUTME: schema-valid JSON by restricting, token by token, which
# ABOUTME: tokens are legal to generate.

from __future__ import annotations

import json
from pathlib import Path
from typing import Callable

from llm_sdk import Small_LLM_Model

from .functions_parser import FunctionDefinition
from .prompts_parser import TestPrompt

TokenAllowed = Callable[[str, str], bool]
IsComplete = Callable[[str], bool]


# --------------------------------------------------------------------
# Vocabulary
# --------------------------------------------------------------------

def load_vocab(llm: Small_LLM_Model) -> dict[str, int]:
    """Load the tokenizer's vocabulary file via the SDK.

    Args:
        llm: The model wrapper providing access to the vocabulary
            file path.

    Returns:
        A mapping from token string (as stored in vocab.json,
        including any BPE markers such as 'Ġ') to its integer
        token id.
    """
    vocab_path = Path(llm.get_path_to_vocab_file())
    with vocab_path.open(encoding="utf-8") as fh:
        vocab: dict[str, int] = json.load(fh)
    return vocab


def invert_vocab(vocab: dict[str, int]) -> dict[int, str]:
    """Invert the vocabulary mapping and decode BPE markers into
    real characters.

    Args:
        vocab: Mapping from token string to token id, as returned
            by :func:`load_vocab`.

    Returns:
        A mapping from token id to its human-readable string
        representation, with the BPE markers 'Ġ' and 'Ċ' replaced
        by an actual space and newline respectively.
    """
    return {
        token_id: token_str.replace("\u0120", " ")
        .replace("\u010a", "\n")
        for token_str, token_id in vocab.items()
    }


# --------------------------------------------------------------------
# Prompt construction
# --------------------------------------------------------------------

def build_prompt(
    user_prompt: str, function_defs: list[FunctionDefinition]
) -> str:
    """Build the natural-language prompt given to the model.

    The prompt lists the available functions (name, description
    and parameter types) as JSON, so the model can pick the
    correct function and extract its arguments from the user's
    request.

    Args:
        user_prompt: The natural-language request typed by the
            user.
        function_defs: The list of functions the model is allowed
            to call.

    Returns:
        The full prompt string to feed to the model.
    """
    functions_json = json.dumps(
        [
            {
                "name": f.name,
                "description": f.description,
                "parameters": {
                    name: p.type for name, p in f.parameters.items()
                },
            }
            for f in function_defs
        ],
        indent=2,
    )
    return (
        "You are a function-calling assistant. "
        "Given the user request and the available functions "
        "below, "
        "choose the correct function and extract the arguments. "
        "Respond only with a single valid JSON object containing "
        'the keys "name" and "parameters", with no additional '
        "text.\n\n"
        f"Available functions:\n{functions_json}\n\n"
        f"User request: {user_prompt}\n"
    )


# --------------------------------------------------------------------
#  Generic constrained-generation loop
# --------------------------------------------------------------------

def generate_constrained(
    llm: Small_LLM_Model,
    input_ids: list[int],
    id_to_token: dict[int, str],
    is_token_allowed: TokenAllowed,
    is_complete: IsComplete,
    max_steps: int = 30,
) -> str:
    """Generate text token-by-token under a schema constraint.

    At each step, every token in the vocabulary is checked against
    `is_token_allowed`; only tokens that keep the output valid are
    considered, and the one with the highest logit among them is
    chosen (constrained greedy decoding).

    Args:
        llm: The model wrapper used to obtain next-token logits.
        input_ids: The token ids of the prompt generated so far
            (context).
        id_to_token: Mapping from token id to its string
            representation.
        is_token_allowed: Predicate ``(generated_so_far,
            candidate_token) -> bool`` that decides whether a
            candidate token may be appended.
        is_complete: Predicate ``(generated_so_far) -> bool`` that
            decides whether generation should stop.
        max_steps: Safety limit on the number of tokens generated,
            to avoid infinite loops if `is_complete` never
            triggers.

    Returns:
        The generated text (not including the initial prompt).
    """
    generated = ""
    working_ids = list(input_ids)
    for _ in range(max_steps):
        if is_complete(generated):
            break

        logits = llm.get_logits_from_input_ids(working_ids)

        valid_ids = []

        for tid, tok_str in id_to_token.items():
            if is_token_allowed(generated, tok_str):
                valid_ids.append(tid)

        if not valid_ids:
            break  # se non e valido stop qui e vaff

        best_id = max(valid_ids, key=lambda tid: logits[tid])
        best_tok = id_to_token[best_id]

        generated += best_tok
        working_ids.append(best_id)

    return generated


# --------------------------------------------------------------------
#  Validity rules for each kind of "slot" we need to fill
# --------------------------------------------------------------------

def make_choice_validator(
    choices: list[str],
) -> tuple[TokenAllowed, IsComplete]:
    """Build validator functions for a fixed multiple-choice field.

    Used for the function name and for boolean values (choice
    between ``"true"`` and ``"false"``).

    Args:
        choices: The exhaustive list of valid final strings.

    Returns:
        A tuple ``(is_allowed, is_complete)`` where:

        * ``is_allowed(generated, token_str)`` returns ``True`` if
          ``generated + token_str`` is still a prefix of at least
          one of `choices`.
        * ``is_complete(generated)`` returns ``True`` once
          `generated` exactly equals one of `choices`.
    """

    def is_allowed(generated: str, token_str: str) -> bool:
        """Check whether appending `token_str` keeps `generated`
        a valid prefix.

        Args:
            generated: The text produced so far for this field.
            token_str: The candidate token's string
                representation.

        Returns:
            ``True`` if ``generated + token_str`` is a prefix of
            at least one of the allowed `choices`, ``False``
            otherwise.
        """
        candidate = generated + token_str
        return any(choice.startswith(candidate) for choice in choices)

    def is_complete(generated: str) -> bool:
        """Check whether `generated` exactly matches one of the
        choices.

        Args:
            generated: The text produced so far for this field.

        Returns:
            ``True`` if `generated` is one of the allowed
            `choices`.
        """
        return generated in choices

    return is_allowed, is_complete


def make_number_validator() -> tuple[TokenAllowed, IsComplete]:
    """Build validator functions for a JSON ``number`` field.

    Accepts digits, a single decimal point, and an optional
    leading minus sign. A single leading space is tolerated on the
    very first token, since real BPE tokenizers commonly fuse the
    space that follows ``": "`` into the same token as the value
    itself (e.g. the token ``" -2"``). Generation stops as soon as
    a JSON terminator (``,`` or ``}``) is produced.

    Returns:
        A tuple ``(is_allowed, is_complete)`` implementing the
        rules above.
    """
    allowed_chars = set("0123456789.")
    terminators = {",", "}"}

    def is_allowed(generated: str, token_str: str) -> bool:
        """Check whether `token_str` may extend the number being
        generated.

        Args:
            generated: The text produced so far for this field.
            token_str: The candidate token's string
                representation.

        Returns:
            ``True`` if every character of `token_str` keeps the
            number syntactically valid (digits, at most one
            leading '-', a terminator only as the very last
            character), ``False`` otherwise.
        """
        if token_str == "":
            return False

        core = token_str
        if generated == "" and core.startswith(" "):
            # Tolerate the leading space that a real tokenizer
            # typically fuses into the value's first token
            # (e.g. " -2", " 42").
            core = core[1:]
            if core == "":
                return True

        for i, ch in enumerate(core):
            if ch in terminators:
                return i == len(core) - 1  # terminatore quim
            if ch == "-" and i != 0:
                return False  # '-' lo ho permesso soolo all inzizo
            if ch not in allowed_chars and ch != "-":
                return False
        return True

    def is_complete(generated: str) -> bool:
        """Check whether the generated number has reached its
        terminator.

        Args:
            generated: The text produced so far for this field.

        Returns:
            ``True`` if `generated` is non-empty and ends with a
            terminator character (``,`` or ``}``).
        """
        return len(generated) > 0 and generated[-1] in terminators

    return is_allowed, is_complete


def make_string_validator() -> tuple[TokenAllowed, IsComplete]:
    """Build validator functions for a JSON ``string`` field.

    Allows any character except a closing double-quote (which
    terminates the string) and characters that would break JSON
    validity if left unescaped (newline, backslash).

    Returns:
        A tuple ``(is_allowed, is_complete)`` implementing the
        rules above.
    """
    terminators = {'"'}

    def is_allowed(generated: str, token_str: str) -> bool:
        """Check whether `token_str` may extend the string being
        generated.

        Args:
            generated: The text produced so far for this field.
            token_str: The candidate token's string
                representation.

        Returns:
            ``True`` if `token_str` contains no unescaped
            characters that would break JSON validity, and the
            closing quote (if present) only appears as the last
            character. ``False`` otherwise.
        """
        if token_str == "":
            return False
        for i, ch in enumerate(token_str):
            if ch in terminators:
                return i == len(token_str) - 1
            if ch in ("\n", "\\"):
                return False  # cosi non si rompe  rutto 
        return True

    def is_complete(generated: str) -> bool:
        """Check whether the generated string has reached its
        closing quote.

        Args:
            generated: The text produced so far for this field.

        Returns:
            ``True`` if `generated` ends with a double-quote
            character.
        """
        return generated.endswith('"')

    return is_allowed, is_complete


# --------------------------------------------------------------------
# Generate a single parameter value, given its declared type
# --------------------------------------------------------------------

def generate_value_for_type(
    llm: Small_LLM_Model,
    input_ids: list[int],
    id_to_token: dict[int, str],
    param_type: str,
) -> tuple[str, object]:
    """Generate a single parameter's value under constrained
    decoding.

    Args:
        llm: The model wrapper used to obtain next-token logits.
        input_ids: The token ids of the prompt generated so far
            (context), up to and including the opening of this
            parameter's value.
        id_to_token: Mapping from token id to its string
            representation.
        param_type: The declared schema type of the parameter
            (``"number"``, ``"boolean"``, or ``"string"``).

    Returns:
        A tuple ``(raw_text, python_value)`` where `raw_text` is
        the exact generated text (including its terminator) and
        `python_value` is the corresponding native Python value
        (``float``, ``bool``, or ``str``).
    """
    if param_type == "number":
        is_allowed, is_complete = make_number_validator()
        raw = generate_constrained(
            llm, input_ids, id_to_token, is_allowed, is_complete
        )
        number_part = (
            raw[:-1] if raw and raw[-1] in {",", "}"} else raw
        )
        try:
            return raw, float(number_part)
        except ValueError:
            return raw, 0.0

    if param_type == "boolean":
        is_allowed, is_complete = make_choice_validator(
            ["true", "false"]
        )
        raw = generate_constrained(
            llm, input_ids, id_to_token, is_allowed, is_complete
        )
        return raw, raw == "true"

    # default: string
    is_allowed, is_complete = make_string_validator()
    raw = generate_constrained(
        llm, input_ids, id_to_token, is_allowed, is_complete
    )
    string_part = raw[:-1] if raw.endswith('"') else raw
    return raw, string_part


# --------------------------------------------------------------------
# Full pipeline for a single prompt
# --------------------------------------------------------------------

def process_single_prompt(
    llm: Small_LLM_Model,
    id_to_token: dict[int, str],
    function_defs: list[FunctionDefinition],
    user_prompt: str,
) -> dict[str, object]:
    """Run the full constrained generation pipeline for one user
    prompt.

    Builds the model prompt, lets the model choose a function name
    under a multiple-choice constraint, then generates each of
    that function's parameters under a type-specific constraint.

    Args:
        llm: The model wrapper used to obtain next-token logits
            and encode text into token ids.
        id_to_token: Mapping from token id to its string
            representation.
        function_defs: The list of functions the model is allowed
            to call.
        user_prompt: The natural-language request to translate
            into a function call.

    Returns:
        A dictionary with the keys ``"prompt"``, ``"name"`` and
        ``"parameters"``, ready to be serialized as part of the
        output JSON file.
    """
    full_prompt = build_prompt(user_prompt, function_defs)
    input_ids = llm.encode(full_prompt).squeeze(0).tolist()

    function_names = [f.name for f in function_defs]

    # --- Step 1: choose the function name ---
    name_is_allowed, name_is_complete = make_choice_validator(
        function_names
    )
    chosen_name = generate_constrained(
        llm,
        input_ids,
        id_to_token,
        name_is_allowed,
        name_is_complete,
    )
    input_ids += llm.encode(chosen_name).squeeze(0).tolist()

    chosen_function = next(
        f for f in function_defs if f.name == chosen_name
    )

    # --- Step 2: generate each parameter, in schema order ---
    parameters: dict[str, object] = {}
    for param_name, param_def in chosen_function.parameters.items():
        literal = f'"{param_name}":'
        if param_def.type == "string":
            literal += '"'
        input_ids += llm.encode(literal).squeeze(0).tolist()

        raw, value = generate_value_for_type(
            llm, input_ids, id_to_token, param_def.type
        )
        input_ids += llm.encode(raw).squeeze(0).tolist()

        parameters[param_name] = value

    return {
        "prompt": user_prompt,
        "name": chosen_name,
        "parameters": parameters,
    }


def run_all(
    vocab: dict[str, int],
    llm: Small_LLM_Model,
    function_defs: list[FunctionDefinition],
    test_prompts: list[TestPrompt],
) -> list[dict[str, object]]:
    """Process every test prompt and collect the results.

    A single failing prompt is logged and skipped rather than
    crashing the whole run, so one bad input never prevents the
    rest from being processed.

    Args:
        vocab: The raw tokenizer vocabulary (token string -> id),
            as returned by :func:`load_vocab`.
        llm: The model wrapper used to obtain next-token logits
            and encode text into token ids.
        function_defs: The list of functions the model is allowed
            to call.
        test_prompts: The natural-language prompts to process.

    Returns:
        A list of result dictionaries (one per successfully
        processed prompt), ready to be serialized as the output
        JSON file.
    """
    id_to_token = invert_vocab(vocab)
    results: list[dict[str, object]] = []

    for test_prompt in test_prompts:
        try:
            result = process_single_prompt(
                llm, id_to_token, function_defs, test_prompt.prompt
            )
            results.append(result)
        except Exception as exc:  # pio tutto
            print(
                "Warning: failed to process prompt "
                f"'{test_prompt.prompt}': {exc}"
            )

    return results
