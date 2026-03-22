#include <stdio.h>

int ft_is_sort(int *tab, int length, int (*f)(int, int))
{
    int i;
    int asc = 1;
    int desc = 1;

    i = 0;
    while (i < length - 1)
    {
        if (f(tab[i], tab[i + 1]) > 0)
            asc = 0;
        if (f(tab[i], tab[i + 1]) < 0)
            desc = 0;
        i++;
    }
    return (asc || desc);
}

