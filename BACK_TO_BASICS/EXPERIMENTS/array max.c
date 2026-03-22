#include <stdio.h>
#include <unistd.h>

int max(int *a, int size)
{
    int massimo;
    if(size == 0)
        return 0;
    massimo = max(a, size - 1);
    return (massimo > a[size - 1]) ? massimo : a[size - 1];
}

int main()
{
    int array[8] = {2, 46, 543, 50000, 9, 76, 4, 18};
    printf("%d\n", max(array, 8));
}