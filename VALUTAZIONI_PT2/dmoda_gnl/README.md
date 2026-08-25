_This project has been created as part of the 42 curriculum by dmoda._

# Description

This project implements the `get_next_line` function in C, a utility that reads a file descriptor line by line and returns one complete line at each call. The main goal is to provide an efficient way to process text streams without loading the entire file into memory.

The implementation uses a static buffer to preserve unread data between function calls, allowing it to handle partial reads and continue reading seamlessly until a newline character (`'\n'`) or the end of the file is reached. Memory is dynamically allocated for each returned line, ensuring that lines of any length can be processed.

Key features of this implementation include:

* Incremental reading using a fixed-size buffer (`BUFFER_SIZE`).
* Persistent state across function calls through static variables.
* Dynamic line construction character by character.
* Proper handling of end-of-file (EOF) conditions.
* Memory cleanup when reading is complete or an error occurs.

This project demonstrates fundamental concepts of systems programming in C, including file descriptor management, buffered I/O, dynamic memory allocation, and state persistence between function calls.

# Instructions

## Compilation

Compile the project using `cc` (or `gcc`) and include all required source files:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

Replace `42` with the desired buffer size if needed.

## Execution

Run the compiled program:

```bash
./gnl
```

If a test file is required, modify `main.c` to open the desired file descriptor or pass a file as an argument depending on your test setup.

## Usage

The `get_next_line()` function can be called repeatedly to read a file descriptor line by line:

```c
char *line;

while ((line = get_next_line(fd)) != NULL)
{
	printf("%s", line);
	free(line);
}
```

Each call returns:

* The next line from the file descriptor, including the newline character (`'\n'`) if present.
* `NULL` when the end of the file is reached or an error occurs.

## Notes

* `BUFFER_SIZE` must be greater than `0`.
* The returned string is dynamically allocated and must be freed by the caller.
* The implementation maintains internal state using static variables, allowing successive calls to continue reading from the same position in the file.
* This version supports reading from a single file descriptor at a time.

# Resources

## References

The following resources were consulted to understand the concepts used in this project:

### Documentation

* Peers and colleagues
* The Linux `read(2)` manual page — explains the behavior of the `read` system call.
* The Linux `malloc(3)` and `free(3)` manual pages — documentation for dynamic memory allocation.
* The Linux `open(2)` and `close(2)` manual pages — information about file descriptor management.
* The C Standard Library documentation.


### Tutorials and Articles

* Tutorials on file descriptors and buffered I/O in C.
* Articles covering dynamic memory management and string manipulation in C.
* Educational materials on static variables and persistent state across function calls.
* 42 School project documentation and subject guidelines for `get_next_line`.

## Use of Artificial Intelligence

Artificial Intelligence (AI) was used as a supporting tool during the development and documentation of this project.

### Tasks Assisted by AI

* Reviewing and improving code readability.
* Explaining the behavior of system calls and memory management functions.
* Assisting with the writing and organization of the README documentation.
* Suggesting clearer descriptions of the implementation and project structure.
* Verifying the correctness of explanations related to static buffers and line-by-line file reading.

### Scope of AI Usage

AI was not extensively used to generate the complete project solution. The implementation, debugging, testing, and validation of the `get_next_line` function were performed by me.

