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

void swap(char **s1, char **s2) // se vuoi fare in modo di cambiare dove puntano le stringhe nella funzione principale devi passare un doppio **
{
    char *tmp = *s1;
    *s1 = *s2;
    *s2 = tmp;
}

// void swap(char *a, char *b)// versione sbagliata,scambia solo le copie locali
// {
//     char *tmp = a;
//     a = b;
//     b = tmp;
// }

void ft_sort_string_tab(char **tab)
{
    char **begin = tab;
    while (*tab)
    {
        if (*(tab + 1) && (ft_strcmp(*tab, *(tab + 1)) > 0))
        {
            swap(tab, (tab + 1));
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
    ft_sort_string_tab(array);
    int i = 0;
    while (array[i])
        printf("%s\n", array[i++]);
}