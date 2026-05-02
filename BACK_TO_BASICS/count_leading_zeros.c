#include <stdio.h>
// conta gli zeri consecutivi che ci sono partendo dal bit alto
unsigned char count_leading_zeros(unsigned char n)
{
    int i = 7;
    for (; !(n >> i & 1) && i >= 0; i--);
    return 7 - i;
}

// int main()
// {
//     printf("%d\n",count_leading_zeros(0b00001001));
//     printf("%d\n",count_leading_zeros(0b10000000));
//     printf("%d\n",count_leading_zeros(0b00000000));
//     printf("%d\n",count_leading_zeros(0b00000001));
// }