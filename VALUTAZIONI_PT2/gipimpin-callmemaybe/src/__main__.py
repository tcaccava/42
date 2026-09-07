"""Main entry point for function-calling with constrained decoding."""

from typing import Any
import sys
import json
import os
import numpy as np
from .custom_tokenizer import CustomTokenizer
from .utils import (
    build_prompt, get_argv, get_json_from_file, get_line_from_file
)
from .schema_aware_validator import SchemaAwareValidator

try:
    from llm_sdk.llm_sdk import Small_LLM_Model
except ModuleNotFoundError as e:
    print(
        "Unable to import llm_sdk. Make sure you have copied the "
        "'llm_sdk' folder into the same directory containing 'src'. "
        f"Details: {e}"
    )
    sys.exit(1)


def _predict_function_name(
    input_ids: list[int],
    model: Any,
    tokenizer: CustomTokenizer,
    schema: SchemaAwareValidator,
    max_steps: int = 1,
) -> str:
    """Predict a function name using constrained greedy decoding.

    Constrains the model logits at each decoding step to only allow
    characters that form valid function names in the schema trie.

    Args:
        input_ids: List of prompt token IDs to be extended.
        model: Language model instance providing next-token logits.
        tokenizer: Custom tokenizer used for encoding and decoding.
        schema: Schema validator containing the function name trie.
        max_steps: Maximum number of generation steps.

    Returns:
        The predicted function name string without surrounding quotes.
    """
    current_name = ""

    for _ in range(max_steps):
        if schema.is_complete(current_name):
            break

        forced = schema.get_unique_completion_for_name(current_name)
        if forced:
            current_name += forced
            input_ids.extend(tokenizer.encode(forced))
            break

        allowed_chars = schema.next_char_for_name(current_name)

        logits = model.get_logits_from_input_ids(input_ids)

        best_id = None
        best_score = float("-inf")

        base_node = schema.name_trie
        for c in current_name:
            base_node = base_node[c]

        for char in allowed_chars:
            for tid in tokenizer.get_tokens_starting_with(char):
                # NOTE: use cache instead of re-decoding
                tok_str = tokenizer.get_decoded_token(tid)

                node = base_node
                valid_token = True
                for c in tok_str:
                    if c not in node:
                        valid_token = False
                        break
                    node = node[c]

                if valid_token and logits[tid] > best_score:
                    best_score = logits[tid]
                    best_id = tid

        if best_id is None:
            break

        chosen_str = tokenizer.get_decoded_token(best_id)
        current_name += chosen_str
        input_ids.append(best_id)

    return current_name.strip('"')


def _extract_parameter_value(
    input_ids: list[int],
    param_type: str,
    model: Any,
    tokenizer: CustomTokenizer,
    max_steps: int = 60,
) -> Any:
    """Extract a single parameter value via greedy decoding.

    Uses precomputed token masks to constrain decoding based on whether
    the parameter is a number or a string.

    Args:
        input_ids: List of prompt token IDs extended in place.
        param_type: Expected schema parameter type ("number" or "string").
        model: Language model instance providing next-token logits.
        tokenizer: Custom tokenizer used for encoding and decoding.
        max_steps: Maximum number of decoding steps.

    Returns:
        The extracted parameter value (float or int for numbers, str for
        strings).
    """
    value_str = ""

    if param_type == "number":
        digit_ids = tokenizer.digit_token_ids  # precomputed once
        delim_ids = tokenizer.delimiter_token_ids  # precomputed once

        for _ in range(max_steps):
            logits = np.asarray(model.get_logits_from_input_ids(input_ids))

            best_id = None
            best_score = float("-inf")

            if len(value_str) > 0 and delim_ids.size:
                idx = delim_ids[np.argmax(logits[delim_ids])]
                score = logits[idx]
                if score > best_score:
                    best_score, best_id = score, int(idx)

            if digit_ids.size:
                idx = digit_ids[np.argmax(logits[digit_ids])]
                score = logits[idx]
                if score > best_score:
                    best_score, best_id = score, int(idx)

            if best_id is None:
                break

            chosen = tokenizer.get_decoded_token(best_id)
            if chosen in (",", "}", " ", "\n"):
                break
            value_str += chosen
            input_ids.append(best_id)

        try:
            return float(value_str) if "." in value_str else int(value_str)
        except ValueError:
            return 0.0

    elif param_type == "string":
        quote_ids = tokenizer.quote_token_ids  # precomputed once
        non_quote_ids = tokenizer.non_quote_token_ids  # precomputed once

        for _ in range(max_steps):
            logits = np.asarray(model.get_logits_from_input_ids(input_ids))
            best_quote_id = (
                int(quote_ids[np.argmax(logits[quote_ids])])
                if quote_ids.size
                else None
            )
            best_quote_score = (
                logits[best_quote_id]
                if best_quote_id is not None
                else float("-inf")
            )

            best_id = (
                int(non_quote_ids[np.argmax(logits[non_quote_ids])])
                if non_quote_ids.size
                else None
            )
            best_score = (
                logits[best_id]
                if best_id is not None
                else float("-inf")
            )

            if best_id is None and best_quote_id is None:
                break

            if best_quote_id is not None and best_quote_score > best_score:
                chosen = tokenizer.get_decoded_token(best_quote_id)
                value_str += chosen.split('"')[0]
                break

            if best_id is None:
                break

            chosen = tokenizer.get_decoded_token(best_id)
            value_str += chosen
            input_ids.append(best_id)

        return value_str.strip()

    return ""


