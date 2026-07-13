
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

/**
 * =====================================================================================
 * @file        popcount_range.c
 * @brief       Conteggio parallelo (SWAR) di bit in un buffer di memoria.
 * =====================================================================================
 *
 * 1. BACKGROUND TEORICO: HAMMING WEIGHT E ALGORITMI SWAR
 * -------------------------------------------------------------------------------------
 * Il problema di contare i bit impostati a '1' (Hamming Weight) in una parola di memoria
 * viene risolto in modo naive iterando su ogni bit. L'approccio SWAR (SIMD Within A
 * Register) ottimizza questo processo trattando il registro come un vettore di piccoli
 * contatori che sommano i bit in parallelo tramite una struttura ad albero logaritmico.
 *
 * 2. APPLICAZIONI PRATICHE
 * -------------------------------------------------------------------------------------
 * - DATABASE & BITMAP INDEXING: Query ultra-veloci su milioni di record rappresentati
 *   come bit-field.
 * - BIOINFORMATICA: Allineamento di sequenze genetiche.
 * - COMPRESSIONE: Calcolo rapido dell'entropia o degli indici di sparsità.
 * - CRITTOGRAFIA: Analisi del peso di chiavi e vettori di inizializzazione.
 *
 * 3. IMPLEMENTAZIONE HARDWARE E OTTIMIZZAZIONE
 * -------------------------------------------------------------------------------------
 * L'approccio vettoriale (64-bit) riduce il numero di operazioni da O(N) a O(N/64).
 * - ALLINEAMENTO: L'accesso a 64 bit richiede che il puntatore sia allineato.
 *   L'uso di uintptr_t per verificare l'allineamento evita crash su architetture
 *   strict-alignment (ARM/RISC-V).
 * - BRANCHLESS: Il conteggio SWAR è intrinsecamente branchless, eliminando il costo
 *   della predizione dei salti (misprediction penalty) durante lo scanning del buffer.
 */

// Algoritmo SWAR per conteggio bit su 64 bit
static inline uint64_t popcount_swar64(uint64_t n)
{
    n = n - ((n >> 1) & 0x5555555555555555ULL);
    n = (n & 0x3333333333333333ULL) + ((n >> 2) & 0x3333333333333333ULL);
    n = (n + (n >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    return (n * 0x0101010101010101ULL) >> 56;
}

// Versione scalare (base)
int bit_count_range(unsigned char *data, size_t n, int from, int to)
{
    if (!data || n == 0 || from > to || from < 0)
        return 0;
    if (to >= (int)(n * 8))
        to = (n * 8) - 1;

    int count = 0;
    for (int i = from; i <= to; i++)
    {
        int byte_idx = i / 8;
        int bit_idx = i % 8;
        if ((data[byte_idx] >> bit_idx) & 1)
            count++;
    }
    return count;
}

// Versione SWAR 64 pura
int ft_bit_count_range(unsigned char *data, size_t n, int from, int to)
{
    int count = 0;
    int cur = from;// bit di partenza

    // --- TESTA ---
    // Vado avanti un bit alla volta finché non sei su un byte allineato a 8 E sono all'inizio del byte(cur % 8 == 0)
    while (cur <= to && ((uintptr_t)(data + (cur / 8)) & 7 || cur % 8 != 0))// il ciclo while continua ad avanzare bit per bit finche ENTRAMBE le condizioni non diventano false
    {
        if ((data[cur / 8] >> (cur % 8)) & 1)
            count++;
        cur++;
    }

    // --- CORPO ---
    // Prendo 8 byte (64 bit) alla volta e uso il popcount
    uint64_t *ptr = (uint64_t *)(data + (cur / 8));
    while (cur + 63 <= to)
    {
        count += (int)popcount_swar64(*ptr); // Somma 64 bit in un colpo
        ptr++;        // Sposta il puntatore di 8 byte
        cur += 64;    // Salta avanti di 64 bit nel nastro
    }

    // --- CODA ---
    // Finisci gli ultimi bit rimasti
    while (cur <= to)
    {
        if ((data[cur / 8] >> (cur % 8)) & 1)
            count++;
        cur++;
    }
    return count;
}

int main()
{
    unsigned char data[] = {0b10011100, 0b01011110, 0b10000110};
    printf("Risultato: %d\n", ft_bit_count_range(data, 3, 3, 17));
    return 0;
}