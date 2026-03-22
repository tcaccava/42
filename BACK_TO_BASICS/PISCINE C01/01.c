#include <stdio.h>

void ft_ultimate_ft(int *********nbr)
{
    *********nbr = 42;
}

int main()
{
    int n = 25;
    int *n1 = &n;
    int **n2 = &n1;
    int ***n3 = &n2;
    int ****n4 = &n3;
    int *****n5 = &n4;
    int ******n6 = &n5;
    int *******n7 = &n6;
    int ********n8 = &n7;
    ft_ultimate_ft(&n8);
    printf("%d\n", n); // stampa 42
}