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

int main(int c, char **v)
{
    ft_putstr(v[0]);
}
