#include <unistd.h>
#include <stdio.h>

int count_occ(int *a, int size, int x)
{
    int occurrence = 0;
    if(size == 0)
        return 0;
    if (a[size - 1] == x)
        occurrence++;
    occurrence += count_occ(a, size - 1, x);
    return occurrence;
}

int main()
{
    int array[8] = {2, 4, 4, 1, 9, 3, 4, 6};
    printf("%d\n", count_occ(array, 8, 4));
}
