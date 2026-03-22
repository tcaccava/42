#include <unistd.h>

void ft_putnbr(int nb)
{
    char c;

    if (nb == -2147483648)
    {
        write(1, "-2147483648", 11);
        return;
    }

    if (nb < 0)
    {
        write(1, "-", 1);
        nb = -nb;
    }
    if (nb >= 10)
        ft_putnbr(nb / 10);
    c = (nb % 10) + '0';
    write(1, &c, 1);
}

void alt_print(int *a, int left, int right)
{
    if (left > right)
        return;
    ft_putnbr(a[left]);
    if (left != right)
        ft_putnbr(a[right]);
    alt_print(a, left + 1, right - 1);
}

int main()
{
    int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    alt_print(a, 0, 9);
}