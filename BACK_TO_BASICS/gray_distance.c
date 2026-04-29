#include <stdio.h>
#include "./gray_code.c"
#include "./count_diff_bits.c"
unsigned int ft_gray_distance(unsigned int a, unsigned int b)
{
    unsigned int a_gray = a ^ (a >> 1);
    unsigned int b_gray = b ^ (b >> 1);
    return count_diff_bits(a_gray, b_gray);
}

int main()
{
    printf("%d\n", ft_gray_distance(3, 5));
}