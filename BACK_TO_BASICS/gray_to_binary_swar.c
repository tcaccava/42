#include <stdio.h>

unsigned int gray_to_binary_swar(__uint64_t n)
{
    n ^= n >> 32;
    n ^= n >> 16;
    n ^= n >> 8;
    n ^= n >> 4;
    n ^= n >> 2;
    n ^= n >> 1;
    return (unsigned int)n;
}