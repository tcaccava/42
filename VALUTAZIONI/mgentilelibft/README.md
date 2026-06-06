# libft

> My first C library — a re-implementation of essential standard C functions, built from scratch as part of the 42 curriculum.

---

## What is libft?

**libft** is a foundational project at 42 School. The goal is to re-implement a set of standard C library functions, understanding how they work under the hood, without using the originals. The result is a personal static library (`libft.a`) that can be reused in future projects.

---

## Functions

### Character checks & conversions

| Function | Description |
|---|---|
| `ft_isalpha` | Checks if a character is alphabetic |
| `ft_isdigit` | Checks if a character is a digit |
| `ft_isalnum` | Checks if a character is alphanumeric |
| `ft_isascii` | Checks if a character is in the ASCII table |
| `ft_isprint` | Checks if a character is printable |
| `ft_toupper` | Converts a character to uppercase |
| `ft_tolower` | Converts a character to lowercase |

### Memory functions

| Function | Description |
|---|---|
| `ft_memset` | Fills a memory area with a given byte |
| `ft_bzero` | Sets a memory area to zero |
| `ft_memcpy` | Copies a memory area |
| `ft_memmove` | Copies a memory area, handling overlaps |
| `ft_memchr` | Searches for a byte in a memory area |
| `ft_memcmp` | Compares two memory areas |
| `ft_calloc` | Allocates and zeroes memory |

### String functions

| Function | Description |
|---|---|
| `ft_strlen` | Returns the length of a string |
| `ft_strlcpy` | Copies a string with size limit |
| `ft_strlcat` | Concatenates strings with size limit |
| `ft_strchr` | Locates a character in a string (first occurrence) |
| `ft_strrchr` | Locates a character in a string (last occurrence) |
| `ft_strncmp` | Compares two strings up to n characters |
| `ft_strnstr` | Locates a substring in a string |
| `ft_strdup` | Duplicates a string using malloc |
| `ft_substr` | Extracts a substring from a string |
| `ft_strjoin` | Concatenates two strings into a new one |
| `ft_strtrim` | Trims characters from both ends of a string |
| `ft_split` | Splits a string by a delimiter into an array |
| `ft_strmapi` | Applies a function to each character, returns new string |
| `ft_striteri` | Applies a function to each character in place |

### Conversion functions

| Function | Description |
|---|---|
| `ft_atoi` | Converts a string to an integer |
| `ft_itoa` | Converts an integer to a string |

### Output functions

| Function | Description |
|---|---|
| `ft_putchar_fd` | Writes a character to a file descriptor |
| `ft_putstr_fd` | Writes a string to a file descriptor |
| `ft_putendl_fd` | Writes a string followed by a newline to a file descriptor |
| `ft_putnbr_fd` | Writes an integer to a file descriptor |

### Linked list functions (bonus)

| Function | Description |
|---|---|
| `ft_lstnew` | Creates a new list node |
| `ft_lstadd_front` | Adds a node at the beginning of a list |
| `ft_lstadd_back` | Adds a node at the end of a list |
| `ft_lstsize` | Returns the number of nodes in a list |
| `ft_lstlast` | Returns the last node of a list |
| `ft_lstdelone` | Deletes a single node |
| `ft_lstclear` | Deletes and frees an entire list |
| `ft_lstiter` | Applies a function to each node's content |
| `ft_lstmap` | Creates a new list by applying a function to each node |

---

## Usage

### Compilation

```bash
make        # builds libft.a
make bonus  # includes linked list functions
make clean  # removes object files
make fclean # removes object files and libft.a
make re     # fclean + make
```

### Using in a project

```bash
gcc your_file.c -L. -lft -o your_program
```

```c
#include "libft.h"
```

---

## Notes

- No use of restricted functions — each function is implemented from scratch
- Memory is carefully managed: every `malloc` is paired with a `free`

---

*Project completed as part of the 42 School curriculum.*
