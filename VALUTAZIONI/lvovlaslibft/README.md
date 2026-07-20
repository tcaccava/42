# Libft

Libft is the very first project of the 42 School curriculum. The objective is to re-code a set of standard C library functions, as well as additional utility functions, to create a personal development library (`libft.a`). This static library will be used and expanded upon in almost all future 42 C projects.

---

## Features
The project is divided into three distinct parts (everything is mandatory):
1. **Libc Functions:** Re-implementations of standard C library functions (`<ctype.h>`, `<string.h>`, `<stdlib.h>`, etc.).
2. **Additional Functions:** Custom utility functions for string manipulation, memory allocation, and file descriptor outputs.
3. **Linked list Functions:** A collection of utilities designed to handle and manipulate singly linked lists.

---

## Installation & Compilation

To build and compile the library, clone this repository and run `make`:

```bash
# Clone the repository
git clone git@example

# Navigate into the project folder
cd libft

# Compile mandatory functions
make
```

### Makefile Rules
* **`make`** or **`make all`**: Compiles the source files and generates the static library `libft.a`.
* **`make clean`**: Deletes all object files (`.o`).
* **`make fclean`**: Deletes all object files (`.o`) and the compiled library file (`libft.a`).
* **`make re`**: Executes a clean and completely rebuilds the library.

---

## Usage

To use this library in your own C programming projects, follow these steps:

1. **Include the header** in your C source files:
   ```c
   #include "libft.h"
   ```

2. **Compile your program** by linking the library path, file, and include directory:
   ```bash
   gcc main.c -L. -lft -I.
   ```
   * `-L.`: Instructs the compiler to search for libraries in the current directory.
   * `-lft`: Links your code against the compiled `libft.a` file.
   * `-I.`: Tells the compiler to look for the `libft.h` header in the current directory.

---

## Full List of Functions

### Part 1 - Libc Functions

| Function | Description |
| :--- | :--- |
| `ft_isalpha` | Checks for an alphabetic character. |
| `ft_isdigit` | Checks for a digit (0 through 9). |
| `ft_isalnum` | Checks for an alphanumeric character. |
| `ft_isascii` | Checks whether the character fits into the ASCII character set. |
| `ft_isprint` | Checks for any printable character. |
| `ft_strlen` | Calculates the length of a string. |
| `ft_memset` | Fills memory with a constant byte. |
| `ft_bzero` | Zeroes a byte string. |
| `ft_memcpy` | Copies memory area. |
| `ft_memmove` | Copies memory area safely even if regions overlap. |
| `ft_strlcpy` | Size-bounded string copying. |
| `ft_strlcat` | Size-bounded string concatenation. |
| `ft_toupper` | Converts a lowercase letter to uppercase. |
| `ft_tolower` | Converts an uppercase letter to lowercase. |
| `ft_strchr` | Locates the first occurrence of a character in a string. |
| `ft_strrchr` | Locates the last occurrence of a character in a string. |
| `ft_strncmp` | Compares two strings up to a specified number of bytes. |
| `ft_memchr` | Scans memory for a specific byte. |
| `ft_memcmp` | Compares two memory areas. |
| `ft_strnstr` | Locates a substring in a string, searching within a length limit. |
| `ft_atoi` | Converts a string to an integer. |
| `ft_calloc` | Allocates memory and initializes its bits to zero. |
| `ft_strdup` | Creates a duplicate of a string using `malloc`. |

### Part 2 - Additional Functions

| Function | Description |
| :--- | :--- |
| `ft_substr` | Allocates and returns a substring from a string. |
| `ft_strjoin` | Allocates and returns a new string, which is the result of concatenation. |
| `ft_strtrim` | Allocates and returns a copy of a string with specified characters removed from start and end. |
| `ft_split` | Allocates and returns an array of strings obtained by splitting a string using a delimiter character. |
| `ft_itoa` | Allocates and returns a string representing the integer received as an argument. |
| `ft_strmapi` | Applies a function to each character of a string to create a new string. |
| `ft_striteri` | Applies a function to each character of a string, passing its index as an argument. |
| `ft_putchar_fd` | Outputs a character to the given file descriptor. |
| `ft_putstr_fd` | Outputs a string to the given file descriptor. |
| `ft_putendl_fd` | Outputs a string to the given file descriptor, followed by a newline. |
| `ft_putnbr_fd` | Outputs an integer to the given file descriptor. |

### Part 3 - Linked List Functions

| Function | Description |
| :--- | :--- |
| `ft_lstnew` | Allocates and returns a new node element. |
| `ft_lstadd_front` | Adds a new node element at the beginning of a list. |
| `ft_lstsize` | Counts the number of elements in a list. |
| `ft_lstlast` | Returns the last node element of a list. |
| `ft_lstadd_back` | Adds a new node element at the end of a list. |
| `ft_lstdelone` | Takes a node element as a parameter and frees its content using a custom function. |
| `ft_lstclear` | Deletes and frees a given node element and every successor of that element. |
| `ft_lstiter` | Iterates a list and applies a custom function to the content of each node element. |
| `ft_lstmap` | Iterates a list and applies a custom function to create a new list resulting from the modifications. |

