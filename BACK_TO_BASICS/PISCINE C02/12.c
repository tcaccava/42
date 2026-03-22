#include <unistd.h>
#include <stdio.h>

#include <unistd.h>

void ft_putchar(char c)
{
    write(1, &c, 1);
}

void print_hex_byte(unsigned char c)
{
    char *hex = "0123456789abcdef";
    ft_putchar(hex[c / 16]);
    ft_putchar(hex[c % 16]);
}

void print_address(void *addr)
{
    unsigned long value = (unsigned long)addr;
    char hex[16];
    int i;

    for (i = 15; i >= 0; i--)
    {
        hex[i] = "0123456789abcdef"[value % 16];
        value /= 16;
    }
    for (i = 0; i < 16; i++)
        ft_putchar(hex[i]);
    ft_putchar(':');
    ft_putchar(' ');
}

void print_hex_content(unsigned char *p, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < 16; i++)
    {
        if (i < len)
            print_hex_byte(p[i]);
        else
            write(1, "  ", 2);

        if (i % 2)
            ft_putchar(' ');
    }
}

void print_chars(unsigned char *p, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; i++)
    {
        if (p[i] >= 32 && p[i] <= 126)
            ft_putchar(p[i]);
        else
            ft_putchar('.');
    }
}

void *ft_print_memory(void *addr, unsigned int size)
{
    unsigned int i;
    unsigned char *p = (unsigned char *)addr;

    for (i = 0; i < size; i += 16)
    {
        unsigned int line_size = (size - i < 16) ? size - i : 16;

        print_address(p + i);
        print_hex_content(p + i, line_size);
        print_chars(p + i, line_size);
        ft_putchar('\n');
    }

    return addr;
}

int main(void)
{
    char *s = "Hello\nHow are you? This is a test.\n";
    ft_print_memory(s, 48);
    return 0;
}
