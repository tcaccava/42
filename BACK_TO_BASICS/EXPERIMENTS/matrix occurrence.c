#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int array_occurrence(int *a, int size, int target)
{
    if (size == 0)
        return 0;
    int occurrence = 0;
    if (a[size - 1] == target)
        occurrence++;
    occurrence += array_occurrence(a, size - 1, target);
    return occurrence;
}

int matrix_occurrence(int **mat, int rows, int cols, int target, int index)
{
    if (index == rows)
        return 0;
    int occurrence = array_occurrence(mat[index], cols, target);
    return occurrence + matrix_occurrence(mat, rows, cols, target, index + 1);
}

int main()
{
    int row0[] = {1, 2, 3, 4};
    int row1[] = {5, 2, 2, 6};
    int row2[] = {7, 8, 2, 9};
    int *mat[3] = {row0, row1, row2};
    printf("%d\n", matrix_occurrence(mat, 3, 4, 2, 0));
}