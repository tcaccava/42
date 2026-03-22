#include <stdlib.h>
#include <stdio.h>

int *ft_range(int min, int max)
{
    if (min >= max)
        return NULL;
    int diff = max - min;
    int *a = malloc(sizeof(int) * (diff));
    if (!a)
        return NULL;
    int i = 0;
    while (i < diff)
        a[i++] = min++;
    return a;
}

int main()
{
    int *a = ft_range(2, 15);
    for (int i = 0; i <= 12; i++)
    {
        printf("%d", a[i]);
    }
    printf("\n");
}