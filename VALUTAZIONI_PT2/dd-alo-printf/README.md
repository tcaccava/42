*This project has been created as part of the 42 curriculum by dd-alo.*

# ft_printf

## Description

`ft_printf` is a C implementation of the mandatory subset of the standard `printf` function, developed for the 42 curriculum. It is delivered as a static library named `libftprintf.a` and writes formatted output to standard output without implementing the buffering used by the original `printf`.

The function returns the number of characters written or `-1` if a write operation fails.

### Supported conversions

| Conversion | Description |
| --- | --- |
| `%c` | Print one character. |
| `%s` | Print a string; a null pointer is displayed as `(null)`. |
| `%p` | Print a pointer in lowercase hexadecimal with the `0x` prefix; a null pointer is displayed as `(nil)` on the target Linux environment. |
| `%d`, `%i` | Print a signed decimal integer. |
| `%u` | Print an unsigned decimal integer. |
| `%x` | Print an unsigned hexadecimal integer using lowercase digits. |
| `%X` | Print an unsigned hexadecimal integer using uppercase digits. |
| `%%` | Print a literal percent sign. |

This version implements the mandatory conversion set only. Field width, precision, and additional formatting flags are outside its scope.

## Instructions

### Requirements

- A C compiler (`cc`), `make`, and `ar`.
- The included `libft/` directory, with its C sources, header, and Makefile.

### Build

From the root of this repository, run:

```sh
make
```

The Makefile builds `libft/libft.a`, copies its contents into the final archive, and adds the `ft_printf` object files to create `libftprintf.a` in the repository root.

Other available commands:

```sh
make clean    # Remove compiled object files
make fclean   # Remove object files and static archives
make re       # Rebuild from scratch
```

Running `make` again without changes should not rebuild the completed archive unnecessarily.

### Usage

Create a separate test file, for example `main.c`, containing:

```c
#include "ft_printf.h"

int main(void)
{
    ft_printf("Hello, %s! Answer: %d\n", "42", 42);
    return (0);
}
```

Compile and run it from the repository root after `make`:

```sh
cc -Wall -Wextra -Werror main.c -L. -lftprintf -o demo
./demo
```

Expected output:

```text
Hello, 42! Answer: 42
```

The example `main.c` is not part of the submitted library.

## Algorithm and data structures

### Parsing the format string

`ft_printf` begins by initializing a `va_list` with `va_start`. It traverses the format string from left to right. Ordinary characters are written directly with `ft_print_char`. When it encounters `%`, it advances to the conversion character and calls `ft_print_format`, which retrieves the next variadic argument with the appropriate `va_arg` type and dispatches it to the corresponding printing helper.

Every helper returns the number of characters it printed, allowing `ft_printf` to accumulate the total. If a helper reports a write error, parsing stops, `va_end` is called, and the function returns `-1`.

### Numeric and pointer formatting

`ft_print_base` converts unsigned values using a supplied digit alphabet. It recursively divides the value by the length of that alphabet, prints the more significant digits first, then prints the final digit. The same helper is reused for unsigned decimal and both hexadecimal alphabets, avoiding separate conversion algorithms.

`ft_print_nbr` converts a signed `int` to `long` before negating negative values, which allows it to handle the minimum signed integer safely. The minus sign is printed separately, and the magnitude is passed to the decimal base printer.

`ft_print_ptr` prints a non-null pointer as `0x` followed by its lowercase hexadecimal representation; on the target Linux environment, a null pointer is represented as `(nil)`.

### Data structure choices

The implementation needs no custom structure, linked list, or dynamically allocated buffer. A `va_list` holds the variadic-argument state; local counters track the format position and the number of printed characters; constant digit strings define the supported numeric bases. Writing directly with `write` keeps the implementation small and satisfies the project's no-buffer-management requirement.

### Library integration

The project includes its own local copy of `libft`. The main Makefile builds that dependency through `libft/Makefile` and combines its archive with the `ft_printf` object files. In particular, `ft_print_base` uses the existing `ft_strlen` implementation from `libft`.

## Project structure

```text
.
├── Makefile
├── README.md
├── ft_printf.h
├── ft_printf.c
├── ft_printf_utils.c
└── libft/
    ├── Makefile
    ├── libft.h
    └── ft_*.c
```

The generated `.o` files and `.a` archives are build outputs, not source files that need to be committed.

## Resources

- The 42 *ft_printf* project subject (version 12.1): scope, mandatory conversions, compilation and submission requirements.
- `man 3 printf`: conversion specifications and standard return-value behavior.
- `man 2 write`: output and error reporting.
- C `<stdarg.h>` reference (`va_list`, `va_start`, `va_arg`, and `va_end`): handling variadic arguments.
- `man 1 ar` and GNU Make documentation: static archives and build dependencies.

### AI usage

AI assistance was used during development to explain variadic arguments, review the code, and prepare documentation. The final project was checked locally through compilation, Norminette, and functional testers by the author. 
