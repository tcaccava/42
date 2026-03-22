#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int maze(int n, char **matrix, int x, int y)
{
    int solutions = 0;
    if (matrix[x][y] == '1')
        return 0;
    if (x == n - 1 && y == n - 1)
        return 1;

    matrix[x][y] = '1';
    if (x > 0)
        solutions += maze(n, matrix, x - 1, y);
    if (y > 0)
        solutions += maze(n, matrix, x, y - 1);
    if (x < n - 1)
        solutions += maze(n, matrix, x + 1, y);
    if (y < n - 1)
        solutions += maze(n, matrix, x, y + 1);
    matrix[x][y] = '0';

    return solutions;
}

int main()
{
    int n = 6;
    char **matrix = malloc(sizeof(char *) * n);
    for (int i = 0; i < n; i++)
    {
        matrix[i] = malloc(n);
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            matrix[i][j] = '0';
    }
    printf("%d\n", maze(n, matrix, 0, 0));
}