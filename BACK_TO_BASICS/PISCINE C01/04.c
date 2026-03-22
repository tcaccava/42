#include <stdio.h>

void ft_ultimate_div_mod(int *a, int *b)
{
    int div = *a / *b;
    int mod = *a % *b;
    *a = div;
    *b = mod;
}

int main()
{
    int a = 9;
    int b = 2;
    ft_ultimate_div_mod(&a, &b);
    printf("a is: %d, b is: %d\n", a, b);
}