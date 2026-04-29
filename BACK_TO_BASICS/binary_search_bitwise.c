#include <stdio.h>
//ritorna l'indice del numero target se presente nell'array di dimensione n,ma senza iterare su tutto l'array
int ft_binary_search_bitwise(int *arr, int n, int target)
{
    int low = 0;
    int hi = n - 1;

    while (low <= hi)
    {
        int mid = low + ((hi - low) >> 1); // usare questa formula astrusa al posto del più comprensibile (low + hi)/2 serve per evitare che la somma dei due dia overflow PRIMA di dividere per 2
        if (arr[mid] == target)
            return mid;
        else if (target > arr[mid])
            low = mid + 1;
        else if (target < arr[mid])
            hi = mid - 1;
    }
    return -1;
}

int main()
{
    int arr[] = {1, 2, 4, 5, 6, 7, 8, 9, 10, 11};
    printf("%d\n", ft_binary_search_bitwise(arr, 10, 11));
}