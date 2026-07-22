*This project has been created as part of the 42 curriculum by mde-matt.*

# ft_printf

## Description

ft_printf is a C reimplementation of the standard `printf` function from the C standard library. The goal is to understand how variadic functions work in C, how format strings are parsed, and how different data types are converted to their string representation for output.

The function signature mirrors the standard:

```c
int ft_printf(const char *format, ...);
```

It returns the total number of bytes written to standard output, exactly as the original does.

### Supported conversions

| Specifier | Description |
|-----------|-------------|
| `%c` | Single character |
| `%s` | String |
| `%p` | Pointer address in hexadecimal |
| `%d` | Signed decimal integer |
| `%i` | Signed decimal integer |
| `%u` | Unsigned decimal integer |
| `%x` | Unsigned hexadecimal integer (lowercase) |
| `%X` | Unsigned hexadecimal integer (uppercase) |
| `%%` | Literal percent sign |

### Supported flags (bonus)

| Flag | Description |
|------|-------------|
| `-` | Left-align output within the field width |
| `0` | Pad with zeros instead of spaces |
| `.` | Precision — minimum digit count for numbers, maximum character count for strings |
| Width | Minimum field width as a decimal number |
| `#` | Prefix `0x` / `0X` for `%x` / `%X` |
| `+` | Always print the sign for numeric conversions |
| ` ` | Print a space before positive numbers |

All flag combinations are supported across all applicable conversions.

### Algorithm and data structure

The format string is scanned character by character in a single pass. When a `%` is encountered, a dedicated parsing function (`parser`) reads the following characters and populates a `t_flags` struct before any output is produced:

```c
typedef struct s_flags
{
    int width;
    int minun;
    int zeros;
    int precision;
    int prefix;
    int spaces;
    int plusle;
}   t_flags;
```

Storing all flag state in a struct before dispatching to the handler was the key architectural decision. It allows each handler function (`printer_d`, `printer_x`, etc.) to receive the complete formatting context as a single argument, making it straightforward to apply padding, precision, and sign rules independently for each conversion type.

Padding is handled by dedicated functions (`widthpadding`, `zerospadding`, `widthhexpadding`, etc.) that are called before and after the value is printed, depending on alignment. This separation keeps each handler within the 25-line limit imposed by the 42 norm.

Number-to-string conversion for unsigned types uses an iterative approach (`utoa`) to avoid allocating unnecessary intermediate buffers. Hexadecimal conversion uses recursion to naturally produce digits in the correct order without reversing a buffer.

## Instructions

### Compilation

```bash
make
```

This produces `libftprintf.a` in the project root.

### Linking your program

```bash
cc your_file.c libftprintf.a -o program
```

Or using the `-L` and `-l` flags:

```bash
cc your_file.c -L. -lftprintf -o program
```

### Include

```c
#include "libftprintf.h"
```

### Makefile rules

| Rule | Description |
|------|-------------|
| `make` / `make all` | Compile the library |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and the library |
| `make re` | Full recompilation |
| `make bonus` | Alias for `all` — bonus is included by default |

### Usage examples

```c
ft_printf("Hello, %s!\n", "world");
ft_printf("%10.5d\n", 42);       // "     00042"
ft_printf("%-20s|\n", "left");   // "left                |"
ft_printf("%#010x\n", 255);      // "0x000000ff"
ft_printf("%+d\n", 42);          // "+42"
```

## Resources

- [printf(3) — Linux man page](https://man7.org/linux/man-pages/man3/printf.3.html)
- [C Standard — variadic functions (stdarg.h)](https://en.cppreference.com/w/c/variadic)
- [Formatted output — GNU C Library](https://www.gnu.org/software/libc/manual/html_node/Formatted-Output.html)
- [printf format string — Wikipedia](https://en.wikipedia.org/wiki/Printf_format_string)

### Use of AI

Claude (Anthropic) was used as a learning assistant throughout this project. Specifically:

- **Concept clarification**: understanding how `va_list`, `va_start`, `va_arg`, and `va_end` work internally, and why `va_arg` requires an explicit type argument.
- **Debugging**: identifying issues with index management when parsing the format string, sign-extension edge cases with `INT_MIN` in `ft_itoa`, and byte-count discrepancies in return values.
- **Architecture guidance**: the decision to use a `t_flags` struct and parse all flags before dispatching to handlers was discussed and validated with AI assistance.
- **Code review**: AI was used to spot off-by-one errors in padding calculations and type mismatches between signed and unsigned comparisons flagged by `-Wextra`.

All code was written by the author; AI was used exclusively in an explanatory and review capacity.
