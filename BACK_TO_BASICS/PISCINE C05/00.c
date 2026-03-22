#include <stdio.h>
#include <limits.h>

int ft_iterative_factorial(int nb)
{
    if (nb < 0)
        return 0;
    if (nb == 0)
        return 1;
    int result = 1;
    while (nb > 0)
    {
        if (result > INT_MAX / nb)
            return 0;
        result = result * nb;
        nb--;
    }
    return result;
}

int main()
{
    printf("%d\n", ft_iterative_factorial(6)); // 720
}