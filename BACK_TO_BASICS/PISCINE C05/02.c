#include <stdio.h>
#include <limits.h>

int ft_iterative_power(int nb, int power)
{
    int result = 1;
    if (power < 0)
        return 0;
    if (power == 0)
        return 1;
    while (power > 0)
    {
        if (nb != 0 && result > INT_MAX / nb)
            return 0;
        result = result * nb;
        power--;
    }
    return result;
}

int main()
{
    printf("%d\n", ft_iterative_power(2, 7));
}