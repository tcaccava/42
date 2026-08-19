_This project has been created as part of the 42 curriculum by jabdujal._

# Libft - Your Very First C Library

## Description

**Libft** is the first individual project at 42. The goal of this project is to re-create standard C library (`libc`) functions, alongside custom memory, string, and linked list manipulation utilities.

Building this library from scratch provides a deep understanding of data structures, memory allocation (`malloc`/`free`), pointer arithmetic, and foundational C algorithms. The resulting `libft.a` static library will serve as a foundational toolkit for future C projects in the 42 curriculum.

---

## Functions Included

### Part 1 - Standard Libc Functions

Re-implementations of standard C library functions adhering to system `man` page behaviors:

- **Character Classification:** `ft_isalpha`, `ft_isdigit`, `ft_isalnum`, `ft_isascii`, `ft_isprint`
- **Character Conversion:** `ft_toupper`, `ft_tolower`
- **Memory Operations:** `ft_memset`, `ft_bzero`, `ft_memcpy`, `ft_memmove`, `ft_memchr`, `ft_memcmp`, `ft_calloc`
- **String Operations:** `ft_strlen`, `ft_strlcpy`, `ft_strlcat`, `ft_strchr`, `ft_strrchr`, `ft_strncmp`, `ft_strnstr`, `ft_strdup`
- **Conversion:** `ft_atoi`

### Part 2 - Additional Functions

Utility functions for string manipulation, memory allocation, and file descriptor output:

- **`ft_substr`**: Extracts a substring from a given string.
- **`ft_strjoin`**: Concatenates two strings into a new dynamically allocated string.
- **`ft_strtrim`**: Trims specified characters from the beginning and end of a string.
- **`ft_split`**: Splits a string into an array of substrings based on a delimiter character.
- **`ft_itoa`**: Converts an integer to its string representation.
- **`ft_strmapi`**: Applies a function to each character of a string to create a new string.
- **`ft_striteri`**: Applies a function to each character of a string in-place by passing its address.
- **`ft_putchar_fd`**: Writes a character to a given file descriptor.
- **`ft_putstr_fd`**: Writes a string to a given file descriptor.
- **`ft_putendl_fd`**: Writes a string followed by a newline to a given file descriptor.
- **`ft_putnbr_fd`**: Writes an integer to a given file descriptor.

### Part 3 - Linked List Functions

Functions designed to manipulate linked lists defined by the `t_list` structure:

- **`ft_lstnew`**: Creates a new list node.
- **`ft_lstadd_front`**: Adds a new node at the beginning of a list.
- **`ft_lstsize`**: Counts the number of nodes in a list.
- **`ft_lstlast`**: Returns the last node of a list.
- **`ft_lstadd_back`**: Adds a new node at the end of a list.
- **`ft_lstdelone`**: Frees the content of a single node using a deletion function.
- **`ft_lstclear`**: Deletes and frees a node and all of its successors.
- **`ft_lstiter`**: Iterates through a list and applies a function to each node's content.
- **`ft_lstmap`**: Iterates through a list, applies a function to create a new list, and frees on error.

---

## Instructions

### Compilation

Clone the repository and navigate into the root directory:

```bash
git clone <your_repository_url> libft
cd libft
```
