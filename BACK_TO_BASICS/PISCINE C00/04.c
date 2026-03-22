#include <unistd.h>

void ft_is_negative(int n)
{
    (n <= 0) ? write(1, "N\n", 2) : write(1, "P\n", 2); // operatore ternario
    // condizione ? se vero : se falso
}

int main()
{
    ft_is_negative(4);
    ft_is_negative(-4);
}