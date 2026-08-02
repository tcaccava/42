*This project has been created as part of the 42 curriculum by <dfelicia>.*

/*-----------------------------------------------------------------------------+
 |                                                                             |
 |  get_next_line                                                              |
 |                                                                             |
 +-----------------------------------------------------------------------------*/

# ┌─────────────────────────────┐
# │ Description                 │
# └─────────────────────────────┘

`get_next_line` is a C function that reads a text file (or any file descriptor,
including standard input) line by line, one call at a time, regardless of the size
of the file or of the internal read buffer.

The goal of the project is to implement a function with the following prototype:

​```c
char *get_next_line(int fd);
​```

Each call to `get_next_line(fd)` returns the next line of the file pointed to by
`fd`, including the trailing `\n` when present, or `NULL` when there is nothing
left to read (end of file or an error).

# ┌─────────────────────────────┐
# │ Structure and subdivision   │
# └─────────────────────────────┘

The project is split into three files. get_next_line.c contains the main function, which orchestrates every call: filling the buffer, extracting the line, and updating what's left for next time.

  *get_next_line*
├── get_next_line.c         Main functions:
│   ├── get_next_line()     - get_next_line(): entry point, calls the others
│   ├── leftovermkr()       - leftovermkr(): reads BUFFER_SIZE bytes
│   ├── newline()           - newline(): extracts the line to return
│   ├── pointer()           - pointer(): rebuilds the leftover for next call
│   └── freebuffer()        - freebuffer(): frees the read buffer safely
│
├── get_next_line_utils.c   Helper functions used internally by get_next_line.c
│   ├── ft_strlen()          - string length (NULL-safe)
│   ├── ft_strchr()          - checks whether '\n' is present in a string
│   └── ft_strjoin()         - concatenates leftover + newly read buffer
│
└── get_next_line.h         Function prototypes and the default BUFFER_SIZE

# ┌─────────────────────────────┐
# │ Instructions                │
# └─────────────────────────────┘

### Compilation

The function is meant to be compiled together with the file(s) that use it.
`BUFFER_SIZE` can (and should) be overridden at compile time to test the
function's robustness with different buffer sizes:

​```bash
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 \
    your_main.c get_next_line.c get_next_line_utils.c \
​```

If `BUFFER_SIZE` is not defined on the command line, it defaults to `42`
(see `get_next_line.h`).

### Usage example

​```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
	int   fd;
	char *line;

	fd = open("some_file.txt", O_RDONLY);
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}
​```

### Testing / debugging

While developing, it is strongly recommended to:

- Compile with `-Wall -Wextra -Werror` and fix every warning.
- Test with several `BUFFER_SIZE` values (`1`, `9999`, file size, etc.).
- Test files with empty lines, no trailing newline, and empty files.
- Check for leaks and invalid memory access with:

​```bash
valgrind --leak-check=full --show-leak-kinds=all
​```

# ┌─────────────────────────────┐
# │ Resources                   │
# └─────────────────────────────┘

- [en.subject.pdf] *get_next_line* (project instructions provided by 42)
- `man 2 read`, `man 3 malloc`, `man 3 free` — C standard library documentation
- [C static variables] (https://en.cppreference.com/w/c/language/storage_duration)
    *— persistence of `static` local variables between function calls.*
- [Valgrind Quick Start Guide] (https://valgrind.org/docs/manual/quick-start.html)
    *— for leak and invalid-access detection.*
- 42 Norm — coding style constraints applied throughout the project

 --------------------------------------------------------
  
### AI Usage
    AI assistant Claude (Anthropic) was used as a tool throughout the project —

    It was used to:
    - Compile the code with several `BUFFER_SIZE` values and run it under
  **Valgrind** and **gdb** to reproduce and pinpoint two bugs:
  1. An out-of-bounds read/write in `newline()` caused by a copy loop with
     no lower bound on its index, leading to heap corruption and a
     segmentation fault.
  2. A logic bug in `pointer()` that re-included the already-consumed `\n`
     character in the new `leftover`, producing a spurious empty line
     before every line following the first one.
- Explain the root cause of each bug and confirm the fix produced clean
  Valgrind output (`0 errors`, all allocations freed) across normal files,
  files with empty lines, and files without a trailing newline.
- Draft this `README.md`.

No AI-generated code was used for the core `get_next_line` logic itself;
all fixes were minimal, targeted one-line/two-line corrections to the
existing implementation, reviewed and validated by the author.
