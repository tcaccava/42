#include <stdio.h>

int ft_is_prime(int nb)
{
    if (nb < 2)
        return 0;
    if (nb == 2)
        return 1;
    if (nb % 2 == 0)
        return 0;
    int i = 3;
    while (i * i <= nb)
    {
        if (nb % i == 0)
            return 0;
        i += 2;
    }
    return 1;
}

int ft_find_next_prime(int nb)
{
    if (nb < 2)
        return 2;
    while(!ft_is_prime(nb))
        nb++;
    return nb;
}

int main()
{
    printf("%d\n", ft_find_next_prime(47));
    printf("%d\n", ft_find_next_prime(49));
}