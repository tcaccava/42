#include <stdio.h>

// cercare ricorsivamente un numero in un array gia ordinato,restituire il suo indice se c'e o -1 se non c'e'

int binary_search(int *a, int left, int right, int target)
{
    if (left > right)
        return -1;
    int mid = (left + right) / 2;
    if (a[mid] == target)
        return mid;
    if (target < a[mid])
        return binary_search(a, left, mid - 1, target); // mid - 1 perche mid e' stato gia controllato
    else
        return (binary_search(a, mid + 1, right, target)); //
}

int main()
{
    int arr[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14};
    printf("%d\n", binary_search(arr, 0, 10, 5));
}