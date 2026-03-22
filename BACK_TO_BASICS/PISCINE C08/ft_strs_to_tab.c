#include "ft_stock_str.h"

struct s_stock_str *ft_strs_to_tab(int ac, char **av)
{
    t_stock_str *array = malloc(sizeof(t_stock_str) * (ac + 1));
    if (!array)
        return NULL;
    int i = 0;
    while (i < ac)
    {
        array[i].size = strlen(av[i]);
        array[i].str = av[i];
        array[i].copy = malloc((array[i].size) + 1);
        if (!array[i].copy)
            return NULL;
        strcpy(array[i].copy, av[i]);
        i++;
    }
    array[i].str = 0;
    return array;
}