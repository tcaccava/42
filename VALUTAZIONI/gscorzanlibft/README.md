*This project has been created as part of the 42 curriculum by gscorzon*

# Description

'libft' is a library recreating a number of C functions, along with some additional ones. Writing it serves as an excercise in low-level C programming as well as allowing students to build a library they will use throughout their studies.

# Instructions

To create the archive, run `make all`. To delete all the `.o` files, run `make clean`. To delete all files including `libft.a`, run `make fclean`.

In order to use the library and test its function, write `#include "libft.h"' at the beginning of your C file and compile it with `cc -Wall -Wextra -Werror \<filename\>.c libft.a`.

# Resources

- Wikipedia
	- https://en.wikipedia.org/wiki/C_data_types
	- https://en.wikipedia.org/wiki/File_descriptor
	- https://en.wikipedia.org/wiki/Include_directive
	- https://en.wikipedia.org/wiki/Linked_list
	- https://en.wikipedia.org/wiki/Make_(software)
	- https://en.wikipedia.org/wiki/Memory_address
- Stackoverflow
	- https://stackoverflow.com/questions/1073157/zero-size-malloc
	- https://stackoverflow.com/questions/17298172/how-does-sizeof-work-for-char-pointer-variables
	- https://stackoverflow.com/questions/21387972/bad-permissions-for-mapped-region
	- https://stackoverflow.com/questions/2186246/what-is-o-file
	- https://stackoverflow.com/questions/2577068/what-is-symbol-table-and-how-is-it-integrated-into-the-executable
	- https://stackoverflow.com/questions/29800636/calloc-and-null
	- https://stackoverflow.com/questions/33821625/incomplete-type-void-is-not-assignable-c
	- https://stackoverflow.com/questions/5329916/why-are-memory-addresses-are-represented-using-hexadecimal-numbers
	- https://stackoverflow.com/questions/67059403/integer-overflow-while-malloc-memory
	- https://stackoverflow.com/questions/68970147/why-does-char-occupy-7-bits-when-the-length-is-1-byte-ie-8-bits
	- https://stackoverflow.com/questions/7081288/assign-values-to-a-dereferenced-void-pointer
	- https://stackoverflow.com/questions/72143401/memory-management-and-overflow-in-c
	- https://stackoverflow.com/questions/75035339/how-to-access-the-content-of-a-void-pointer-inside-a-struct-pointer
	- https://stackoverflow.com/questions/75191/what-is-an-unsigned-char
	- https://stackoverflow.com/questions/76052480/building-a-static-library-in-c-using-the-ar-cmd
	- https://stackoverflow.com/questions/7828393/c-programming-casting-a-void-pointer-to-an-int
	- https://stackoverflow.com/questions/8208021/how-to-increment-a-pointer-address-and-pointers-value
- GeeksforGeeks
	- https://www.geeksforgeeks.org/c/c-typecasting/
	- https://www.geeksforgeeks.org/c/dynamically-allocate-2d-array-c/
	- https://www.geeksforgeeks.org/c/header-files-in-c-cpp-and-its-uses/
	- https://www.geeksforgeeks.org/c/how-do-i-create-a-library-in-c/
	- https://www.geeksforgeeks.org/c/memset-c-example/
	- https://www.geeksforgeeks.org/c/pointer-arithmetics-in-c-with-examples/
	- https://www.geeksforgeeks.org/c/unsigned-char-in-c-with-examples/
	- https://www.geeksforgeeks.org/c/void-pointer-c-cpp/
	- https://www.geeksforgeeks.org/cpp/array-decay-in-c/
	- https://www.geeksforgeeks.org/dsa/insert-a-node-at-front-beginning-of-a-linked-list/
	- https://www.geeksforgeeks.org/dsa/stack-vs-heap-memory-allocation/
	- https://www.geeksforgeeks.org/linux-unix/ar-command-in-linux-with-examples/
- Man pages for reading about the original functions' behavior

### AI

LLMs such as Claude and Gemini were used in order to troubleshoot some of the trickiest problems, especially regarding memory issues that came up during the latest stages of testing. Gemini was also used to shorten the ft_arrfill function in ft_split.
