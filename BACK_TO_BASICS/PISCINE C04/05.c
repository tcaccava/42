#include <string.h>
#include <stdio.h>

int is_in_base(char *base, char c)
{
    int i = 0;
    while (base[i])
    {
        if (base[i] == c)
            return i;
        i++;
    }
    return -1;
}

int is_base_valid(char *base)
{
    if (!base || !base[1])
        return 0;
    int i = 0;
    while (base[i])
    {
        if (base[i] == 32 || (base[i] >= 9 && base[i] <= 13) || base[i] == '-' || base[i] == '+')
            return 0;
        for (int j = i + 1; j < strlen(base); j++)
        {
            if (base[i] == base[j])
                return 0;
        }
        i++;
    }
    return 1;
}

int ft_atoi_base(char *str, char *base)
{
    int base_len = strlen(base);
    int result = 0;
    int sign = 1;
    if (!is_base_valid(base))
        return 0;
    while (*str == 32 || (*str >= 9 && *str <= 13))
        str++;
    while (*str == '+' || *str == '-')
    {
        if (*str = '-')
            sign = -sign;
        str++;
    }
    while (*str)
    {
        int val = is_in_base(base, *str);
        if (val == -1)
            break;
        result = (result * base_len) + val;
    }
    return result * sign;
}


