#include <stdio.h>
#include <unistd.h>
void ft_putnbr(int nb)
{
    char c;

    if (nb == -2147483648)
    {
        write(1, "-2147483648", 11);
        return;
    }

    if (nb < 0) // se il numero è negativo
    {
        write(1, "-", 1);
        nb = -nb;
    }
    if (nb >= 10)
        ft_putnbr(nb / 10);
    c = (nb % 10) + '0';
    write(1, &c, 1);
}

void front_count(int n)
{
    if (n < 0)
        return;
    if (n == 0)
    {
        ft_putnbr(0);
        write(1, " ", 1);
        return;
    }
    front_count(n - 1); // prima chiamo prima il numero precedente
    ft_putnbr(n);       // poi stampo questo
    write(1, " ", 1);
}

int main()
{
    front_count(5);
}