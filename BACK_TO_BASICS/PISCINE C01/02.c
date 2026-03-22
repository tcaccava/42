#include <stdio.h>

void ft_swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int main()
{
    int a = 2;
    int b = 7;
    ft_swap(&a, &b);
    printf("a is: %d\n", a); // 7
    printf("b is: %d\n", b); // 2
}