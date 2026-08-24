 *This project has been created as part of the 42 curriculum by mbrunial*

## Description

`ft_printf` is a 42 school project where you need to recreate the behaviour of the function `printf`.

The goals are to learn the behaviour of this standard function, how data are flushed into the standard output, and how variable number of argument in a function works (variadic arguments).

Those were the mandatory format specifier were:

- `%c`
- `%s`
- `%p`
- `%d`
- `%i`
- `%u`
- `%x`
- `%X`
- `%%`

### Algorithm Description

In `printf` one argument is a string and a variable argument can be passed as second parameters.
Since each argument and its type are flagged inside a string with `%` followed by different argument.

My algorithm simply read the string, writing (`write`) each character on the std output until `%` is read. If so, it will read the next character and decide which operation to perform.

For string the length of the string is read, then the whole string is printed (`write(1, str, str_len)`).

For char and `%` simply write the char or the `%` symbol.

Since integer, hexadecimal, and pointer are represented as 8 byte data type, `putnbr base` is sufficient for each option (for pointer `"0x"` is printed).
For unsigned integer a `putnbr_base_uns` (unsigned) is used in a recursive function, analogous the same is done for signed numbers.

The `NULL` pointer behave as in the standard C of `printf`; in my case it's undefined behaviour.

## Instruction

This function can be compiled in other projects using:

- `ft_printf.c`
- `ft_printf.h`

## Resources

Web forums were used to understand the concepts of file descriptors and static variables.

YouTube videos about the topics above were also consulted to understand the key concepts of the project.

AI was used as a learning tool during this project:

- Making the `README.md` visually clearer and better organized.
- Explaining unclear programming concepts.
- Improving explanations and documentation.
