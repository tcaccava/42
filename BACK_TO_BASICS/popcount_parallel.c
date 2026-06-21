#include <stdio.h>
#include <stdint.h>
// #include "./popcount_swar32.c" // Assumiamo sia inclusa correttamente

unsigned long popcount_parallel(unsigned int *arr, int n, unsigned int *results)
{
    int i = 0;

    // Protezione base: se l'array è vuoto, ritorna 0
    if (n <= 0)
        return 0;

    for (; i < n; i++)
        results[i] = popcount_swar32(arr[i]);

    uint64_t res = 0;

    // Cambiamo la condizione: ci fermiamo finché ci sono ALMENO 2 elementi da processare.
    // L'indice scende in modo sicuro: non andrà mai sotto lo zero.
    while (i > 1)
    {
        uint64_t temp = results[--i];
        temp = (temp << 32) | results[--i];
        res += temp;
    }

    // Se l'array era dispari (es. n=7), i si fermerà esattamente a 1.
    // Se l'array era pari (es. n=6), i si fermerà esattamente a 0.
    // L'elemento "avanzato" (se c'è) è SEMPRE posizionato all'indice 0.

    // Creiamo la maschera: se i == 1 (dispari), -(1) = 0xFFFFFFFF
    // Se i == 0 (pari), -(0) = 0x00000000
    return ((res >> 32) & 0xFFFFFFFF) + (res & 0xFFFFFFFF) + (results[0] & -(i == 1));
}

int main()
{
    unsigned int arr[] = {8, 16, 32, 64, 255, 1, 1}; // 7 elementi (dispari)
    unsigned int results[7];
    printf("Dispari (7): %lu\n", popcount_parallel(arr, 7, results));

    unsigned int arr_pari[] = {8, 16, 32, 64, 255, 1}; // 6 elementi (pari)
    unsigned int results_pari[6];
    printf("Pari (6):    %lu\n", popcount_parallel(arr_pari, 6, results_pari));

    return 0;
}
https://youtu.be/gQpRfNZavhQ
