#include <stdio.h>

unsigned int ft_multiply_no_operator(unsigned int a, unsigned int b)
{
    unsigned int count_shift = 0;
    for (int i = 0; i <= 7; i++)
    {
        count_shift += (b >> i & 1) * i;
    }
    unsigned int res = a <<= count_shift;
    unsigned int factor = 1;
    while (count_shift--)
        factor *= 2;
    while (factor != b)
    {
        if (factor < b)
        {
            res += a;
            factor += 1;
        }
        else
        {
            res -= a;
            factor -= 1;
        }
    }
    return res;
}

int main()
{
    printf("%u %u %u %u\n", ft_multiply_no_operator(2, 5), ft_multiply_no_operator(10, 6), ft_multiply_no_operator(9, 3), ft_multiply_no_operator(8, 4));
}