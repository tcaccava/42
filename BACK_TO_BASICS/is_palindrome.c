#include <stdio.h>
#include "./reverse_bits.c"
int is_palindrome_bits(unsigned char n)
{
    int i = 0;
    while (((n << i) & 128) == (((n >> i) & 1) << 7))
    {
        i++;
        if (i == 4)
            return 1;
    }
    return 0;
}

//oppure piu elegantemente 
// int is_palindrome_bits(unsigned char n) {
//     return n == reverse_bits(n);
// }

int main()
{
    printf("%d\n", is_palindrome_bits(0b10011001));
    printf("%d\n", is_palindrome_bits(0b11111111));
    printf("%d\n", is_palindrome_bits(0b11111110));
    printf("%d\n", is_palindrome_bits(0b11101111));
    printf("%d\n", is_palindrome_bits(0b01111111));
}