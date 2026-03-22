#include <stdio.h>

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void reverse_array(int *arr, int left, int right)
{
    if (left >= right)
        return;
    swap(&arr[left], &arr[right]);
    reverse_array(arr, left + 1, right - 1);
}

int main()
{
    int arr[] = {1, 2, 3, 4, 5};
    reverse_array(arr, 0, 4);
    int i = 0;
    while (i < 5)
    {
        printf("%d\n", arr[i++]);
    }
}