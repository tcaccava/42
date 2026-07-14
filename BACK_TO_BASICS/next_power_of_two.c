/**
 * =====================================================================================
 * @file        next_pow2.c
 * @brief       Calcolo branchless della potenza di due strettamente superiore.
 *
 * 1. BACKGROUND TEORICO E MATEMATICO:
 *    Il problema di trovare la potenza di 2 successiva si riduce a due step logici:
 *    A) Identificare il Most Significant Bit (MSB) acceso.
 *    B) Generare il valore 2^(MSB + 1).
 *
 *    Fase 1: Parallel Prefix OR (Bit Smearing)
 *    Invece di cercare l'MSB con un loop (O(N)), l'algoritmo usa una progressione
 *    geometrica di shift logici (1, 2, 4). Per un intero a N bit, sono necessari
 *    log2(N) shift. 
 *    - Esempio su 8 bit: se il bit più alto è alla posizione k, shiftando di 1 e
 *      facendo OR, accendiamo il bit k-1.
 *    - Shiftando di 2, accendiamo k-2 e k-3.
 *    - Shiftando di 4, copriamo i restanti bit verso il basso.
 *    Risultato: Tutti i bit dall'MSB fino al bit 0 diventano 1. Abbiamo trasformato
 *    un numero casuale in un formato (2^(k+1)) - 1 (una maschera di saturazione).
 *
 *    Fase 2: Isolamento dell'MSB (Aritmetica di saturazione)
 *    Avendo saturato il valore (es: 00010110 diventa 00011111), sappiamo che 
 *    questo numero è dispari. Se lo shiftiamo a destra di 1 (00001111) e lo
 *    sottraiamo al valore saturato, stiamo eseguendo:
 *    ((2^(k+1)) - 1) - ((2^k) - 1) = 2^k.
 *    La sottrazione elimina tutta l'entropia bassa, lasciando intatto SOLO l'MSB.
 *
 *    Fase 3: Branchless Zero-Handling
 *    Il caso base n=0 fallisce nel Bit Smearing (restituisce 0). Ma noi vogliamo 2^0 = 1.
 *    In C, l'operatore di negazione logica '!n' restituisce 1 se n == 0, e 0 per ogni
 *    altro valore.
 *    Valutando '(!n) | (keep_highest_bit(n) << 1)':
 *    - Se n == 0:  1 | (0 << 1) -> 1
 *    - Se n > 0:   0 | (2^k << 1) -> 2^(k+1)
 *
 * 2. APPLICAZIONI PRATICHE E SISTEMISTICHE:
 *    - Buddy Memory Allocation: I kernel Linux/Unix usano algoritmi simili per
 *      arrotondare le richieste di memoria dei processi ai blocchi contigui più vicini.
 *    - Tabelle di Hash (Ring Buffers): Forzare le dimensioni degli array a potenze di 2
 *      permette di usare l'operatore AND (x & (size - 1)) invece del lento operatore 
 *      MODULO (x % size) per calcolare gli indici.
 *
 * 3. IMPLEMENTAZIONE HARDWARE (PIPELINE DELLA CPU):
 *    Questo codice è al 100% BRANCHLESS. Non ci sono istruzioni 'if'. 
 *    I salti condizionali (branch) obbligano la CPU a svuotare la pipeline se la 
 *    branch prediction fallisce, sprecando 10-20 cicli di clock. Le operazioni
 *    bitwise (OR, SHIFT, SUB) hanno latenza fissa (solitamente 1 ciclo di clock)
 *    e vengono eseguite interamente all'interno della ALU, garantendo un tempo 
 *    di esecuzione deterministico (costante). L'approccio è O(1). 
 *    Su molte architetture moderne, il calcolo di MSB è supportato da istruzioni native 
 *    (es. BSR su x86 o CLZ su ARM), rendendo questa operazione estremamente efficiente.  
 * =====================================================================================
 */

#include <stdio.h>

// Isola il Most Significant Bit (MSB) usando Parallel Prefix OR
unsigned char keep_highest_bit(unsigned char n) {
    n |= n >> 1;  // Propaga l'MSB di 1 posizione verso il basso
    n |= n >> 2;  // Propaga i 2 bit accesi di altre 2 posizioni
    n |= n >> 4;  // Propaga i 4 bit accesi di altre 4 posizioni -> saturazione a 8 bit completata
    return n - (n >> 1);  // Sottrazione bitwise per isolare l'MSB originale
}

// Ritorna la piu' piccola potenza di 2 strettamente maggiore di n (Branchless)
unsigned char next_power_of_two(unsigned char n)
{
    // !n gestisce il caso 0 restituendo 1.
    // keep_highest_bit(n) << 1 moltiplica per 2 l'MSB isolato.
    return (!n) | (keep_highest_bit(n) << 1); 
}

int main()
{
    printf("next(0) = %d\n", next_power_of_two(0));
    printf("next(7) = %d\n", next_power_of_two(7));
    printf("next(8) = %d\n", next_power_of_two(8));
    return 0;
}