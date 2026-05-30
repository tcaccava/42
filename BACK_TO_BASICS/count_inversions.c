#include <stdio.h>
// Conta le inversioni in un array (coppie i,j dove i < j ma arr[i]>arr[j]) usando operazioni bitwise per il confronto branchless.
// Un array ordinato ha 0 inversioni, uno invertito ha n*(n-1)/2. Usato per misurare quanto un array è disordinato —
// applicazione diretta in algoritmi di ranking e recommendation systems.
// int count_inversions(int *arr, int n)
// {
//     if (n < 2 || !arr)
//         return 0;
//     int inv = 0;
//     int i = 0;
//     for (; i < n - 1; i++)
//         for (int j = i + 1; j < n; j++)
//             inv += (arr[i] > arr[j]);
//     return inv;
// }

int main()
{
    int arr[] = {1, 4, 6, 5, 7, 8, 10, 9, 3};
    printf("%d\n", count_inversions(arr, 9));
}

int inversion(int *half1, int *half2, void (*d)(int*, int), void (*d2)(int*, int), int a1, int a2)
{
    int arr[a1 + a2];
    int count = 0;
    int p, q = 0;
    for (int i = 0; i < (a1 + a2); i++)
    {
        if (half1[p] < half2[q])
        {
            arr[i] = half1[p];
            p++;
            if (p == a1 / 2)
            {
                for (; q < a2 / 2; q++)
                    arr[i++] = half2[q];
                break;
            }
        }
        else
        {
            arr[i] = half2[q];
            count += a1 - p;
            q++;
            if (q == a2 / 2)
            {
                for (; p < a1 / 2; p++)
                    arr[i++] = half2[p];
                break;
            }
        }
    }
    return count;
}


int divide(int *array, int n)
{
    if (n == 1)
        return 0;
    int inv = 0;
    int a1 = n / 2;
    int a2 = a1 + (n & 1);
    int half1[a1];
    int half2[a2];
    int i = 0;
    for (; i < a1; i++)
        half1[i] = array[i];
    for (int j = 0; j < a2; j++)
        half2[j] = array[i++];
    return inversion(half1,half2,divide(half1,a1), divide(half2,a2),a1, a2);
}

