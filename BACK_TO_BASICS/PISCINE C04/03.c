#include <stdio.h>
#include <stdlib.h>

int ft_atoi(char *str)
{
    int result = 0;
    int sign = 1;
    while (*str)
    {
        while ((*str >= 9 && *str <= 13) || (*str == 32))
            str++;
        while (*str == '-' || *str == '+')
        {
            if (*str == '-')
                sign = -sign;
            str++;
        }
        while (*str >= 48 && *str <= 57)
        {
            result = result * 10 + (*str - 48);
            str++;
        }
        return result * sign;
    }
}

int main() {
    printf("%d\n", ft_atoi(" ---+--+1234ab567"));
}