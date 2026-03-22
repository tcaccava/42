#include <stdio.h>

int ft_strlen(char *s) {
    char *p = s;
    while(*p++);
    return --p - s;
}

//oppure ancora piu' elegante ma limitata dalla dimensione della stack
int ft_strlen2(char *s) {
    return *s ? ft_strlen2(s + 1) + 1 : 0; 
}

int main() {
    printf("%d\n", ft_strlen("ciao"));
    printf("%d\n", ft_strlen2("ciao"));
}