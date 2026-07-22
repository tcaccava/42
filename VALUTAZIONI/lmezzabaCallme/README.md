*This project has been created as part of the 42 curriculum by lmezzaba.*

# call me maybe

## Description

`call me maybe` is a function-calling tool: it translates natural-language
prompts (e.g. *"What is the sum of 40 and 2?"*) into structured, schema-valid
function calls (e.g. `{"name": "fn_add_numbers", "parameters": {"a": 40.0, "b": 2.0}}`),
using a small local language model (**Qwen/Qwen3-0.6B**, 0.6B parameters).

Small models are unreliable at spontaneously producing valid JSON. Instead of
prompting and hoping, this project uses **constrained decoding**: at every
generation step, only tokens that keep the output both syntactically valid
JSON *and* compliant with the declared function schema are allowed. This
guarantees 100% parseable, schema-correct output regardless of the model's
raw reliability.

## Instructions

### Requirements
- Python 3.10+
- [uv](https://docs.astral.sh/uv/) for dependency management

### Installation
```bash
make install      # equivalent to: uv sync
```
The `llm_sdk` package is installed as a local path dependency (see
`pyproject.toml`); no separate installation step is needed for it.

### Running
```bash
make run
# equivalent to:
uv run python -m src [--functions_definition <file>] [--input <file>] [--output <file>]
```
By default the program reads `data/input/functions_definition.json` and
`data/input/function_calling_tests.json`, and writes
`data/output/function_calling_results.json`.

Example with custom paths:
```bash
uv run python -m src \
  --functions_definition data/input/functions_definition.json \
  --input data/input/function_calling_tests.json \
  --output data/output/function_calling_results.json
```

### Other Makefile targets
- `make debug` — runs the program under `pdb`.
- `make lint` — runs `flake8` and `mypy` with the mandatory flags
  (`--warn-return-any --warn-unused-ignores --ignore-missing-imports
  --disallow-untyped-defs --check-untyped-defs`).
- `make lint-strict` — runs `flake8` and `mypy --strict`.
- `make clean` — removes caches and build artifacts (`__pycache__`,
  `.mypy_cache`, `.pytest_cache`, `.ruff_cache`, `*.egg-info`, ...).

## Resources

- [Constrained decoding / structured generation — general overview](https://en.wikipedia.org/wiki/Large_language_model)
- [PEP 257 — Docstring Conventions](https://peps.python.org/pep-0257/)
- [uv documentation](https://docs.astral.sh/uv/)

**AI usage**: An AI assistant (Claude) was used to draft this README, write
the docstrings across the source files, and explain key concepts and reformatting
the code to match flake8 and mypy requirements.

## Algorithm Explanation

For each prompt, the program builds a textual prompt listing the available
functions (name, description, parameter types) and the user's request. It
then generates the answer **token by token** using only the SDK's public
methods (`encode`, `get_logits_from_input_ids`, `get_path_to_vocab_file`):

1. **Function name**: at each step, only tokens that keep the generated text
   a valid *prefix* of one of the declared function names are allowed among
   all vocabulary tokens; the highest-logit valid token is picked. Generation
   stops once the text exactly matches a function name.
2. **Parameters**: the JSON skeleton (`"param_name": `) is injected directly
   as fixed tokens (no need to generate it), then the value is generated
   under a type-specific validator:
   - **number**: only digits, `.` and a leading `-` are allowed; generation
     stops at `,` or `}` (the JSON separator itself is not part of the value).
   - **string**: any character except `"`, `\` and newline is allowed;
     generation stops at the closing `"`.
   - **boolean**: same prefix-matching mechanism as the function name, with
     `"true"`/`"false"` as the only two choices.

At every step, invalid tokens are filtered out from the vocabulary **before**
picking the arg-max of the logits — this is the "modify the logits before
selection" idea from constrained decoding, implemented here as a hard token
filter rather than a `-inf` logit mask, which is equivalent but avoids
scanning the entire logits vector twice.

## Design Decisions

- **Generic engine, not hardcoded per type**: `generate_constrained` is a
  single reusable loop; each parameter/field type only needs to provide two
  small functions (`is_token_allowed`, `is_complete`). Adding a new type
  later would require no changes to the core loop.
- **pydantic for all input models**: `FunctionDefinition`, `ParameterDefinition`
  and `TestPrompt` validate the input JSON files structurally, so malformed
  files fail fast with a clear error instead of raising confusing
  `KeyError`/`TypeError` deep inside the generation logic.
- **No private SDK access**: only `encode`, `get_logits_from_input_ids`, and
  `get_path_to_vocab_file` are used, all public methods of `Small_LLM_Model`.
- **Fail-soft per prompt**: if a single prompt fails (unexpected model
  behaviour, encoding issue, etc.), it is logged and skipped so the rest of
  the batch still completes — this satisfies the "never crash" requirement
  without silently producing wrong output for the other prompts.

## Performance Analysis

- **Accuracy**: Function selection and argument extraction are always
  schema-correct by construction (the decoder can never emit an invalid
  function name, wrong type, or malformed JSON) — the model only has to
  pick *which* valid option is right, which Qwen3-0.6B does reliably given
  the small, explicit list of candidate functions/values in the prompt.
- **Validity**: 100% of outputs are valid, parseable JSON, since every
  character in the structural skeleton (`{`, `"name":`, `"parameters":`, `,`,
  `}`) is injected directly and never left to free generation.
- **Speed**: generation is capped at 30 tokens per field
  (`max_steps` in `generate_constrained`), and each field only needs a few
  forward passes (function names and short values are usually 1-5 tokens),
  keeping a full batch of prompts well within a few minutes on CPU.

## Challenges Faced

- **Mapping raw logits to legal JSON tokens**: solved by inverting the
  tokenizer's `vocab.json` once per run and re-decoding BPE space/newline
  markers (`Ġ`, `Ċ`) into real characters before matching them against the
  validator functions.
- **Keeping the engine generic across types**: solved by extracting a common
  `(is_token_allowed, is_complete)` contract so number/string/boolean/function
  name all reuse the same `generate_constrained` loop instead of duplicated
  ad-hoc code.
- **Never crashing on bad input**: solved with `pydantic` validation for the
  input files (caught and reported clearly in `__main__.py`) and a per-prompt
  try/except in `run_all` so one bad prompt doesn't stop the batch.
- **Static analysis compliance**: numpy's newer type stubs use Python
  3.12-only syntax (PEP 695 `type` statements), which broke `mypy` under the
  project's required Python 3.10+ target; solved by pinning `numpy` to a
  version whose stubs remain compatible with Python 3.10.

## Testing Strategy

The implementation was validated by running it against the provided example
files (`data/input/functions_definition.json` and
`data/input/function_calling_tests.json`, covering `number`, `string` and
multi-parameter functions), and by manually checking edge cases:
- missing/invalid input files (wrong path, malformed JSON, empty array),
- prompts with no obviously matching function,
- numbers with decimals and multi-digit values,
- strings containing punctuation/quotes in the surrounding sentence.
In every case, the output file remained valid, schema-compliant JSON.

## Example Usage

```bash
make install
make run
cat data/output/function_calling_results.json
```

Example output for `"What is the sum of 2 and 3?"`:
```json
{
  "prompt": "What is the sum of 2 and 3?",
  "name": "fn_add_numbers",
  "parameters": { "a": 2.0, "b": 3.0 }
}
```