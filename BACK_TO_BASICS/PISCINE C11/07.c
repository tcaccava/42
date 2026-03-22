#include <stdio.h>

int ft_strcmp(char *s1, char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

void swap(char **s1, char **s2)
{
    char *tmp = *s1;
    *s1 = *s2;
    *s2 = tmp;
}

void ft_advanced_sort_string_tab(char **tab, int (*cmp)(char *, char *))
{
    char **begin = tab;
    while (*tab)
    {

        if (*(tab + 1) && (cmp)(*tab, *(tab + 1)) > 0)
        {
            swap(tab, tab + 1);
            tab = begin;
            continue;
        }
        tab++;
    }
}

int main()
{
    char *array[] = {"Bismuto", "Cadmio",
                     "Andonio",
                     "Dario", NULL};
    int (*cmp)(char *,char *) = ft_strcmp;
    ft_advanced_sort_string_tab(array,cmp);//ma potevo fare anche direttamente ft_advanced_sort_string_tab(array,ft_strcmp)
    int i = 0;
    while (array[i])
        printf("%s\n", array[i++]);
}