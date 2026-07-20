#include <stdio.h>

int ft_strcmp(const char *str1, const char *str2)
{
    while(*str1 == *str2 && *str1){
        str1++;str2++;
    }
    return (unsigned char)*str1 - (unsigned char)*str2;
}

int main() {
    printf("%d\n", ft_strcmp("tobiac", "tobiaa"));
}