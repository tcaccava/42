#include <stdio.h>

int pascal_triangle(int row, int col)
{
    if (col == 0)
        return 1;
    if (col == row)
        return 1;

    return pascal_triangle(row - 1, col - 1) + pascal_triangle(row - 1, col);
}

int main()
{
    printf("%d\n", pascal_triangle(4, 2));
}