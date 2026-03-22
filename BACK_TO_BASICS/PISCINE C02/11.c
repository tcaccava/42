#include <stdio.h>
#include <unistd.h>

int is_printable(char c)
{
    return (c >= 32 && c <= 126);
}

void ft_putchar(char c)
{
    write(1, &c, 1);
}

void print_hex(char c)
{
    char hex[2];
    char *digits = "0123456789abcdef";
    hex[0] = digits[c / 16];
    hex[1] = digits[c % 16];
    ft_putchar('\\');
    ft_putchar(hex[0]);
    ft_putchar(hex[1]);
}

void ft_putstr_non_printable(char *str)
{
    int i = 0;
    unsigned char c;
    while (str[i])
    {
        c = (unsigned char)str[i];
        if (is_printable(c))
            ft_putchar(c);
        else
            print_hex(c);
        i++;
    }
}

int main()
{
    char *str = "Hello\nHow are you?";
    ft_putstr_non_printable(str);
}