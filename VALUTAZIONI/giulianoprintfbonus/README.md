*This project has been created as part of the 42 curriculum by gipaglie.*

## Description
Project: ft_printf (base + bonus).  
Goal: Reimplement a subset of the C standard printf: parse format strings, handle conversion specifiers, flags, width, precision, and produce formatted output. This repository contains both the basic and bonus implementations with modularized converters, specifier handlers, and utility functions.

## Instructions
- Build:
  - Run: make or make bonus
- Usage:
  - Include the library headers in your program and call ft_printf similarly to standard printf.
  - Example usage (in your C program):
    ```
    #include "ft_printf.h"

    int main(void)
    {
        ft_printf("Hello %s %d\n", "world", 42);
        return 0;
    }
    ```
- Tests:
  - Compile provided tests or add test programs that call ft_printf with various format strings.
- Backup source files before running any preprocessing scripts.

## Resources
- C standard printf documentation and man pages.
- Tutorials on variadic functions (va_list, va_start, va_arg, va_end).
- Articles on integer-to-string conversion and formatting rules.
- AI usage: AI was used to search theory material (variadic arguments, structs, printf's flags use) and to draft and proofread the README. AI was NOT used to write the project source code.

## Algorithm & Data Structure — explanation and justification
- Overall approach: parse the format string sequentially, writing literal characters directly and invoking a handler when '%' is found.
- Parsing:
  - Use a flags struct (t_flags) to store parsed flags: left_align, zero, dot, width, precision, plus, space, hash, and conv_spec. Centralizing format state simplifies formatting decisions.
  - The parser (fill_flags) scans characters after '%' until a conversion specifier is found and updates t_flags fields incrementally (width and precision parsed numerically).
  - Reason: a single-pass parser is simple, efficient (O(n) over format length), and matches typical printf parsing behavior.
- Formatting data:
  - Use a separate t_f_data structure to hold computed lengths: precision_len, padding_len, total len, literal_notation pointer, and negative_arg flag.
  - Reason: separating parsed flags from computed layout keeps responsibilities clear: parsing vs. layout computation.
- Number handling:
  - Absolute-value helpers convert signed values to non-negative for digit extraction while recording sign separately; digit_len computes digit count in any base.
  - putnbr outputs numbers in the requested base; precision and padding are applied using computed lengths.
  - Reason: splitting numeric conversion, digit counting, and output simplifies correctness across decimal/hex/unsigned conversions and supports precision rules.
- Output strategy:
  - Handlers for each conversion family (chars/strings, integers, hex/pointer) produce output using primitives:
    - putnchar writes repeated padding characters.
    - print_literal_notation writes "0x"/"0X" when hash applies.
    - print_* functions orchestrate order: optional padding, sign or prefix, precision zeros, value bytes, and trailing padding for left alignment.
  - Reason: decomposition avoids duplicated logic and simplifies handling width/precision/flag interactions.
- Edge cases and rules:
  - Zero and precision interaction: printing of zero may be suppressed depending on precision and hash flag; width is still respected.
  - Sign/space/plus interactions: negative overrides plus/space.
  - Pointer handling: "(nil)" for NULL or "0x..." otherwise.
  - Reason: explicit handling mirrors standard printf semantics.
- Complexity:
  - Time complexity: O(n + m) where n = length of format string and m = total length of produced output. Each specifier processing cost depends on number size and width.
  - Space complexity: O(1) extra heap (small temporary allocations for literal notation), output written directly to fd 1 without large intermediate buffers.
- Justification:
  - Struct-based flags and formatting data: clearer, easier to extend, and reduces coupling.
  - Small primitive output functions: enable precise control over padding and ordering and simplify testing.
  - Single-pass parsing with separate layout computation: efficient and conceptually aligned with formatted output generation.

## Additional sections
- Files of interest:
  - src/: the core of the function and functions used by both the base version and the bonus version.
  - base/: basic implementation sources (specifier parsing, printing helpers).
  - bonus/: bonus implementation with extended flag parsing and formatting.
  - include/: header files (ft_printf.h, ft_printf_bonus.h).
- Known limitations:
  - Not a full replacement of libc printf; supports a subset as required by the 42 assignment.
  - Small dynamic allocation only for literal notation; most output written directly to stdout (as using fflush(stdout) right after each call of the original printf).

