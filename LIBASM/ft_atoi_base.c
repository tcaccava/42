
#include <stdio.h>

size_t ft_strlen(char *s)
{
    return !*s ? 0 : 1 + ft_strlen(s + 1);
}

int has_dup(char *str)
{
    int i = 0;
    while (str[i])
    {
        int j = i + 1;
        while (str[j])
        {
            if (str[i] == str[j++])
                return 1;
        }
        i++;
    }
    return 0;
}

int has_forbid_char(char *s)
{
    char *forbidden = " +-\t\n\v\f\r";
    while (*s)
    {
        int i = 0;
        while (forbidden[i])
        {
            if (*s == forbidden[i])
                return 1;
            i++;
        }
        s++;
    }
    return 0;
}

int invalid_length(char *s)
{
    return (ft_strlen(s) == 0 || ft_strlen(s) == 1);
}

int is_invalid_base(char *base)
{
    return ((invalid_length(base)) || (has_forbid_char(base)) || has_dup(base));
}

int is_char_in_base(char c, char *base)
{
    while (*base)
    {
        if (c == *base)
            return 1;
        base++;
    }
    return 0;
}

int return_base_value(char *base, char c)
{
    char *begin = base;
    while (*base)
    {
        if (c == *base)
            return base - begin;
        base++;
    }
    return 0;
}

int ft_atoi_base(char *str,char *base)
{
    int sign = 1;
    int res = 0;
    if (is_invalid_base(base))
        return 0;
    size_t base_len = ft_strlen(base);
    while (*str == 32 || (*str >= 9 && *str <= 13))
        str++;
    if (*str == '+' || *str == '-')
    {
        if (*str == '-')
            sign = -sign;
        str++;
    }
    int digit = 0;
    while ((digit = return_base_value(base, *str)) >= 0)
    {
        res = (res * base_len) + digit;
        str++;
    }
    return res * sign;
}

int main()
{
    // char *nodup = "abcdefghjilmnopqrstuvz";
    // char *dup = "abcdefghjilmnopqrstuvza";
    // char *forbid = "abcdefghjilmnop qrstuvza";
    // char *empty = "";
    // char *one_char = "a";
    // printf("%d\n", return_base_value(nodup, 'f'));
    // printf("%d %d\n", is_char_in_base('e', nodup), is_char_in_base('8', nodup));
    // printf("%d %d %d %d %d\n", is_invalid_base(nodup), is_invalid_base(dup), is_invalid_base(forbid), is_invalid_base(empty), is_invalid_base(one_char));
    // printf("%d %d\n", invalid_length(dup), invalid_length(one_char));
    // printf("%d %d\n", has_dup(nodup), has_dup(dup));
    // printf("%d %d\n", has_forbid_char(nodup), has_forbid_char(forbid));
    char *base = "abcd";
    printf("%d\n", ft_atoi_base("  -bc", base));
}
