#include "ft_printf.h"
#include <stdio.h>

int main(void)
{
    int n = 2;
    ft_printf("%c\n", 'a');
    ft_printf("%c %c\n", 'a', 98);
    ft_printf("%s\n", "hola");
    char arr[] = {'c', 'i', 'a', 'o'};
    ft_printf("%s\n", arr);
    ft_printf("%p\n", &n);
    ft_printf("%i\n", 54);
    ft_printf("%u\n", 12);
    ft_printf("%x\n", 225);

    ft_printf("%d\n", 0);

    ft_printf("%d\n", -2143487648);
    ft_printf("%s\n", "");
    char *str = "ciaone";
    printf("%d\n",ft_printf("%s\n", str));
    ft_printf("%p\n", str);
    ft_printf("%%\n");
    ft_printf("%\n");
    ft_printf("%a");
    ft_printf("////////////////////////////////////////////");
    int n1 = 2;
    printf("%c\n", 'a');
    printf("%c %c\n", 'a', 98);
    printf("%s\n", "hola");
    char arr1[] = {'c', 'i', 'a', 'o'};
    printf("%s\n", arr1);
    printf("%p\n", &n1);
    printf("%i\n", 54);
    printf("%u\n", 12);
    printf("%x\n", 225);

    printf("%d\n", 0);

    printf("%d\n", -2143487648);
    printf("%s\n", "");
    char *str2 = NULL;
    // printf("%s\n", str2);
    printf("%p\n", str2);
    //printf("%a");

    ft_printf("%a");
    // char *user_input = "hello";
}