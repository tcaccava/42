#include <stdio.h>

void ft_ft(int *nbr)
{
    *nbr = 42;
}

int main()
{
    int a = 25;
    ft_ft(&a);
    printf("%d\n", a); // printa 42,non 25
}