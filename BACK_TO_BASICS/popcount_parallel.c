#include <stdio.h>

void popcount_parallel(unsigned int *arr, int n, unsigned int *results) — applica popcount_swar32 a un intero array in parallelo, salvando i risultati. Poi calcola la somma totale di tutti i popcount con SWAR su uint64_t processando due elementi alla volta.


