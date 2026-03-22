#include <stdio.h>

int is_sorted(int *a, int size)
{
    if (size == 1)
        return 1;
    if (a[size - 1] < a[size - 2])
        return 0;
    return is_sorted(a, size - 1);
}

int main()
{
    int sorted[] = {1, 2, 3, 4, 5, 6};
    int not_sorted[] = {1, 2, 3, 4, 3, 2};
    printf("%d\n", is_sorted(sorted, 6));
    printf("%d\n", is_sorted(not_sorted, 6));
}