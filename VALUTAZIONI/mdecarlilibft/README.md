# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    README.md                                          :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mdecarli <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/06/02 16:34:57 by mdecarli          #+#    #+#              #
#    Updated: 2026/06/02 18:26:10 by mdecarli         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

*This project has been created as part of the 42 curriculum by mdecarli.*

# libft

## Description

The libft project consists of recreating a selection of standard C library (`libc`) functions from scratch, along with additional functions useful for string manipulation, memory management, and linked lists. The goal of this project is to deeply understand the low-level mechanics of memory allocation and pointers in C, while building a static library (`.a`) that will be reused in all subsequent projects throughout the 42 cursus.

The library is divided into three parts:

- **Libc functions** — reimplementations of standard functions from `<string.h>`, `<ctype.h>`, `<stdlib.h>`, and others.
- **Additional functions** — utility functions not in the standard library but useful for future projects.
- **Linked lists** — a complete linked list implementation using a custom `t_list` struct.

---

## Functions List

### Standard C Library Functions (Libc)
* `ft_isalpha` - Checks for an alphanumeric character.
* `ft_isdigit` - Checks for a digit (0 through 9).
* `ft_isalnum` - Checks for an alphanumeric character.
* `ft_isascii` - Checks whether c fits into the ASCII character set.
* `ft_isprint` - Checks for any printable character.
* `ft_strlen` - Computes the length of a string.
* `ft_memset` - Fills memory with a constant byte.
* `ft_bzero` - Zeroes a byte string.
* `ft_memcpy` - Copies memory area.
* `ft_memmove` - Copies memory area handling overlaps securely.
* `ft_strlcpy` - Size-bounded string copying.
* `ft_strlcat` - Size-bounded string concatenation.
* `ft_toupper` - Converts a char to uppercase.
* `ft_tolower` - Converts a char to lowercase.
* `ft_strchr` - Locates the first occurrence of a character in a string.
* `ft_strrchr` - Locates the last occurrence of a character in a string.
* `ft_strncmp` - Compares two strings up to N characters.
* `ft_memchr` - Scans memory for a byte.
* `ft_memcmp` - Compares memory areas.
* `ft_strnstr` - Locates a substring in a string with a length limit.
* `ft_atoi` - Converts a string to an integer.

### Additional Functions
* `ft_calloc` - Allocates memory and initializes it to zero.
* `ft_strdup` - Duplicates a string by allocating new memory.
* `ft_substr` - Extracts a substring from a string.
* `ft_strjoin` - Concatenates two strings into a new allocated string.
* `ft_strtrim` - Trims specified characters from the beginning and end of a string.
* `ft_split` - Splits a string into an array of strings using a delimiter.
* `ft_itoa` - Converts an integer to a string.
* `ft_strmapi` - Applies a function to each character of a string to create a new string.
* `ft_striteri` - Applies a function to each character of a string (modifying it in-place).
* `ft_putchar_fd` - Outputs a character to a given file descriptor.
* `ft_putstr_fd` - Outputs a string to a given file descriptor.
* `ft_putendl_fd` - Outputs a string followed by a newline to a given file descriptor.
* `ft_putnbr_fd` - Outputs an integer to a given file descriptor.

### Linked List Functions
* `ft_lstnew` - Creates a new list node.
* `ft_lstadd_front` - Adds a node at the beginning of the list.
* `ft_lstsize` - Counts the number of nodes in the list.
* `ft_lstlast` - Returns the last node of the list.
* `ft_lstadd_back` - Adds a node at the end of the list.
* `ft_lstdelone` - Deletes a single node and frees its content.
* `ft_lstclear` - Deletes and frees an entire list from a given node onwards.
* `ft_lstiter` - Iterates over a list and applies a function to each node's content.
* `ft_lstmap` - Iterates over a list and creates a new modified list.

## How to Use the Library

### 1. Compilation

To compile the library, clone the repository and run `make` in the root directory:

```bash
make
```
This will compile all mandatory functions and produce `libft.a`.

### 2. Cleaning

```bash
make clean    # Removes all object files (.o)
make fclean   # Removes object files and the libft.a library
make re       # Performs a full clean and recompiles everything from scratch
```

### Using the Library in Another Project

#include "libft.h"

```bash
gcc your_file.c -L. -lft -o your_program
```

Make sure `libft.h` and `libft.a` are accessible from your project directory.

### Testing

- [Tripouille/libftTester](https://github.com/Tripouille/libftTester) — the tester used to validate this library

## Resources

### References
* [C Library Man Pages](https://linux.die.net/man/) - Official documentation for standard libc functions.
* [Tripouille/libftTester](https://github.com/Tripouille/libftTester) - The main test suite used to validate this library.

### AI Usage Disclosure
In compliance with the 42 AI Instructions, Artificial Intelligence was used exclusively as a learning and explanation tool during this project. Specifically:
* **Task Isolation:** AI was utilized to analyze failed test cases from the tester (such as integer overflows in `ft_calloc` or unsigned char comparisons in `ft_strncmp`).
* **Code Implementation:** No final code was auto-generated or copy-pasted from AI tools. All logic, pointer manipulations, and structure adaptations were written and reasoned manually by the author to ensure strong technical foundations.

---

All code was written manually by Maria Decarli. AI was used exclusively as a learning and explanation tool, not to generate final code.
