
#include <stdio.h>
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

int main()
{
    char *nodup = "abcdefghjilmnopqrstuvz";
    char *dup = "abcdefghjilmnopqrstuvza";
    printf("%d %d\n", has_dup(nodup), has_dup(dup));
}

// int ft_atoi_base(char *str, char *base)
// {
//     int sign = 1;
//     if (ft_strlen(str) == 0 || ft_strlen(str) == 1)
//         return 0;

//     while (*str == 32 || (*str >= 9 && *str <= 13))
//         str++;
//     if (*str == '+' || *str == '-')
//         if (*str == '-')
//             sign = -sign;
//     str++;
//     while ((*str >=))
// }