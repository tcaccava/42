*This project has been created as part of the 42 curriculum by gmauti.*

# ft_printf

---

## Description

`ft_printf` is a reimplementation of the C standard library `printf` function. The goal is to understand how variadic functions work in C, how a format string is parsed character by character, and how different data types are converted to text and written to standard output.

The project produces a static library `libftprintf.a` that can be linked into any C project as a drop-in replacement for `printf`.

It supports the following conversion specifiers: `%c`, `%s`, `%d`, `%i`, `%u`, `%x`, `%X`, `%p`, and `%%`.

The bonus part adds support for flags (`-`, `0`, `#`, `+`, space), field minimum width, and precision (`.`), allowing combinations like `%-10.5s`, `%+05d`, or `%#x`.

---

## Algorithm and Data Structure

### Format string parsing

The core of `ft_printf` is a loop that walks the format string one character at a time. When it encounters a `%`, it switches into parsing mode: it reads any flags, then the width, then the precision, and finally the conversion specifier. Once the specifier is known, it calls the appropriate handler and passes it the next variadic argument.

This is a **single-pass linear scan** — O(n) in the length of the format string — which is the standard approach used by real `printf` implementations.

### The `t_flags` struct

All formatting information collected during parsing is stored in a single `t_flags` struct:

```c
typedef struct s_flags
{
    int minus;      // left-align
    int zero;       // pad with zeros
    int dot;        // precision present
    int hash;       // alternate form (#)
    int plus;       // always show sign
    int space;      // space before positive numbers
    int width;      // minimum field width
    int precision;  // precision value
}   t_flags;
```

This struct is initialized to zero before each conversion and passed by value to each handler. Using a struct instead of individual parameters keeps the function signatures clean and makes it easy to add new flags without changing every function signature.

### Padding logic

Each handler computes the length of the output it is about to produce, then calculates `pad = width - len`. If `pad > 0`, it prints either spaces or zeros before or after the value depending on the `-` and `0` flags. The `-` flag takes priority over `0` (left-align always uses spaces).

### Recursion for number printing

Integer and hexadecimal values are printed using simple recursive functions: divide by the base, recurse, then print the remainder. This avoids the need for a temporary buffer and keeps the code short. The base case is when the value is less than the base.

### Special cases

- `INT_MIN` (-2147483648) cannot be negated without overflow, so it is handled as a hardcoded string.
- `NULL` strings print as `(null)`.
- `NULL` pointers print as `(nil)`.
- `%p` reads the argument as `unsigned long` to correctly handle 64-bit addresses.

---

## Instructions

### Compilation

```bash
git clone https://github.com/gmauti/ft_printf.git
cd ft_printf
make
```

This produces `libftprintf.a`.

### Linking in your project

```bash
cc -Wall -Wextra -Werror main.c libftprintf.a -o program
```

### Header

```c
#include "ft_printf.h"
```

### Makefile targets

| Target | Description |
|--------|-------------|
| `make` | Compile the library |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and library |
| `make re` | Full recompile from scratch |

---

## Usage Examples

```c
ft_printf("Hello, %s!\n", "world");
ft_printf("Pointer: %p\n", &x);
ft_printf("Signed: %+d | Unsigned: %u\n", -42, 42);
ft_printf("Hex: %#x | Upper: %#X\n", 255, 255);
ft_printf("Padded: %-10s|\n", "left");
ft_printf("Zero:   %05d\n", 42);
ft_printf("Precision: %.5s\n", "hello world");
```

Output:
```
Hello, world!
Pointer: 0x7fff5fbff5a0
Signed: -42 | Unsigned: 42
Hex: 0xff | Upper: 0XFF
Padded: left      |
Zero:   00042
Precision: hello
```

---

## File Structure

```
ft_printf/
├── ft_printf.h       — header, t_flags struct, prototypes
├── ft_printf.c       — main function and format parser
├── ft_utils.c        — putchar, putstr, putnbr, puthex, putptr
├── ft_printf_c.c     — %c handler
├── ft_printf_s.c     — %s handler
├── ft_printf_d.c     — %d / %i handler
├── ft_printf_u.c     — %u handler
├── ft_printf_x.c     — %x / %X handler
├── ft_printf_p.c     — %p handler
└── Makefile
```

---

## Resources

- [C Standard — printf specification (cppreference)](https://en.cppreference.com/w/c/io/fprintf)
- [Variadic functions in C (cppreference)](https://en.cppreference.com/w/c/variadic)
- [IEEE — stdarg.h explained](https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/stdarg.h.html)
- [42 Docs — ft_printf subject](https://cdn.intra.42.fr/pdf/pdf/104394/en.subject.pdf)

### AI usage

Claude (Anthropic) was used as a learning and debugging assistant throughout this project. Specifically:

- **Concept explanation**: variadic functions (`va_start`, `va_arg`, `va_end`, `va_copy`), the parsing loop structure, and padding logic were explained interactively.
- **Debugging**: Claude helped identify bugs such as using `% 10` instead of `% 16` in the hex printer, and incorrect argument types for `%p` (`unsigned long` vs `void *`).
- **Code review**: individual functions were reviewed for correctness and edge cases (e.g. `INT_MIN`, `NULL` pointers, `NULL` strings).
- Claude did **not** write the final submitted code autonomously — all files were written, understood, and typed by the student with Claude used as a reference and debugger.