def generate_function_call(
    prompt: str,
    functions_txt: str,
    model: Any,
    tokenizer: CustomTokenizer,
    schema: SchemaAwareValidator,
) -> dict[str, Any]:
    """Generate a complete function-call JSON object.

    Orchestrates the full constrained-decoding pipeline:

    1. Build the chat-ML prompt and tokenise it.
    2. Predict the function name via :func:`_predict_function_name`.
    3. For each required parameter, extract its value via
       :func:`_extract_parameter_value`.

    Args:
        prompt: The user's natural-language query.
        functions_txt: A JSON-formatted string listing the available
            functions.
        model: Language model exposing a
            ``get_logits_from_input_ids`` method.
        tokenizer: The BPE tokenizer instance.
        schema: The schema validator providing function metadata.

    Returns:
        A dictionary with keys ``"name"`` (str) and
        ``"parameters"`` (dict) representing the generated function
        call.
    """
    prompt_txt = build_prompt(prompt, functions_txt)
    input_ids = tokenizer.encode(prompt_txt)

    start_prefix = '{\n  "name": "'
    input_ids.extend(tokenizer.encode(start_prefix))

    func_name = _predict_function_name(input_ids, model, tokenizer, schema)

    params_prefix = '",\n  "parameters": {'
    input_ids.extend(tokenizer.encode(params_prefix))

    required_params = schema.get_required_params(func_name)
    extracted_params: dict[str, Any] = {}

    for i, p_name in enumerate(required_params):
        p_type = schema.get_param_type(func_name, p_name)

        p_prefix = f'\n    "{p_name}": ' if i == 0 else f',\n    "{p_name}": '
        if p_type == "string":
            p_prefix += '"'
        input_ids.extend(tokenizer.encode(p_prefix))

        val = _extract_parameter_value(input_ids, p_type, model, tokenizer)
        extracted_params[p_name] = val

        if p_type == "string":
            input_ids.extend(tokenizer.encode('"'))

    input_ids.extend(tokenizer.encode("\n  }\n}"))

    return {"name": func_name, "parameters": extracted_params}


def main() -> None:
    """Run the function-calling pipeline end-to-end.

    Loads model resources, reads function definitions and input prompts,
    executes constrained decoding for each prompt, and writes the
    resulting JSON output.

    Returns:
        None.
    """
    argv = get_argv()
    model = Small_LLM_Model()
    try:
        merge_path = model.get_path_to_merges_file()
        tokenizer_path = model.get_path_to_tokenizer_file()
        vocab_path = model.get_path_to_vocab_file()
    except Exception as e:
        print(f"Error in method of Small_LLM_Model: {e}")
        sys.exit(1)
    try:
        dict_merge = get_line_from_file(merge_path)
        json_tokenizer = get_json_from_file(tokenizer_path)
        json_vocab = get_json_from_file(vocab_path)
        schema = SchemaAwareValidator(
            json_functions=get_json_from_file(argv.functions_definition)
        )
        json_prompts = get_json_from_file(argv.input)
    except Exception as e:
        print(f"Error: {e}")
        sys.exit(1)
    tokenizer = CustomTokenizer(
        token_id=json_vocab,
        dict_merge=dict_merge,
        special_token_id=json_tokenizer["added_tokens"],
    )
    print(vocab_path)
    function_txt = "\n".join(
        f'{f["name"]}: {f["description"]}' for f in schema.json_functions
    )
    results = []
    for p in json_prompts:
        prompt_text = p.get("prompt", "") if isinstance(p, dict) else p
        call_result = generate_function_call(
            prompt_text, function_txt, model, tokenizer, schema
        )
        final_obj = {
            "prompt": prompt_text,
            "name": call_result.get("name", "error_missing_name"),
            "parameters": call_result.get("parameters", {}),
        }

        if "error" in call_result:
            final_obj["error"] = call_result["error"]
            final_obj["raw_text"] = call_result.get("raw_text", "")

        results.append(final_obj)

    out_path = argv.output
    if os.path.isdir(out_path) or not out_path.endswith(".json"):
        os.makedirs(out_path, exist_ok=True)
        out_path = os.path.join(out_path, "function_calling_results.json")
    else:
        os.makedirs(os.path.dirname(out_path) or ".", exist_ok=True)

    with open(out_path, "w", encoding="utf-8") as f:
        json.dump(results, f, indent=2)


if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"{e}")
