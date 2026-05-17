#include <stdio.h>
#include "./popcount_swar32.c"
int count_bits_in_range32(unsigned int n, int from, int to)
{
    from &= 31; // se from o to sono superiori a 32 ne prendiamo solo il resto
    to &= 31;
    if (from < to) // facciamo swap in place senza temporanea se from e' minore di to sfruttando la reversibilita' di xor
    {
        from ^= to;
        to ^= from;
        from ^= to;
    }
    unsigned char is_edge_case = from - to == 31;                            // se from e' 31 e to 0,nella riga successiva otterremmo una maschera di lunghezza 32,e shiftare un numero di 32 posizioni in C e' undefined behavior
    unsigned int mask_length = (from - to + 1) - is_edge_case;               // calcoliamo la lunghezza della maschera di cui abbiamo bisogno,riducendola di un unita' nel caso dell'edge case
    unsigned int mask = (1U << mask_length) - 1;                             // sottraendo 1 alla potenza di 2 elevata a mask length,accendiamo tutti i bit a destra,ottenendo una sequenza di 1 di lunghezza pari a mask length
    unsigned int mask_shifted = (mask << to) | ((1U << 31) & -is_edge_case); // shiftiamo la sequenza in posizione to; nell'edge case,visto che nella riga precedente la length e' 31,ma noi abbiamo bisogno di una mask di 32,aggiungiamo il 32esimo bit nel caso di edge case
    return popcount_swar32(n & mask_shifted);                                // selezione finale con popcount                           // con l'AND selezioniamo il popcount solo sulla maschera
}

// oppure alternativa interamente geometrica basata sul metodo dei due tagli
// int count_bits_in_range32(unsigned int n, int from, int to)
// {
//     from &= 31;
//     to &= 31;
//
//     if (from < to)
//     {
//         from ^= to;
//         to ^= from;
//         from ^= to;
//     }
//
//     Costruzione della maschera tramite intersezione di due shift sicuri (0-31)
//     unsigned int mask = (~0U >> (31 - from)) & (~0U << to);
//
//     return popcount_swar32(n & mask);
// }

int main()
{
    printf("%d %d %d\n", count_bits_in_range32(0xFF00FF00, 15, 8), count_bits_in_range32(0xFF0FF00F, 23, 0), count_bits_in_range32(0xF00F0F00, 15, 31));
    printf("%d\n", count_bits_in_range32(0xFF00FF00, 31, 0)); // edge case
}
