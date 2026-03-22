#include <unistd.h>

void ft_print_alphabet()
{
    char c = 97; // a
    while (c <= 122)
    {
        write(1, &c, 1);
        c++;
    }
}

int main()
{
    ft_print_alphabet();
}