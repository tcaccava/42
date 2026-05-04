#include <stdio.h>

int ft_count_leading_ones(unsigned char n) {
    int i = 0;
    for(; n << i & 128; i++);
    return i;
}

// int main()
// {
//     printf("%d\n",ft_count_leading_ones(0b00001001));
//     printf("%d\n",ft_count_leading_ones(0b10000000));
//     printf("%d\n",ft_count_leading_ones(0b11100000));
//     printf("%d\n",ft_count_leading_ones(0b11110001));
// }