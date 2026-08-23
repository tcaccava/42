#include <stdio.h>

char *ft_strcpy(char *dst, char *src);
int ft_strlen(char *str);
int ft_strcmp(char *s1, char *s2);
__ssize_t ft_write(int fd, const void* buffer, size_t count);

int main()
{
    // char src[] = "tobia";
    // char dst[6];
    // ft_strcpy(dst, src);
    // char cmp[] = "tobaa";
    // printf("%d\n", ft_strcmp(src, cmp));
    // printf("%d\n", ft_strlen(src));
    // printf("%s\n", dst);
    ft_write(1, "tobia", 5);
}