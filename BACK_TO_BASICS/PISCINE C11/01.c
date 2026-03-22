#include <stdlib.h>
#include <unistd.h>

int *ft_map(int *tab, int length, int (*f)(int))
{
    int *new_array = malloc(sizeof(int) * length);
    if (!new_array)
        return NULL;
    int i = 0;
    while (i < length)
    {
        new_array[i] = f(tab[i]);
        i++;
    }
    return new_array;
}

int add_one(int num)
{
    return num + 1;
}

int main()
{
    int arr[] = {1, 2, 3, 4, 5, 6};
    int (*f)(int) = add_one;
    int *new_array = ft_map(arr, 6, f);
    int i = 0;
    while (i < 6)
    {
        char c = new_array[i++] + '0';
        write(1, &c, 1);
    }
    free(new_array);
}