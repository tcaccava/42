#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int is_in_base(char c, char *base)
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

int is_valid_base(char *base)
{
    int i;
    int j;

    if (!base || !base[1])
        return 0;

    i = 0;
    while (base[i])
    {
        if (base[i] == '+' || base[i] == '-' ||
            base[i] == ' ' || (base[i] >= 9 && base[i] <= 13))
            return 0;
        j = i + 1;
        while (base[j])
        {
            if (base[i] == base[j])
                return 0;
            j++;
        }
        i++;
    }
    return 1;
}

int ft_atoi_base(char *str, char *base)
{
    int result = 0;
    int sign = 1;
    int base_len = strlen(base);
    while (*str == 32 || (*str >= 9 && *str <= 13))
        str++;
    while (*str == '+' || *str == '-')
    {
        if (*str == '-')
            sign = -sign;
        str++;
    }
    while (*str)
    {
        int val = is_in_base(*str, base);
        if (val == -1)
            break;
        result = (result * base_len) + val;
        str++;
    }
    return result * sign;
}

char *ft_itoa_base(int num, char *base)
{
    long n = num;
    int base_len = strlen(base);
    char buffer[65536];
    int i = 0;
    if (num < 0)
        n = -n;
    if (n == 0)
        buffer[i++] = base[0];
    while (n)
    {
        buffer[i++] = base[n % base_len];
        n = n / base_len;
    }
    if (num < 0)
        buffer[i++] = '-';
    char *res = malloc(sizeof(char) * (i + 1));
    if (!res)
        return NULL;
    int len = i;
    res[len] = '\0';
    int j = 0;
    while (i--)
        res[j++] = buffer[i];
    return res;
}

char *ft_convert_base(char *nbr, char *base_from, char *base_to)
{
    if (!is_valid_base(base_from) || !is_valid_base(base_to))
        return NULL;
    int atoied = ft_atoi_base(nbr, base_from);
    return ft_itoa_base(atoied, base_to);
}

int main()
{
    printf("%s\n", ft_convert_base("10000", "01", "0123456789"));             // 16
    printf("%s\n", ft_convert_base("10000", "0123456789", "01"));             // 10011100010000
    printf("%s\n", ft_convert_base("255", "0123456789", "0123456789abcdef")); // ff
}
