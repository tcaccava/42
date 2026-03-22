#include <stdio.h>

void ft_sort_int_tab(int *tab, int size)
{
    int i = 0;
    while (i < size - 1)
    {
        if (tab[i] > tab[i + 1])
        {
            int tmp = tab[i + 1];
            tab[i + 1] = tab[i];
            tab[i] = tmp;
            i = 0;
            continue;
        }
        i++;
    }
}

int main() {
    int array[] = {3,2,4,6,5,1,7};
    ft_sort_int_tab(array, 7);
    for(int i = 0; i < 7 ; i++)
        printf("%d\n", array[i]);
}