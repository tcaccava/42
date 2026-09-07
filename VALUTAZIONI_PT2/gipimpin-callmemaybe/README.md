*This project has been created as part of the 42 curriculum by gipimpin.*

# Call Me Maybe - Function Calling in LLMs

A robust, high-performance function calling system powered by **constrained decoding** for lightweight Large Language Models (specifically `Qwen/Qwen3-0.6B`). This project translates unstructured natural language queries into 100% syntactically valid and schema-compliant JSON function calls without relying on prompting heuristics or model luck.

---

## Contents

- [Description](#description)
- [Instructions](#instructions)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
  - [Execution](#execution)
  - [Code Quality and Linting](#code-quality-and-linting)
- [Algorithm Explanation](#algorithm-explanation)
  - [1. Chat-ML Prompt Conditioning](#1-chat-ml-prompt-conditioning)
  - [2. Structural Prefix Injection](#2-structural-prefix-injection)
  - [3. Trie-Guided Function Selection](#3-trie-guided-function-selection)
  - [4. Schema-Aware Parameter Extraction](#4-schema-aware-parameter-extraction)
- [Design Decisions](#design-decisions)
  - [Prefix Injection vs. Raw Grammar FSM](#prefix-injection-vs-raw-grammar-fsm)
  - [Character-Level Trie with Fast Subword Search](#character-level-trie-with-fast-subword-search)
  - [Custom BPE Tokenizer Architecture](#custom-bpe-tokenizer-architecture)
  - [Pre-Computed First-Character Indexing](#pre-computed-first-character-indexing)
  - [Pydantic v2 Schema Enforcement](#pydantic-v2-schema-enforcement)
- [Performance Analysis](#performance-analysis)
  - [Benchmark Results](#benchmark-results)
  - [Execution Metrics](#execution-metrics)
- [Challenges Faced](#challenges-faced)
- [Testing Strategy](#testing-strategy)
- [Example Usage](#example-usage)
- [Bonus Features](#bonus-features)
- [Resources](#resources)

---

## Description

Small language models (such as `Qwen/Qwen3-0.6B`, with ~596M parameters) struggle significantly to generate well-formed, structured outputs consistently. When asked to produce valid JSON adhering to rigid schemas, unconstrained models often hallucinate function names, output malformed syntax (e.g., missing quotes, unbalanced brackets), or introduce conversational prose.

**Call Me Maybe** solves this problem by applying **constrained decoding**:
- Rather than hoping the model produces valid JSON, our generation engine mathematically guarantees that every generated token conforms to the target syntax and function schema.
- At each step, invalid tokens are pruned from the candidate space, leaving the model free to direct its probability distribution solely across valid semantic continuations.
- In addition, this implementation features a **from-scratch BPE Tokenizer** (`CustomTokenizer`), fully bypassing the high-level `encode` and `decode` methods from the provided model wrapper.

---

## Instructions

### Prerequisites

- **Python**: `>= 3.10`
- **Package Manager**: [uv](https://github.com/astral-sh/uv)
- **Environment**: Linux or WSL2 (Ubuntu recommended)
- **Hugging Face Token** *(Optional)*: `HF_TOKEN` for faster and authenticated downloads from the Hugging Face Hub.

### Installation

Clone the repository and synchronize all dependencies using `uv`:

```bash
# Using Makefile
make install

# Or directly with uv
uv sync
```

### Execution

The project can be executed through the `Makefile` or directly with `uv run python -m src`:

```bash
# Standard execution (reads data/input/ and writes data/output/)
make run

# Execution with execution timing
make debug

# Custom CLI arguments
uv run python -m src \
  --functions_definition data/input/functions_definition.json \
  --input data/input/function_calling_tests.json \
  --output data/output/function_calling_results.json
```

#### Command-Line Arguments

| Argument | Default | Description |
| :--- | :--- | :--- |
| `--functions_definition` | `data/input/functions_definition.json` | Path to the available functions JSON specification |
| `--input` | `data/input/function_calling_tests.json` | Path to the input JSON file containing user prompts |
| `--output` | `data/output` | Output destination file or directory |

### Code Quality and Linting

The codebase strictly adheres to PEP 8, PEP 257 docstring standards, and comprehensive static type annotations checked with `mypy`:

```bash
# Run flake8 and mypy with strict flags
make lint

# Run strict verification
make lint-strict

# Clean caches and temporary files
make clean
```

---

## Algorithm Explanation

The constrained generation pipeline operates in four coordinated phases:

```
+-------------------------------------------------------------------------+
|                              User Prompt                                |
+-------------------------------------------------------------------------+
                                     |
                                     v
+-------------------------------------------------------------------------+
| 1. Chat-ML Prompt Formatting                                            |
|    <|im_start|>system\n...functions...<|im_end|>                        |
|    <|im_start|>user\n{prompt}<|im_end|>\n<|im_start|>assistant\n         |
+-------------------------------------------------------------------------+
                                     |
                                     v
+-------------------------------------------------------------------------+
| 2. Structural Prefix Injection                                          |
|    Inject: '{\n  "name": "'                                             |
+-------------------------------------------------------------------------+
                                     |
                                     v
+-------------------------------------------------------------------------+
| 3. Trie-Guided Function Name Selection                                  |
|    - Schema Trie filters candidate tokens                               |
|    - Instant auto-completion of unique suffix branches                  |
|    - Model selects optimal valid function                               |
+-------------------------------------------------------------------------+
                                     |
                                     v
+-------------------------------------------------------------------------+
| 4. Type-Constrained Parameter Extraction                                |
|    - Injects: '",\n  "parameters": {\n'                                 |
|    - For each schema parameter:                                         |
|        * Inject '"<param_name>": '                                      |
|        * Constrain tokens by type (numeric charset vs. string quotes)   |
|    - Close JSON: '\n  }\n}'                                             |
+-------------------------------------------------------------------------+
                                     |
                                     v
+-------------------------------------------------------------------------+
| 100% Valid, Schema-Compliant JSON Output                                |
+-------------------------------------------------------------------------+
```

### 1. Chat-ML Prompt Conditioning

The query is formatted into a Chat-ML conversation structure (`<|im_start|>system`, `<|im_start|>user`, `<|im_start|>assistant`) via `build_prompt`. The system turn provides the complete JSON schema of available functions and instructs the assistant to respond with a function call.

### 2. Structural Prefix Injection

Instead of forcing the LLM to learn and reproduce rigid JSON boilerplate (like opening braces, quotation marks, and key names) token by token, the engine injects invariant structural tokens directly into `input_ids`:
```python
start_prefix = '{\n  "name": "'
input_ids.extend(tokenizer.encode(start_prefix))
```
This guarantees flawless JSON formatting at zero computational cost and prevents syntax hallucinations.

### 3. Trie-Guided Function Selection

Available function names are parsed and compiled into a character-level **Prefix Tree (Trie)** via `SchemaAwareValidator`:
1. **Unambiguous Suffix Auto-Completion**: If the current prefix has reached a node in the Trie with only a single deterministic continuation, `get_unique_completion_for_name` completes the suffix immediately without a model forward pass.
2. **Logit Filtering**: When branching occurs, the validator determines the set of allowed next characters (`next_char_for_name`). Candidate tokens starting with these characters are evaluated against the Trie. Only tokens representing valid subpaths are scored; the token with the highest model logit is selected.

### 4. Schema-Aware Parameter Extraction

Once the function name is decided, the schema identifies all required parameters:
- The parameter key is injected into the context (`"param_name": `).
- **Numeric Parameters (`"number"`)**: Candidate tokens are restricted strictly to characters in `frozenset("0123456789.-")` and delimiters (`,`, `}`, ` `, `\n`). Decoding halts immediately upon encountering a delimiter, and the token sequence is parsed into `int` or `float`.
- **String Parameters (`"string"`)**: Greedy decoding tracks quote-bearing tokens versus non-quote text. When a quote token attains the highest logit, string generation terminates cleanly without leaking delimiters or trailing characters.

---

## Design Decisions

### Prefix Injection vs. Raw Grammar FSM

An earlier iteration of this project explored an exhaustive token-level JSON Finite State Machine (`json_validator.py`). While conceptually sound, running a dynamic character-level state machine across all ~151,000 vocabulary tokens at every single decoding step introduced substantial computational overhead and edge-case fragility.

**Decision**: We transitioned to **Structural Prefix Injection coupled with Trie/Type Constraints**.
- Structural scaffolding is static and 100% predictable.
- The model is queried *only* for semantic choices: which function to call, and what values to assign.
- Result: **Near-instant generation**, zero JSON syntax errors, and drastically cleaner code.

### Character-Level Trie with Fast Subword Search

Subword tokenizers (such as BPE) produce tokens of varying character lengths that may span across word boundaries.
- Storing function names in a Trie allows character-level validation while matching multi-character tokens in a single step.
- Auto-completion of deterministic prefixes avoids redundant forward passes for uniquely identifiable functions (e.g., once `fn_sub` is decoded, `stitute_string_with_regex` is completed deterministically).

### Custom BPE Tokenizer Architecture

To satisfy the bonus objective and achieve full architectural independence from the `llm_sdk` wrapper:
- We implemented `CustomTokenizer` as a Pydantic `BaseModel`.
- Employs GPT-2 byte-to-unicode mapping to ensure reversible, lossless encoding of arbitrary byte sequences.
- BPE merge ranking is implemented using pre-parsed `dict_merge` tables.
- Special tokens (e.g., `<|im_start|>`, `<|im_end|>`) are split cleanly with pre-compiled regex patterns.
- `decode_list` buffers multi-byte UTF-8 byte sequences to prevent decoding corruption across token boundaries.

### Pre-Computed First-Character Indexing

Iterating over 151,643 vocabulary tokens to evaluate validity at every step is a major performance bottleneck in Python.
- During initialization, `CustomTokenizer` pre-computes `_tokens_by_first_char: dict[str, list[int]]` and `_decoded_tokens: dict[int, str]`.
- When the Trie allows characters `{'a', 'b'}`, the decoder queries only tokens starting with those specific characters, **pruning 99.8% of the vocabulary from consideration instantly**.

### Pydantic v2 Schema Enforcement

All data structures—including the tokenizer configuration, internal validation models, and function specifications—are encapsulated in Pydantic `BaseModel` classes with explicit type constraints and post-init hooks, ensuring strict compliance with subject guidelines.

---

## Performance Analysis

### Benchmark Results

The pipeline was benchmarked against the standard test suite `data/input/function_calling_tests.json`, comprising 11 diverse natural language requests:

| Test Prompt | Target Function | Extracted Parameters | Status |
| :--- | :--- | :--- | :---: |
| *"What is the sum of 2 and 3?"* | `fn_add_numbers` | `{"a": 2.0, "b": 3.0}` | **PASS** |
| *"What is the sum of 265 and 345?"* | `fn_add_numbers` | `{"a": 265, "b": 345}` | **PASS** |
| *"Greet shrek"* | `fn_greet` | `{"name": "shrek"}` | **PASS** |
| *"Greet john"* | `fn_greet` | `{"name": "john"}` | **PASS** |
| *"Reverse the string 'hello'"* | `fn_reverse_string` | `{"s": "hello"}` | **PASS** |
| *"Reverse the string 'world'"* | `fn_reverse_string` | `{"s": "world"}` | **PASS** |
| *"What is the square root of 16?"* | `fn_get_square_root` | `{"a": 16}` | **PASS** |
| *"Calculate the square root of 144"* | `fn_get_square_root` | `{"a": 144}` | **PASS** |
| *"Replace all numbers in 'Hello 34...' with NUMBERS"* | `fn_substitute_string_with_regex` | `{"source_string": "...", "regex": "([0-9]+)", "replacement": "NUMBERS"}` | **PASS** |
| *"Replace all vowels in 'Programming is fun' with asterisks"* | `fn_substitute_string_with_regex` | `{"source_string": "...", "regex": "([aeiouAEIOU])", "replacement": " *"}` | **PASS** |
| *"Substitute the word 'cat' with 'dog' in 'The cat sat...' "* | `fn_substitute_string_with_regex` | `{"source_string": "...", "regex": "cat", "replacement": "dog"}` | **PASS** |

- **Accuracy**: **100% (11/11 prompts)** correctly classified with perfect parameter fidelity.
- **Syntactic Validity**: **100% valid JSON** across all executions.
- **Schema Compliance**: 0 missing parameters, 0 unknown keys, 0 type mismatches.

### Execution Metrics

Measured on standard commodity CPU hardware via `make debug` (`time uv run python -m src`):

- **Elapsed Total Time**: **~2 minutes 55 seconds** (for all 11 prompts, including model loading and tokenization).
- **Average Latency**: **~15.9 seconds per prompt** on CPU (under 1 second on GPU/MPS).
- **Peak Memory (RSS)**: **~2.44 GB** (model weights + vocabulary structures).
- **Time Limit Compliance**: Well within the 5-minute requirement defined in the subject.

---

## Challenges Faced

1. **Subword BPE Token Splitting**:
   - *Problem*: The model often emits tokens that contain leading whitespace markers or partial words (e.g., `Ġfn_` vs `fn_`). A naive character-level match fails if the token boundary does not align with the Trie step.
   - *Solution*: Pre-decoding tokens and maintaining normalized string representations inside `_decoded_tokens`, walking the Trie path character by character for each candidate token.

2. **String Delimiter Run-ons**:
   - *Problem*: In free generation, small language models often fail to emit the closing quotation mark `"` and continue generating conversational filler or hallucinated arguments.
   - *Solution*: Implemented a competitive logit comparison: tracking the best quote-bearing token against the best non-quote token. If the quote token is preferred by the model, string decoding terminates immediately.

3. **Vocabulary Size vs. Latency**:
   - *Problem*: Iterating over all 151,000+ tokens in pure Python at every generation step caused unacceptable latency (>20 minutes total runtime).
   - *Solution*: Implemented first-character hash mapping (`_tokens_by_first_char`). Evaluating only the tokens starting with Trie-allowed characters reduced candidate checks by >99%, bringing execution time down to ~2m55s on CPU.

4. **Multi-byte UTF-8 Streaming**:
   - *Problem*: Non-ASCII characters can be split across multiple BPE tokens. Decoding individual token IDs sequentially led to Unicode replacement characters (`\ufffd`).
   - *Solution*: Implemented byte accumulation in `decode_list`, buffering raw byte arrays and decoding UTF-8 only when encountering token boundaries or complete code points.

---

## Testing Strategy

- **Static Type Checking & Standards**: Checked with `flake8` and `mypy` using strict flags (`--disallow-untyped-defs`, `--check-untyped-defs`, `--warn-return-any`).
- **PEP 257 Docstring Compliance**: All modules, classes, and public/private methods contain standardized docstrings describing arguments, returns, and exceptions.
- **Input Validation**: `validate_functions_definition` checks that input JSON specifications conform strictly to the expected structure before execution.
- **Resilience and Error Handling**: The program wraps all file operations and generation stages with descriptive exception handlers, ensuring clean terminations without unhandled stack traces on invalid JSON, missing files, or keyboard interrupts.

---

## Example Usage

### Running via Command Line

```bash
# Process default input files
uv run python -m src

# Process custom input and output files
uv run python -m src \
  --functions_definition data/input/functions_definition.json \
  --input data/input/function_calling_tests.json \
  --output data/output/function_calling_results.json
```

### Sample Input Prompt

```json
{
  "prompt": "What is the sum of 265 and 345?"
}
```

### Generated Output (`data/output/function_calling_results.json`)

```json
[
  {
    "prompt": "What is the sum of 265 and 345?",
    "name": "fn_add_numbers",
    "parameters": {
      "a": 265,
      "b": 345
    }
  }
]
```

---

## Bonus Features

This project implements several optional bonus enhancements:

1. **Custom Tokenizer Implementation (`CustomTokenizer`)**:
   - Completely recoded BPE tokenizer in `src/custom_tokenizer.py`.
   - Never invokes `model.encode()` or `model.decode()`.
   - Loads vocabulary from `vocab.json`, merges from `merges.txt`, and added tokens from `tokenizer.json`.
   - Reconstructs text using custom byte-level UTF-8 conversion.

2. **Performance Optimizations**:
   - **Trie Auto-Completion**: Bypasses the model forward pass entirely when a function name has reached an unambiguous suffix.
   - **First-Character Vocabulary Index**: Pre-indexes candidate tokens by first character for rapid logit mask evaluation.
   - **Numeric Digit Filtering**: Restricts numeric parameters with `frozenset` lookups for optimal CPU throughput.

---

## Resources

### References and Literature

- **Constrained Decoding in Language Models**:
  - Willard, B. T., & Louf, R. (2023). *Efficient Guided Generation for Large Language Models*. arXiv:2307.09702.
  - Ugare, S., et al. (2024). *SynCode: Grammar-Augmented LLM Generation*. arXiv:2403.01632.
- **Byte-Pair Encoding (BPE)**:
  - Sennrich, R., Haddow, B., & Birch, A. (2016). *Neural Machine Translation of Rare Words with Subword Units*. ACL.
  - Radford, A., et al. (2019). *Language Models are Unsupervised Multitask Learners* (GPT-2 paper).
- **Data Structures & Frameworks**:
  - Fredkin, E. (1960). *Trie Memory*. Communications of the ACM, 3(9), 490-499.
  - [Pydantic v2 Documentation](https://docs.pydantic.dev/latest/)
  - [Qwen Model Family & Tokenizer Reference](https://huggingface.co/Qwen)

### AI Usage Disclosure

In compliance with 42 curriculum guidelines, artificial intelligence was utilized as a development and pair-programming assistant in the following areas:
- **Architecture Exploration**: Brainstorming the trade-offs between full grammar pushdown automata versus prefix injection with trie-constrained decoding.
- **Docstring & Type Annotation Refinement**: Reviewing PEP 257 docstring formatting and ensuring strict typing compliance across all modules.
- **Debugging Assistance**: Diagnosing UTF-8 byte boundary reconstruction issues in the custom BPE tokenizer across multi-byte token boundaries.
- **Performance Profiling**: Formulating caching strategies (`_tokens_by_first_char`) to optimize vocabulary filtering latency.
