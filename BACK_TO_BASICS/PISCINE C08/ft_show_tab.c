#include <unistd.h>
#include "ft_stock_str.h"

void ft_putstr(char *s)
{
    while (*s)
        write(1, s++, 1);
    write(1, "\n", 1);
}

void putnbr(int num)
{
    if (num >= 10)
        putnbr(num / 10);
    char c = num % 10 + '0';
    write(1, &c, 1);
    write(1, "\n", 1);
}

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

void ft_show_tab(struct s_stock_str *par)
{
    while (par->str) // -> dereferenzia par e accede al valore di str,per questo non c'e bisogno di
    // scrivere *par->str; par->str equivale a (*par).str
    {
        ft_putstr(par->str);
        putnbr(par->size);
        ft_putstr(par->copy);
        par++;
    }
}

int main(void)
{
    char *av[] = {"ciao", "come", "va"};
    int ac = 3;

    struct s_stock_str *tab = ft_strs_to_tab(ac, av);
    if (!tab)
        return 1;

    ft_show_tab(tab);

    for (int i = 0; i < ac; i++)
        free(tab[i].copy);
    free(tab);

    return 0;
}
