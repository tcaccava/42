#include <stdio.h>

int is_vowel(char c)
{
    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
        return 1;
    return 0;
}

int vowel_count(char *s, int index)
{
    if (index == 0)
        return (is_vowel(s[0]));
    return is_vowel(s[index]) + vowel_count(s, index - 1);
}

int main()
{
    char str[] = "ciaone";
    printf("%d\n", vowel_count(str, 5));
}