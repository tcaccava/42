#include <unistd.h>

void printbits(unsigned char n)
{
    for (int i = 7; i >= 0; i--)
    {
        unsigned char c = (n >> i & 1) + 48;
        write(1, &c, 1);
    }
    write(1, "\n", 1);
}