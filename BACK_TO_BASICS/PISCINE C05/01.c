#include <stdio.h>
#include <limits.h>

int ft_recursive_factorial(int nb)
{
    int result;
    if (nb < 0)
        return 0;
    if (nb == 0)
        return 1;
    result = nb * ft_recursive_factorial(nb - 1);
    return result;
}

int main()
{
    printf("%d\n", ft_recursive_factorial(6));
}