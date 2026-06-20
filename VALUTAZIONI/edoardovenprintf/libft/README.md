*This project has been created as part of the 42 curriculum by edveneto*

## Description
Libft is the first project of the 42 core curriculum. The goal is to create a custom C library (`libft.a`) that implements standard `libc` functions, as well as additional utility functions for string manipulation, memory management, and linked lists.

## Library Content
The library provides the following features:
* **Libc Functions :** Re-implementations of standard C functions (e.g., `ft_strlen`, `ft_memset`, `ft_atoi`, `ft_strchr`, `ft_calloc`, `ft_strdup`).
* **Additional Functions :** Custom utility functions for strings and file descriptors (e.g., `ft_split`, `ft_strjoin`, `ft_itoa`, `ft_putstr_fd`).
* **Linked Lists :** Functions to create, iterate, and manage custom linked list structures (`t_list`).

## Instructions
To compile and use the library, clone the repository and run the following commands via the provided Makefile:

* `make` - Compiles and generates the `libft.a` static library.
* `make clean` - Removes all object (`.o`) files.
* `make fclean` - Removes all object files and the `libft.a` binary.
* `make re` - Completely cleans and recompiles the library.

To use this library in your own projects, include the header `#include "libft.h"` in your source files and compile your program with the flags `-L. -lft`.

## Resources
* Linux Programmer's Manual (`man 3`).
* GNU Make Documentation.
* **AI Usage Statement:** No AI tools were used to generate the core logic of the functions in this repository, just for code revision and optimization of readme.
