/*
 * =====================================================================================
 * BIT_GATHER_SCATTERED - ALGORITMO DI DE-INTERLEAVING (COMPRESSIONE BIT)
 * =====================================================================================
 *
 * BACKGROUND TEORICO:
 * L'operazione di Bit Gathering è l'inverso dell'istruzione PDEP (Parallel Bits Deposit).
 * Mentre il "Deposit" sparge bit contigui in una mappa arbitraria, il "Gather" (spesso 
 * implementato tramite l'istruzione PEXT - Parallel Bits Extract nelle architetture 
 * x86-64 BMI2) estrae bit dispersi in posizioni non lineari per ricomporli in una 
 * sequenza contigua (LSB-aligned).
 *
 * LOGICA OPERATIVA:
 * Il processo estrae i bit sorgente in base a una mappa di posizioni positions[i]. 
 * A differenza del "Deposit", dove il flusso è Sorgente -> Destinazione, qui 
 * seguiamo il flusso Destinazione <- Sorgente: il bit nella posizione 
 * positions[i] del registro sorgente viene instradato nella posizione i del 
 * registro di destinazione.
 *
 * APPLICAZIONI PRATICHE:
 * - Compilatori (Register Renaming): Recupero dei tag dei registri fisici dagli opcode 
 * delle istruzioni macchina dove i campi sono frammentati.
 * - Digital Signal Processing (DSP): De-interleaving di dati seriali dove i bit 
 * di controllo sono "annidati" tra i dati utili (bit-stuffing).
 * - Crittografia: Decodifica di flussi dove la permutazione è basata su maschere 
 * dinamiche (es. cifrari a blocchi custom o algoritmi di hashing).
 *
 * IMPLEMENTAZIONI HARDWARE:
 * In ambito ASIC/FPGA, questa funzione viene sintetizzata come una rete di 
 * multiplexer (MUX) o tramite il "barrel shifter" configurabile. In architetture 
 * x86-64 con set di istruzioni BMI2, questa operazione viene eseguita in un 
 * singolo ciclo di clock tramite l'istruzione PEXT (Parallel Bits Extract), 
 * che è il gold standard SOTA per la manipolazione di bit su scala industriale.
 * =====================================================================================
 */

#include <stdio.h>

/**
 * @brief Estrae bit sparsi da un numero e li compatta in un intero.
 * @param n Numero sorgente (i bit utili sono dispersi).
 * @param positions Array di indici che indica da dove "pescare" ogni bit.
 * @param count Numero di bit da raccogliere.
 * @return Un intero con i bit compattati in ordine crescente.
 */
unsigned int bit_scatter(unsigned int n, unsigned int *positions, int count) 
{
    unsigned int res = 0;
    
    for (int i = 0; i < count; i++) 
    {
        /* * 1. MASCHERAMENTO: Forziamo la posizione nell'intervallo [0, 31]
         * 2. ESTRAZIONE: Leggiamo il bit alla posizione pos in n
         * 3. COMPATTAZIONE: Spostiamo il bit raccolto nella posizione i di res
         */
        unsigned int pos = positions[i] & 31;
        unsigned int bit = (n >> pos) & 1;
        
        res |= (bit << i);
    }
    
    return res;
}

int main(void) 
{
    /* Esempio: Estrarre bit nelle posizioni 0, 2, 4, 6 di n */
    /* n = 0b01010101 (85), pos = {0, 2, 4, 6} -> bit in pos 0,2,4,6 sono 1,1,1,1 */
    unsigned int n = 0b01010101; 
    unsigned int positions[] = {0, 2, 4, 6};
    
    unsigned int result = bit_scatter(n, positions, 4);
    
    printf("Risultato Gather: 0x%04X\n", result);
    
    return 0;
}