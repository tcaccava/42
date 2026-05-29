*This project has been created as part of the 42 curriculum by lmezzaba.*

# libft

## Description

libft is a custom C library built from scratch as part of the 42 school curriculum. The goal is to reimplement a set of standard C library functions, along with additional utility functions and a linked list API. The library is compiled as a static archive (`libft.a`) and can be reused in future 42 projects.

The library is divided into three parts:

**Part 1 — Libc functions:** reimplementations of standard functions from `<string.h>`, `<ctype.h>`, and `<stdlib.h>`, prefixed with `ft_`.

**Part 2 — Additional functions:** utility functions not present in the standard libc, or present in a different form, such as string splitting, joining, trimming, and output to file descriptors.

**Part 3 — Linked list (bonus):** a full singly linked list API using the `t_list` structure.

---

## Library Overview

### Part 1 — Libc functions

| Function | Description |
|---|---|
| `ft_isalpha` | Returns 1 if `c` is alphabetic, 0 otherwise |
| `ft_isdigit` | Returns 1 if `c` is a digit, 0 otherwise |
| `ft_isalnum` | Returns 1 if `c` is alphanumeric, 0 otherwise |
| `ft_isascii` | Returns 1 if `c` is a valid ASCII character, 0 otherwise |
| `ft_isprint` | Returns 1 if `c` is a printable character, 0 otherwise |
| `ft_strlen` | Returns the length of a string |
| `ft_memset` | Fills a memory area with a given byte value |
| `ft_bzero` | Sets `n` bytes of a memory area to zero |
| `ft_memcpy` | Copies `n` bytes from `src` to `dest` (no overlap) |
| `ft_memmove` | Copies `n` bytes from `src` to `dest` (handles overlap) |
| `ft_strlcpy` | Copies a string into a buffer with size limit, returns source length |
| `ft_strlcat` | Appends a string to a buffer with size limit, returns total length |
| `ft_toupper` | Converts a lowercase letter to uppercase |
| `ft_tolower` | Converts an uppercase letter to lowercase |
| `ft_strchr` | Returns a pointer to the first occurrence of `c` in a string |
| `ft_strrchr` | Returns a pointer to the last occurrence of `c` in a string |
| `ft_strncmp` | Compares up to `n` bytes of two strings |
| `ft_memchr` | Searches for a byte value in a memory area |
| `ft_memcmp` | Compares two memory areas byte by byte |
| `ft_strnstr` | Finds the first occurrence of a substring within `len` bytes |
| `ft_atoi` | Converts a string to an integer |
| `ft_calloc` | Allocates zero-initialized memory for `nmemb` elements of `size` bytes |
| `ft_strdup` | Returns a malloc'd duplicate of a string |

### Part 2 — Additional functions

| Function | Description |
|---|---|
| `ft_substr` | Returns a malloc'd substring from string `s`, starting at `start` with max length `len` |
| `ft_strjoin` | Returns a malloc'd concatenation of strings `s1` and `s2` |
| `ft_strtrim` | Returns a malloc'd copy of `s1` with characters in `set` removed from both ends |
| `ft_split` | Returns a NULL-terminated array of strings obtained by splitting `s` on delimiter `c` |
| `ft_itoa` | Returns a malloc'd string representing the integer `n` (handles negatives) |
| `ft_strmapi` | Applies function `f` to each character of `s` and returns the resulting malloc'd string |
| `ft_striteri` | Applies function `f` to each character of `s` in place, passing index and address |
| `ft_putchar_fd` | Writes character `c` to file descriptor `fd` |
| `ft_putstr_fd` | Writes string `s` to file descriptor `fd` |
| `ft_putendl_fd` | Writes string `s` followed by a newline to file descriptor `fd` |
| `ft_putnbr_fd` | Writes integer `n` to file descriptor `fd` |

### Part 3 — Linked list

The linked list uses the following structure defined in `libft.h`:

```c
typedef struct s_list
{
    void          *content;
    struct s_list *next;
} t_list;
```

| Function | Description |
|---|---|
| `ft_lstnew` | Allocates and returns a new node with the given `content` |
| `ft_lstadd_front` | Adds a node at the beginning of the list |
| `ft_lstsize` | Returns the number of nodes in the list |
| `ft_lstlast` | Returns the last node of the list |
| `ft_lstadd_back` | Adds a node at the end of the list |
| `ft_lstdelone` | Frees a node's content using `del` and frees the node itself |
| `ft_lstclear` | Frees all nodes of the list using `del` and sets the pointer to NULL |
| `ft_lstiter` | Applies function `f` to the content of each node |
| `ft_lstmap` | Applies `f` to each node's content, returns a new list; uses `del` on failure |

---

## Instructions
### Compilation

To compile the library (includes all functions):
```bash
make
```

To clean object files:
```bash
make clean
```

To clean object files and the library:
```bash
make fclean
```

To recompile everything from scratch:
```bash
make re
```

### Using the library in your own project

After compiling, link `libft.a` to your program:
```bash
cc -Wall -Wextra -Werror main.c -L. -lft -o my_program
```

Include the header at the top of your C files:
```c
#include "libft.h"
```

---

## Resources

- [C Standard Library reference — cppreference.com](https://en.cppreference.com/w/c)
- [man pages online](https://man7.org/linux/man-pages/)
- [42 Norm documentation](https://github.com/42School/norminette)