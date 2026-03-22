#include <unistd.h>

void ft_putstr(char *s)
{
    while (*s)
    {
        write(1, s, 1);
        s++;
    }
    write(1, "\n", 1);
}

int ft_strcmp(char *s1, char *s2)
{
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return ((unsigned char)*s1 - (unsigned char)*s2);
}

void ft_sort_params(int c, char **v)
{
    int i = 1;
    while (i < c - 1)
    {
        if (ft_strcmp(v[i], v[i + 1]) > 0)
        {
            char *tmp = v[i];
            v[i] = v[i + 1];
            v[i + 1] = tmp;
            i = 1;
            continue;
        }
        i++;
    }
}

void ft_print_params(int c, char **v)
{
    v++;
    while (*v)
        ft_putstr(*v++);
}

int main(int c, char **v)
{
    ft_sort_params(c, v);
    ft_print_params(c, v);
}