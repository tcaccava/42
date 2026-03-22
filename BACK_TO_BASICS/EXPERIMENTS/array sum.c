#include <unistd.h>
#include <stdio.h>

int sum(int *a, int size)
{
    int result;
    if (size == 0)
        return 0;
    result = a[size - 1] + sum(a, size - 1);
    return result;
}

int main()
{
    int a[] = {1, 2, 3, 4, 5};
    printf("%d\n", sum(a, 5));
}
