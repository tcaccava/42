/* ===================================================================================
 * FILE: sliding_xor.c
 *
 * 1. OBIETTIVI
 *    Implementare tre algoritmi di analisi bit-stream con diverse complessità:
 *    - block_xor: calcolo di parità(XOR-sum) su blocchi di k bit non sovrapposti e indipendenti (O(32/K)).
 *    - sliding_xor: parità su finestre scorrevoli(overlap) tramite ricalcolo (O(32*K)).
 *    - sliding_xor_fast: parità su finestre scorrevoli tramite aggiornamento O(1),indipendente da k.
 *
 * 2. BACKGROUND TEORICO: L'INVARIANTE DI PARITÀ
 *    L'operazione XOR di K bit è matematicamente equivalente al calcolo della parità 
 *    (oddismo/parità) del numero di bit accesi.
 *    - Se il conteggio dei bit accesi è dispari, lo XOR totale è 1.
 *    - Se è pari, lo XOR totale è 0.
 *    Da qui deriva la intuizione: popcount(window) & 1 è la forma canonica e 
 *    più veloce per calcolare la parità di un set di bit senza dover fare `b0 ^ b1 ^ ... ^ bk`.
 *
 *    L'algoritmo 'fast' si basa sulla proprietà commutativa e associativa dello XOR.
 *    Se conosci la parità della finestra corrente (P_old), la parità della finestra 
 *    successiva (P_new) è data da:
 *    P_new = P_old ^ BitCheEsce ^ BitCheEntra
 *    Questo trasforma un calcolo che dipendeva dalla dimensione K in un'operazione 
 *    a costo costante, indipendentemente dalla lunghezza della finestra.
 *
 * 3. APPLICAZIONI PRATICHE
 *    - Checksum & Protocolli di Streaming: In protocolli ad alta velocità, il calcolo 
 *      di parità scorrevoli viene usato per rilevare errori di bit-flip in tempo reale.
 *    - Bit-Stream Pattern Matching: Usato per trovare sequenze di dati che soddisfano 
 *      particolari vincoli di parità in flussi compressi o crittografati.
 *    - Digital Signal Processing (DSP): Il campionamento a finestra scorrevole è la 
 *      base per filtri FIR (Finite Impulse Response) a 1 bit.
 * 
 * 4. IMPLEMENTAZIONE HARDWARE
 *    - Il cuore di questo codice è il "Reduction Network":
 *      In silicio, calcolare lo XOR di K bit non avviene sequenzialmente, ma tramite 
 *      una struttura ad albero di porte XOR (XOR Tree). 
 *    - La funzione popcount(e la parità derivata) viene mappata sulle istruzioni 
 *      di POPCNT delle moderne CPU (x86, ARM). Queste istruzioni sono "pipelined": 
 *      il processore può calcolarle in un singolo ciclo di clock.
 *    - Il loop for  è un esempio di "Dataflow Parallelism": ogni 
 *      iterazione è indipendente (nel caso dello sliding, il registro a scorrimento 
 *      è la sola dipendenza), permettendo al processore di eseguire il calcolo 
 *      molto vicino al limite teorico della frequenza di clock.
 *    - L'approccio fast minimizza la pressione sulla ALU. Mentre il ricalcolo classico
 *      richiede K operazioni logiche, qui eseguiamo solo 2 XOR per iterazione.
 *    - Questo riduce drasticamente il numero di porte logiche attive e permette al 
 *      processore di mantenere la pipeline satura senza attendere i risultati dei 
 *      calcoli del popcount per ogni finestra.
 * 
 * 5. NOTE DI IMPLEMENTAZIONE
 *    - Sliding Window: L'implementazione attuale esegue 32 iterazioni. Nelle ultime 
 *      iterazioni, la finestra estrae bit "zeropadded" (a causa del right shift n >>= 1). 
 *      Questo è il comportamento standard per checksum sliding.

 *    
 * ===================================================================================
 */

#include <stdio.h>
#include "./popcount_swar32.c"

/**
 * BLOCK XOR: Analisi a compartimenti stagni.
 */
unsigned int block_xor(unsigned int n, int k)
{
    k &= 31;
    unsigned int mask = (1U << k) - 1;
    unsigned int res = 0;
    unsigned int run = 32 / k;
    for (int i = 0; i < run; i++)
    {
        unsigned int window = n & mask;
        unsigned int xor = (popcount_swar32(window) & 1) << i;
        res |= xor;
        n >>= k;
    }
    return res;
}

/**
 * SLIDING XOR (Ricalcolo):
 * Crea una finestra scorrevole di dimensione K bit ri-campionando ogni volta.
 */
unsigned int sliding_xor(unsigned int n, int k)
{
    k &= 31;
    unsigned int mask = (1U << k) - 1;
    unsigned int res = 0;
    for (int i = 0; i < 32; i++)
    {
        unsigned int window = n & mask;
        unsigned int xor = (popcount_swar32(window) & 1) << i;
        res |= xor;
        n >>= 1;
    }
    return res;
}

/**
 * SLIDING XOR FAST (Aggiornamento Invariante):
 * Ottimizzazione O(N). Aggiorna la parità con soli 2 XOR per passo.
 */
unsigned int sliding_xor_fast(unsigned int n, int k)
{
    k &= 31;
    unsigned int res = 0;

    // 1. Calcola la parità della PRIMA finestra (bit 0 a k-1)
    unsigned int mask = (1U << k) - 1;
    unsigned int first_window = n & mask;
    unsigned int current_parity = popcount_swar32(first_window) & 1;
    res = current_parity;

    // 2. Sliding Update (O(1) per finestra)
    for (int i = 0; i < (32 - k); i++)
    {
        unsigned int bit_out = (n >> i) & 1;
        unsigned int bit_in = (n >> (i + k)) & 1;

        current_parity ^= (bit_out ^ bit_in);
        res |= (current_parity << (i + 1));
    }
    return res;
}

int main()
{
    unsigned int n = 0xFFFFFFFF;
    int k = 3;

    printf("Risultato sliding_xor:      %#X\n", sliding_xor(n, k));
    printf("Risultato sliding_xor_fast: %#X\n", sliding_xor_fast(n, k));
    
    return 0;
}