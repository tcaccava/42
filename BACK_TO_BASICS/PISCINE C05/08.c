#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// #include <unistd.h>

// int is_safe(int pos[10], int col, int row)
// {
//     int i;

//     i = 0;
//     while (i < col)
//     {
//         if (pos[i] == row)
//             return 0;
//         if (pos[i] - i == row - col)
//             return 0;
//         if (pos[i] + i == row + col)
//             return 0;
//         i++;
//     }
//     return 1;
// }

// void print_solution(int pos[10])
// {
//     char c;
//     int i;

//     i = 0;
//     while (i < 10)
//     {
//         c = pos[i] + '0';
//         write(1, &c, 1);
//         i++;
//     }
//     write(1, "\n", 1);
// }

// int solve(int pos[10], int col)
// {
//     int row;
//     int count;

//     if (col == 10)
//     {
//         print_solution(pos);
//         return 1;
//     }

//     count = 0;
//     row = 0;
//     while (row < 10)
//     {
//         if (is_safe(pos, col, row))
//         {
//             pos[col] = row;
//             count += solve(pos, col + 1);
//         }
//         row++;
//     }
//     return count;
// }

// int ft_ten_queens_puzzle(void)
// {
//     int pos[10];

//     return solve(pos, 0);
// }

//******************************************************************

// int right(int row, int col, char **matrix)
// {
//     if (col > 9)
//         return 0;
//     if (matrix[row][col] == 'Q')
//         return 1;
//     return right(row, col + 1, matrix);
// }

// int left(int row, int col, char **matrix)
// {
//     if (col < 0)
//         return 0;
//     if (matrix[row][col] == 'Q')
//         return 1;
//     return left(row, col - 1, matrix);
// }

// int up(int row, int col, char **matrix)
// {
//     if (row < 0)
//         return 0;
//     if (matrix[row][col] == 'Q')
//         return 1;
//     return up(row - 1, col, matrix);
// }

// int down(int row, int col, char **matrix)
// {
//     if (row > 9)
//         return 0;
//     if (matrix[row][col] == 'Q')
//         return 1;
//     return down(row + 1, col, matrix);
// }

// int torre(int row, int col, char **matrix)
// {
//     return (right(row, col + 1, matrix) || left(row, col - 1, matrix) || up(row - 1, col, matrix) || down(row + 1, col, matrix));
// }

// int ul(int row, int col, char **matrix)
//  {
//      if (row < 0 || col < 0)
//          return 0;
//      if (matrix[row][col] == 'Q')
//          return 1;
//      return ul(row - 1, col - 1, matrix);
//  }
//  int ur(int row, int col, char **matrix)
//  {
//      if (row < 0 || col > 9)
//          return 0;
//      if (matrix[row][col] == 'Q')
//          return 1;
//      return ur(row - 1, col + 1, matrix);
//  }
//  int bl(int row, int col, char **matrix)
//  {
//      if (row > 9 || col < 0)
//          return 0;
//      if (matrix[row][col] == 'Q')
//          return 1;
//      return bl(row + 1, col - 1, matrix);
//  }
//  int br(int row, int col, char **matrix)
//  {
//      if (row > 9 || col > 9)
//          return 0;
//      if (matrix[row][col] == 'Q')
//          return 1;
//      return br(row + 1, col + 1, matrix);
//  }
//  int alfiere(int row, int col, char **matrix)
//  {
//      return (ul(row - 1, col - 1, matrix) || ur(row - 1, col + 1, matrix) || bl(row + 1, col - 1, matrix) || br(row + 1, col + 1, matrix));
//  }

//*******************************************************************************

int torre(int row, int col, char **matrix)
{
    for (int i = 0; i <= 9; i++)
    {
        for (int j = 0; j <= 9; j++)
        {
            if ((row == i || col == j) && matrix[i][j] == 'Q')
                return 1;
        }
    }
    return 0;
}

int alfiere(int row, int col, char **matrix)
{
    for (int i = 0; i <= 9; i++)
    {
        for (int j = 0; j <= 9; j++)
        {
            if (matrix[i][j] == 'Q')
            {
                if (i - j == row - col)
                    return 1;
                if (i + j == row + col)
                    return 1;
            }
        }
    }
    return 0;
}

int is_not_legit(int row, int col, char **matrix)
{
    return (torre(row, col, matrix) || alfiere(row, col, matrix));
}

int solve(int col, char **matrix)
{
    int total = 0;
    if (col == 10)
        return 1;
    int row = 0;
    while (row <= 9)
    {
        if (!is_not_legit(row, col, matrix))
        {
            matrix[row][col] = 'Q';
            total += solve(col + 1, matrix);
            matrix[row][col] = '0';
        }
        row++;
    }
    return total;
}

int main()
{
    char **matrix = malloc(sizeof(char *) * 10);
    for (int i = 0; i <= 9; i++)
        matrix[i] = malloc(sizeof(char) * 10);
    for (int i = 0; i <= 9; i++)
    {
        for (int j = 0; j <= 9; j++)
            matrix[i][j] = '0';
    }
    printf("%d\n", solve(0, matrix));
}