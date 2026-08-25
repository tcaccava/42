#include <stdio.h>
#include <stdlib.h>

char *ft_strcpy(char *dst, const char *src);
size_t ft_strlen(const char *s);
int ft_strcmp(const char *s1,const char *s2);
__ssize_t ft_write(int fd, const void* buf, size_t count);
__ssize_t ft_read(int fd, void *buf, size_t count);
char *ft_strdup(const char *s);
int ft_atoi_base(char *str, char *base);
int has_dup(char *str);
int has_forbid_char(char *s);

int main()
{
    char src[] = "tobia";
    char dup[] = "tobit";
    char forbid[] = "tobia ";
    // char dst[6];
    // ft_strcpy(dst, src);
    // char cmp[] = "tobaa";
    // printf("%d\n", ft_strcmp(src, cmp));
    // printf("%d\n", ft_strlen(src));
    // printf("%s\n", dst);
    //ft_write(1, "tobia", 5);
    // char *s = malloc(100);
    // __ssize_t bytes_read = ft_read(0, s, 100);
    // s[bytes_read] = 0;
    // printf("%s\nbytes read: %zd" , s, bytes_read);
    // char* dup = ft_strdup("tobia");
    // printf("%s\n", dup);
    printf("%d %d\n", has_dup(src), has_dup(dup));
    printf("%d %d\n", has_forbid_char(dup), has_forbid_char(forbid));
}