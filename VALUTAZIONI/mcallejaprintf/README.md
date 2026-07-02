# ft_printf - Your First Formatting Library

*This project was created as part of the 42 curriculum by [mcalleja].*

## Description

`ft_printf` is a custom re-implementation of the standard C `printf` function, built from scratch as part of the 42 school curriculum. The goal of this project is to deeply understand variadic functions, string formatting, and low-level output handling in C — without relying on the standard library's own implementation.

The function handles the following conversion specifiers:

| Specifier | Description |
|-----------|-------------|
| `%c` | Single character |
| `%s` | String of characters |
| `%d` / `%i` | Signed decimal integer |
| `%u` | Unsigned decimal integer |
| `%x` | Unsigned hexadecimal (lowercase) |
| `%X` | Unsigned hexadecimal (uppercase) |
| `%p` | Pointer address |
| `%%` | Literal percent sign |

---

## Technical Decisions & Data Structures

### Data Structures
This project processes data line by line as a simple stream of text. Because of this, no advanced data structures like trees or linked lists were required. We only use standard **character arrays (strings)** and **basic variables** allocated on the stack. This keeps memory usage extremely low and efficient.

### Algorithm For Numbers & Pointers
To convert numbers into base 10 and base 16 (hexadecimal):
1. **Math Division:** We use a simple loop with the modulo operator (`%`) and division (`/`) to extract digits one by one from right to left.
2. **Pointers (`%p`):** Memory addresses are converted by casting the `void *` pointer into an `unsigned long` variable. This ensures the address fits perfectly on 64-bit systems without losing data.

---

## Technical Features
- **Language:** C
- **Compilation Flags:** `-Wall -Werror -Wextra` 
- **Norm:** Fully compliant with the 42 Norm guidelines.
- **Global Variables:** Strictly forbidden (all functions are self-contained).

---

## Instructions

### Prerequisites

- GCC or any C compiler compatible with `-Wall -Wextra -Werror`
- GNU Make

### Compiling the Library

Navigate to the root directory and run:

```bash
make
```

This will compile both the internal `libft` dependency and the `ft_printf` source files, generating the static library archive `libftprintf.a`.

### Using the Library in Your Project

Link `libftprintf.a` when compiling your project:

```bash
gcc main.c -L. -lftprintf -o my_program
```

### Cleaning Up

Remove object files:

```bash
make clean
```

Remove object files and compiled libraries:

```bash
make fclean
```

Recompile everything from scratch:

```bash
make re
```

### Project Structure

```
printf/
├── Makefile
├── README.md
├── ft_printf.h
├── ft_printf.c
├── ft_printf_format.c
├── ft_printfhex.c
├── ft_printfptr.c
├── ft_printfunbr.c
├── ft_putchar_len.c
├── ft_putstr_len.c
├── ft_printfnbr.c
└── libft/
    ├── Makefile
    ├── libft.h
    ├── ft_strlen.c
    └── ... (other libft source files)
```
---

## Resources

### References

- [C Standard Library — printf documentation](https://en.cppreference.com/w/c/io/fprintf)
- [Variadic Arguments in C — stdarg.h](https://en.cppreference.com/w/c/variadic)
- [42 Official Norminette Guidelines](https://github.com/42School/norminette)

---

**Author**: mcalleja  
