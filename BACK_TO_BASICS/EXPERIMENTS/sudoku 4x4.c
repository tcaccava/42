#include <stdio.h>

#define N 4 // dimensione Sudoku 4x4

// Controllo riga
int row_legit(int grid[N][N], int row, int num)
{
    for (int col = 0; col < N; col++)
        if (grid[row][col] == num)
            return 0;
    return 1;
}

// Controllo colonna
int col_legit(int grid[N][N], int col, int num)
{
    for (int row = 0; row < N; row++)
        if (grid[row][col] == num)
            return 0;
    return 1;
}

// Controllo quadrato 2x2
int box_legit(int grid[N][N], int row, int col, int num)
{
    int start_row = row - row % 2;
    int start_col = col - col % 2;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            if (grid[start_row + i][start_col + j] == num)
                return 0;
    return 1;
}

// Controllo complessivo
int is_legit(int grid[N][N], int row, int col, int num)
{
    return row_legit(grid, row, num) &&
           col_legit(grid, col, num) &&
           box_legit(grid, row, col, num);
}

// Stampa la griglia
void print_grid(int grid[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            printf("%d ", grid[i][j]);
        printf("\n");
    }
    printf("\n");
}

// Funzione ricorsiva per risolvere il Sudoku
void sudoku(int grid[N][N], int numbers[N])
{
    for (int row = 0; row < N; row++)
    {
        for (int col = 0; col < N; col++)
        {
            if (grid[row][col] == 0)
            { // cella vuota
                for (int i = 0; i < N; i++)
                {
                    int num = numbers[i];
                    if (is_legit(grid, row, col, num))
                    {
                        grid[row][col] = num;  // provo questo numero
                        sudoku(grid, numbers); // ricorsione
                        grid[row][col] = 0;    // backtracking
                    }
                }
                return; // se nessun numero valido -> torna indietro
            }
        }
    }
    // Se arrivo qui, la griglia è completa: stampo soluzione
    print_grid(grid);
}

int main()
{
    int numbers[N] = {1, 2, 3, 4};
    int grid[N][N] = {
        {1, 0, 0, 0},
        {0, 0, 0, 2},
        {0, 0, 0, 0},
        {0, 0, 0, 4}};

    sudoku(grid, numbers); // stampa tutte le soluzioni
    return 0;
}
