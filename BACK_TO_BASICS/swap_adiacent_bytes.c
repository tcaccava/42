/*
 * =====================================================================================
 * PARADIGMA SWAR: SWAP DI BYTE ADIACENTI A 32-BIT
 * =====================================================================================
 *
 * BACKGROUND TEORICO:
 * Il codice implementa l'algoritmo SWAR (SIMD Within A Register) per la permutazione
 * parallela di sotto-blocchi di bit. A differenza dell'approccio classico (maschera e poi shift),
 * questo pattern esegue prima lo shift e applica la maschera di allineamento subito dopo.
 *
 * Dato un registro a 32 bit suddiviso in 4 byte [B3][B2][B1][B0]:
 * 1. (n << 8) sposta tutti i byte a sinistra di una posizione -> [B2][B1][B0][00].
 * Applicando la maschera 0xFF00FF00 isoliamo solo i byte che si sono spostati nelle
 * posizioni dispari corrette: [B2][00][B0][00].
 *
 * 2. (n >> 8) sposta tutti i byte a destra di una posizione -> [00][B3][B2][B1].
 * Applicando la maschera 0x00FF00FF isoliamo i byte nelle posizioni pari: [00][B3][00][B1].
 *
 * 3. L'operatore OR (|) unisce i due flussi intermedi generando: [B2][B3][B0][B1].
 * Il risultato è lo swap simultaneo delle coppie adiacenti (0 con 1, 2 con 3) in tempo costante O(1).
 *
 * APPLICAZIONI PRATICHE:
 * Questa specifica operazione è un pilastro in due domini critici del low-level programming:
 * - Network & Protocol Handling: Quando si opera su payload misti in contesti Big-Endian 
 * (Network Byte Order) che contengono sequenze di dati a 16 bit (come gli header UDP/TCP o
 * ID di protocolli industriali come Modbus), questo swap adatta le word all'architettura
 * Little-Endian della CPU senza toccare la memoria orientata ai byte.
 * - Digital Audio Processing (PCM Interleaved): Nei flussi audio stereo a 16 bit, i campioni
 * vengono trasmessi alternando canale sinistro (L) e canale destro (R) [L0][R0][L1][R1].
 * Questo algoritmo permette di invertire la polarità spaziale dei canali (Left-to-Right Swap)
 * di due campioni simultaneamente in un unico ciclo di clock del registro a 32 bit.
 *
 * IMPLEMENTAZIONI HARDWARE & OTTIMIZZAZIONE COMPILATORE (SOTA):
 * Sebbene il codice sia scritto usando operazioni bitwise esplicite per garantire la 
 * portabilità, i compilatori moderni (GCC 11+, Clang 13+) dotati di ottimizzazione (-O3) 
 * non eseguono letteralmente questi shift e maschere. Il compilatore riconosce l'idioma 
 * strutturale di re-packing e lo mappa direttamente sul silicio sfruttando istruzioni dedicate:
 *
 * - Architettura ARM: Viene compilato direttamente nell'istruzione nativa 'REV16'. Questa
 * istruzione inverte l'ordine dei byte all'interno di ciascuna meta-parola (half-word) 
 * a 16 bit in un singolo ciclo di clock.
 *
 * - Architettura x86_64: Poiché l'istruzione 'BSWAP' inverte completamente tutti e 4 i byte 
 * (0 con 3, 1 con 2), il compilatore implementa questo swap parziale combinando 'BSWAP' 
 * con una rotazione hardware (ROL/ROR di 16 posizioni), riducendo l'intera funzione a sole 
 * due istruzioni macchina senza alcuna sosta nella pipeline di esecuzione.
 * =====================================================================================
 */

#include <stdio.h>

/**
 * @brief Scambia i byte adiacenti di un intero a 32 bit (0b[B3][B2][B1][B0] -> 0b[B2][B3][B0][B1]).
 * @param n Il registro a 32 bit da permutare.
 * @return Il registro con le coppie di byte invertite.
 */
unsigned int swap_adjacent_bytes(unsigned int n) 
{
    // Parentesi aggiunte per evitare i warning di precedenza (-Wparentheses)
    // e garantire la massima conformità agli standard ISO C.
    return ((n << 8) & 0xFF00FF00U) | ((n >> 8) & 0x00FF00FFU);
}

int main(void) 
{
    unsigned int test_val = 0x00FF00FFU;
    unsigned int result   = swap_adjacent_bytes(test_val);

    printf("==================================================\n");
    printf(" SWAP ADJACENT BYTES - PERFORMANCE TEST           \n");
    printf("==================================================\n");
    printf("Input originale:  0x%08X\n", test_val);
    printf("Output elaborato: 0x%08X\n", result);
    printf("==================================================\n");

    return 0;
}