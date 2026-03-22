#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int ft_ultimate_range(int **range, int min, int max)
{
    if (min >= max)
    {
        *range = NULL;
        return 0;
    }
    *range = malloc(sizeof(int) * (max - min));//facendo questo vado a cambiare dove punta range,per questo ho bisogno del doppio puntatore
    if (!*range)
        return 0;
    int i = 0;
    int diff = max - min;
    while (i < diff)
        (*range)[i++] = min++;
    return i;
}

int main()
{
    int *range;
    printf("%d\n", ft_ultimate_range(&range,2, 15));
    for (int i = 0; i <= 12; i++)
        printf("%d", range[i]);
    printf("\n");
}