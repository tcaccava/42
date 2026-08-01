#include <stdlib.h>
#include <stdio.h>

void recur(int *arr, int sum, int n, int index, int current_sum, int *buffer, int buf_size)
{
    if (index == n)
    { // se avanzando l'indice sono arrivato ad n
        // significa che l'array e' finit0,quindi in ogni caso devo ritornare
        // terminando la catena ricorsiva

        if (current_sum == sum && buf_size > 0) // tuttavia se il buffer che ho costruito
        // sommato da' proprio sum,ho trovato un sottoinsieme valido,quindi lo stampo prima di ritornare
        {
            printf("Soluzione: ");
            for (int i = 0; i < buf_size; i++)
                printf("%d%s ", buffer[i], (i == buf_size - 1) ? "" : " +");
            printf(" = %d\n", sum);
        }
        return;
    }
    buffer[buf_size] = arr[index]; // prendo il numero e ricorro
    recur(arr, sum, n, index + 1, current_sum + arr[index], buffer, buf_size + 1);
    // non lo prendo e avanzo al numero successivo di arr
    recur(arr, sum, n, index + 1, current_sum, buffer, buf_size);
}

int main(int c, char **v)
{
    if (c < 3) // ci deve essere almeno il programma,il target e un numero di test
    {
        printf("Error");
        return -1;
    }
    int n = c - 2;                         // numero di addendi da testare
    int sum = atoi(v[1]);                  // il target somma
    int *arr = malloc(sizeof(int) * n);    // l'array formato dagli addendi da terminale
    int *buffer = malloc(sizeof(int) * n); // il buffer temporaneo
    for (int i = 0; i < n; i++)
        arr[i] = atoi(v[i + 2]);         // faccio aoti per ogni numero da terminale
    recur(arr, sum, n, 0, 0, buffer, 0); // catena ricorsiva
}

// 11 3 8 2 7 1