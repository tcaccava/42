#include <unistd.h>

void ft_print_reverse_alphabet()
{
    char c = 122; // z
    while (c >= 97)
    {
        write(1, &c, 1);
        c--;
    }
}

int main()
{
    ft_print_reverse_alphabet();
}