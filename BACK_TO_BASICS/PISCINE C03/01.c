#include <unistd.h>
#include <stdio.h>

int ft_strncmp(char *s1, char *s2, unsigned int n)
{
    if (n == 0)
        return 0;
    int i = 0;
    while (i < n && s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    if (i == n) // tutti i primi n caratteri sono uguali
        return 0;
    return (unsigned char)s1[i] - (unsigned char)s2[i];
}

int main()
{
    char *s1 = "stocazzo";
    char *s2 = "stocazzone";
    char *s3 = "stocazzonz";
    printf("%d\n", ft_strncmp(s1, s2, 7));  // 0
    printf("%d\n", ft_strncmp(s3, s2, 10)); // z - e, 122 - 101 = 21
}