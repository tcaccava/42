#include "ft_printf.h"
#include <stdio.h>

int main(void)
{
    ft_printf("%d\n", 424271726);
    printf("%d\n", 4765856);
    ft_printf("%c\n", 97);
    printf("%c\n", 97);
    ft_printf("%s\n", "ciao5887");
    printf("%s\n", "ciao65759");
    char *str = NULL;
    ft_printf("%s\n", str);
    //printf("%s\n", str);
    int i = 5;
    ft_printf("%p\n", &i);
    printf("%p\n", &i);
    ft_printf("%p\n", str);
    printf("%p\n", str);
    ft_printf("%X\n", 255);
    printf("%X\n", 255);
    ft_printf("Colore %s, numero1 %d, numero2 %05d, hex %x, float %5.2f.\n",
             "rosso", 123456, 89, 255, 3.14);
    printf("Colore %s, numero1 %d, numero2 %05d, hex %x, float %5.2f.\n",
             "rosso", 123456, 89, 255, 3.14);
    printf("%d\n", ft_printf("%s\n", "test1"));
    ft_printf("%a\n");
}