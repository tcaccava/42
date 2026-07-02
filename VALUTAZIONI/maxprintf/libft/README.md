# Libft - Your First Library

*This project has been created as part of the 42 curriculum by [mcalleja].*

## Description

Libft is a collection of general-purpose functions in C that reimplements standard library behaviors and introduces utilities for string manipulation and linked lists.

The project is divided into three main parts:

1. **Part 1**: Reimplemented libc functions with the `ft_` prefix
2. **Part 2**: Additional string manipulation functions
3. **Part 3**: Implementation of linked lists

### Compilation

To compile the library:

```bash
make
```

This will generate the `libft.a` file in the root of the repository.

### Makefile Commands

- `make` - Compiles the library
- `make clean` - Removes object files (.o)
- `make fclean` - Removes object files and library
- `make re` - Recompiles from scratch
- `make bonus` - Compiles the library including bonus functions

## Library Contents

### Part 1: libc Functions

Reimplementations of standard C functions:

#### Character Classification Functions
- `ft_isalpha` - Check if character is a letter
- `ft_isdigit` - Check if character is a digit
- `ft_isalnum` - Check if character is alphanumeric
- `ft_isascii` - Check if character is ASCII
- `ft_isprint` - Check if character is printable
- `ft_toupper` - Convert character to uppercase
- `ft_tolower` - Convert character to lowercase

#### Memory Management Functions
- `ft_memset` - Fill memory with value
- `ft_bzero` - Zero out memory
- `ft_memcpy` - Copy memory
- `ft_memmove` - Copy memory with overlap handling
- `ft_memchr` - Search for byte in memory
- `ft_memcmp` - Compare memory
- `ft_calloc` - Allocate and zero-initialize memory

#### String Functions
- `ft_strlen` - Calculate string length
- `ft_strlcpy` - Copy string safely
- `ft_strlcat` - Concatenate string safely
- `ft_strchr` - Find character in string
- `ft_strrchr` - Find last character in string
- `ft_strncmp` - Compare n characters of strings
- `ft_strnstr` - Find substring in string
- `ft_atoi` - Convert string to integer
- `ft_strdup` - Duplicate a string

### Part 2: Additional Functions

Advanced string manipulation functions:

- `ft_substr` - Extract a substring
- `ft_strjoin` - Concatenate two strings (with malloc)
- `ft_strtrim` - Remove characters from edges
- `ft_split` - Split string using delimiter
- `ft_itoa` - Convert integer to string
- `ft_strmapi` - Apply function to each character
- `ft_striteri` - Iterate string with function
- `ft_putchar_fd` - Print character to file descriptor
- `ft_putstr_fd` - Print string to file descriptor
- `ft_putendl_fd` - Print string with newline to file descriptor
- `ft_putnbr_fd` - Print number to file descriptor

### Part 3: Linked Lists

Implementation of structure and functions for linked lists:

**t_list Structure:**
```c
typedef struct s_list
{
    void            *content;
    struct s_list   *next;
} t_list;
```

**Functions:**
- `ft_lstnew` - Create new node
- `ft_lstadd_front` - Add node to beginning
- `ft_lstadd_back` - Add node to end
- `ft_lstsize` - Count nodes
- `ft_lstlast` - Return last node
- `ft_lstdelone` - Delete a node
- `ft_lstclear` - Delete entire list
- `ft_lstiter` - Iterate the list
- `ft_lstmap` - Map function to list

## Project Structure

```
libft/
├── Makefile
├── libft.h
├── ft_*.c          (function implementations)
├── ft_lst*.c    (bonus functions)
└── README.md
```

## Technical Features

- **Language**: C
- **Compilation Flags**: `-Wall -Werror -Wextra` 
- **Norm**: Fully compliant with the 42 Norm guidelines
- **Global Variables**: Strictly forbidden (all functions are static or self-contained).

## Resources

### Official Documentation
- [C Standard Library Reference](https://en.cppreference.com/w/c) - Reference manual for types and standard definitions.

### AI Usage Disclosure
In strict alignment with the 42 curriculum framework regarding artificial intelligence:
- **Tasks Performed**: AI was utilized exclusively as an educational resource to clarify technical lower-level definitions, trace pointer status during nested matrix cleanups (such as allocation failure handling inside `ft_split`), and understand compiler warning logic.
- **Boundaries**: No automated code generation or direct shortcuts were used to replace individual logical reasoning or manual code implementation.

---

**Author**: [mcalleja]

