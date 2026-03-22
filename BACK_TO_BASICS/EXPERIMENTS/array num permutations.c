#include <unistd.h>
// ho risolto questo esercizio in 3 modi diversi:
// 1) printo tutte le permutazioni,anche quelle con doppioni usando solo un ciclo
// while (i < len)
// {
//    tmp[tindex] = a[i];
//    permutations(a, index + 1, len, tmp, tindex + 1);
//    i++;
// }
// poi le filtro con contains double prima di printare,escludendo tutte quelle con doppioni
//
// 2) in fase di permutazione controllo con contains a double che ogni elemento che sto per aggiungere a tmp
// non sia uguale ad a[i]. Limite: funziona solo se a[i] non contiene di per se doppioni.
//
// void permutations(int *a, int index, int len, int *tmp, int tindex)
// {
//     if (index == len)
//     {
//         // if (!contains_double(tmp, tindex))
//         print(tmp, tindex);
//         return;
//     }
//     int i = 0;
//     while (i < len)
//     {
//         if (!contains_a_double(tmp, tindex, a[i]))
//         {
//             tmp[tindex] = a[i];
//             permutations(a, index + 1, len, tmp, tindex + 1);
//         }
//         i++;
//     }
// }
//
// 3) soluzione seguente con backtracking,non commentata,la migliore perche' piu' versatile

// int contains_double(int *a, int len)
// {
//     for (int i = 0; i < len; i++)
//     {
//         for (int j = i + 1; j < len; j++)
//             if (a[i] == a[j])
//                 return 1;
//     }
//     return 0;
// }

// int contains_a_double(int *a, int size, int doub)
// {
//     int i = 0;
//     while (i < size)
//     {
//         if (a[i] == doub)
//             return 1;
//         i++;
//     }
//     return 0;
// }

void print(int *tmp, int tmp_index)
{
    int i = 0;
    while (i < tmp_index)
    {
        char c = tmp[i] + '0';
        write(1, &c, 1);
        i++;
    }
    write(1, "\n", 1);
}

void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void permute(int *a, int index, int size)
{
    if (index == size)
    {
        print(a, size);
        return;
    }
//per ogni posizione index dell'array faccio lo swap in place con gli elementi rimanenti
    for (int i = index; i < size; i++)
    {
        swap(&a[index], &a[i]);     // scelta
        permute(a, index + 1, size);
        swap(&a[index], &a[i]);     // BACKTRACK
    }
}

int main() {
    int a[] = {1,2,3};
    permute(a,0,3);
}