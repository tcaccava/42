*This project has been created as part of the 42 curriculum by gleccia.*

# ft_printf

## Description
The **ft_printf** project consists of recoding the standard C library function `printf(3)`. The primary goal of this project is to gain a thorough understanding of variadic functions in C (`stdarg.h`), formatted output manipulation, and low-level data conversion (hexadecimal, pointers, integers).

The resulting library, `libftprintf.a`, provides a custom implementation of `ft_printf` that handles standard format specifiers, correctly tracking and returning the total number of printed characters.

### Implemented Specifiers
- `%c` : Prints a single character.
- `%s` : Prints a string (handles `NULL` pointers by outputting `(null)`).
- `%p` : Prints a memory address pointer in hexadecimal format preceded by `0x` (handles `NULL` pointers by outputting `(nil)`).
- `%d` / `%i` : Prints a signed decimal integer.
- `%u` : Prints an unsigned decimal integer.
- `%x` : Prints a number in lowercase hexadecimal format.
- `%X` : Prints a number in uppercase hexadecimal format.
- `%%` : Prints a percent sign.

---

## Instructions

### Compilation
The library is compiled using `make`. The provided `Makefile` compiles all `.c` files with `cc` and flags `-Wall -Wextra -Werror`, creating the static library `libftprintf.a`.

Available Makefile rules:
- `make` or `make all`: Compiles the source files and creates `libftprintf.a`.
- `make clean`: Removes all compiled object files (`.o`).
- `make fclean`: Removes object files and the generated library `libftprintf.a`.
- `make re`: Performs `fclean` followed by `all` to recompile the project.

### Usage Example
To use `ft_printf` in your project, include the header header and link the library during compilation:

1. Create a `main.c`:
```c
#include "ft_printf.h"

int main(void)
{
    int count;

    count = ft_printf("Hello %s! Value: %d, Hex: %x, Pointer: %p\n", "42", 42, 255, &count);
    ft_printf("Printed characters: %d\n", count);
    return (0);
}
