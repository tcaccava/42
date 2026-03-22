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

void ft_print_params(int c, char **v)
{
    int i = c - 1;
    while (i > 0)
        ft_putstr(v[i--]);
}

int main(int c, char **v)
{
    ft_print_params(c, v);
}