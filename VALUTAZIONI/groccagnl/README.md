*This project has been created as part of the 42 curriculum by grocca.*

## Description

`get_next_line` is a C function that reads a file descriptor line by line. Each call returns the next line from the file, including the terminating `\n` character (except at end of file if no `\n` is present). It returns `NULL` when there is nothing left to read or an error occurs.

The bonus version supports multiple file descriptors simultaneously using a single static variable, implemented via a circular linked list that stores the reading state for each open file descriptor.

## Instructions

### Compilation

Without bonus:
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c
```

With bonus:
```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c
```

The `-D BUFFER_SIZE=n` flag is required. If omitted, a default value of 42 is used. You can adjust `n` to any positive integer.

### Usage

```c
#include "get_next_line.h"

int fd = open("file.txt", O_RDONLY);
char *line;

while ((line = get_next_line(fd)) != NULL)
{
    printf("%s", line);
    free(line);
}
close(fd);
```

## Algorithm

The function uses a static buffer (`buf`) of size `BUFFER_SIZE + 1` per file descriptor to store data read in excess between calls.

On each call:
1. If the buffer is empty, `read()` is called to fill it.
2. The buffer is scanned for `\n`. If not found, `read()` is called again in a loop, accumulating all data in a dynamically allocated string `s`.
3. Once `\n` is found (or EOF is reached), the line is extracted from `s` using `substruntil()`.
4. The buffer is shifted with `strshift()` to preserve any data after the `\n` for the next call.

This approach minimises reads: as soon as a `\n` is found, reading stops and the current line is returned immediately.

For the bonus, a circular linked list stores one buffer per file descriptor, keyed by `fd`. The single static variable points to this list, satisfying the "only one static variable" requirement while supporting multiple file descriptors.

## Resources

- `man 2 read` — read() system call documentation
- `man 3 malloc` / `man 3 free` — memory management
- [Static variables in C — GeeksForGeeks](https://www.geeksforgeeks.org/static-variables-in-c/)
- [File descriptors explained](https://www.bottomupcs.com/file_descriptors.xhtml)

### AI usage

Claude (claude.ai) was used during this project as a learning support tool — not to generate code directly, but to ask conceptual questions and get feedback on code already written. Specifically:
- To understand how `read()` returns values at EOF
- To understand how static variables persist between function calls
- To get feedback on logical errors in functions already written by hand
- To discuss the circular linked list approach for the bonus

All code was written and reasoned through independently before and after consulting the AI.
