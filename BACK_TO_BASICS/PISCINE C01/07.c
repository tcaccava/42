#include <stdio.h>

void ft_rev_int_tab(int *tab, int size)
{
    int i = 0;
    while (i < size / 2)
    {
        int tmp = tab[i];
        tab[i] = tab[size - i - 1];
        tab[size - i - 1] = tmp;
        i++;
    }
}

int main()
{
    int tab[] = {1, 2, 3, 4, 5, 6};
    ft_rev_int_tab(tab, 6);
    for (int i = 0; i < 6; i++)
        printf("%d\n", tab[i]);
}