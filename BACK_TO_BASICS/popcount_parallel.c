#include <stdio.h>
#include <stdint.h>
#include "./popcount_swar32.c"
/*
 * =====================================================================================
 * PARALLEL POPCOUNT REDUCTION
 * =====================================================================================
 *
 * Calcola in serie il popcount per ogni elemento di un array di dati e poi somma a coppie
 * quei popcount sfruttando un uint64_t come accumulatore,dimezzando il numero di addizioni
 * richieste.
 *
 * BASI TEORICHE:
 * Implementa una "Parallel Reduction" utilizzando il paradigma SWAR (SIMD Within
 * A Register). Invece di accumulare i risultati serialmente (creando una dipendenza
 * sequenziale), il loop unisce i calcoli a coppie. La complessità asintotica rimane
 * O(n), ma si dimezzano fisicamente le iterazioni del ciclo di riduzione, massimizzando
 * l'Instruction-Level Parallelism (ILP).
 *
 * IMPLEMENTAZIONE HARDWARE:
 * Il codice sfrutta l'intera larghezza del data bus e della ALU a 64 bit.
 * "Impacchettando" due interi a 32 bit in un singolo uint64_t, creiamo due "corsie"
 * (lanes) isolate. Quando eseguiamo 'res += temp', l'hardware della CPU esegue due
 * addizioni distinte in un singolo ciclo di clock. Le corsie sono matematicamente
 * sicure: il popcount massimo di 32 non causerà mai un overflow verso la corsia
 * adiacente. Infine, la maschera bitwise finale risolve gli array dispari
 * azzerando completamente le penalità da branch prediction (0 salti condizionali).
 *
 * APPLICAZIONI PRATICHE:
 * Questa tipologia di ottimizzazione è fondamentale in domini "compute-bound" dove
 * si analizzano flussi continui di dati grezzi:
 * - Bioinformatica: calcolo ultra-veloce della Distanza di Hamming tra sequenze DNA.
 * - Database: calcoli su indici a bitmap e filtri di Bloom.
 * - Motori Scacchistici (Chess Engines): valutazione rapida dei pezzi sulle Bitboards.
 * - Crittografia: analisi crittanalitica e parity checking.
 * =====================================================================================
 */

unsigned long popcount_parallel(unsigned int *arr, int n, unsigned int *results)
{
    int i = 0;

    if (n <= 0)
        return 0;
    // calcolo in serie il popcount di ogni singolo elemento dell'array
    for (; i < n; i++)
        results[i] = popcount_swar32(arr[i]);

    uint64_t res = 0;

    // ciclo sull'array dei risultati finche ci sono almeno due elementi rimasti,il che significa
    // che se l'array aveva un numero dispari di elementi i uscira' dall'ultimo ciclo pari a 1.mentre se era pari
    // sara' 0
    while (i > 1)
    {
        uint64_t temp = results[--i];
        temp = (temp << 32) | results[--i];
        res += temp;
    }

    // l'eventuale elemento avanzato nel caso l'array fosse dispari è SEMPRE posizionato all'indice 0.
    // uso un bit trick branchless per aggiungere result[0] solo quando i e' 1
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
