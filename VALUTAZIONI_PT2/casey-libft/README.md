*This project has been created as part of the 42 curriculum by keiestre.*

# Libft

### Description of the project

Libft is a custom C library created from scratch as part of the 42 curriculum.

The goal of this project is to recreate a collection of basic and commonly used C standard library functions, as well as to implement additional utility functions that are useful when developing larger C projects.

The project focuses on strengthening fundamental C programming skills, including:

- Memory management
- String manipulation
- Character checking and conversion
- Linked lists
- Pointers
- Dynamic memory allocation
- Working with arrays and buffers
- Understanding and reproducing standard C library behaviour

The library is compiled into a static library called `libft.a`, which can then be included and reused in other C projects.

---

### Instructions

# Compilation

To compile the library, clone the repository and run:

```bash
make

This will compile the source files and create the libft.a static library


## Makefile commands
- make 
	This compiles the library
- make clean
	This removes the object files (*.o) generated during compilation
- make fclean
	This removes all object files and the compiled libft.a
- make re
	This removes the previous compilation and recompiles the library from scratch


### Usage

After compiling this project, libft.a can be linked and used in other C projects


### Resources

During the development of this project, I used different resources to understand the expected behaviour of the functions I have to recreate and the concepts involved.

The main resources I used were:
	- Linux man pages
	- Online C programming tutorials
	- Articles and examples related to memory management, pointer, and linked lists
	- AI tools

## AI Usage
AI tools were mainly used as a support for debugging and understanding problems I encountered during implementation.

---
### Detailed description of the LIBFT Library

Libft is designed to provide a collection of reusable functions that can be used as a foundation for future C projects.

The library includes implementations for common operations involving characters, strings, memory, numbers, and other data structures.

One of the main objectives of the project is to understand what happens internally when using functions that are normally provided by the C standard library.

For example, implementing memory-related functions requires understanding how memory is represented and accessed through pointers. String-related functions require working directly with null-terminated character arrays, while functions involving dynamic memory require careful allocation and deallocation to avoid memory leaks.

The project also introduces the use of a static library. Instead of compiling every source file again when using the functions in another project, the compiled object files can be grouped into libft.a and linked when needed.

This makes Libft reusable across different projects in the 42 curriculum.
