/*
 * =====================================================================================
 * BIT_DEPOSIT_SCATTERED - ALGORITMO DI MAPPATURA (SCATTER)
 * =====================================================================================
 *
 * BACKGROUND TEORICO:
 * La funzione implementa un'operazione di "Scatter" (dispersione) di bit. A differenza 
 * dell'istruzione hardware PDEP (Parallel Bits Deposit), che utilizza una maschera, 
 * questa implementazione utilizza una mappa di indici arbitrari. 
 *
 * APPLICAZIONI PRATICHE:
 * - Compilatori (Register Allocation): Durante la fase di "Code Generation", il 
 * compilatore deve mappare variabili logiche (es. registri virtuali) in campi 
 * non contigui di un'istruzione macchina (opcode),la cosidetta Instruction Set Architecture (ISA) encoding.
 * Quando un compilatore deve tradurre un'istruzione astratta in codice macchina (opcode), deve inserire i numeri 
 * dei registri scelti (es. registro R5, registro R2) all'interno di campi specifici e non contigui dell'istruzione binaria: 
 * i bit del registro sorgente potrebbero dover andare nei bit 16-20 dell'istruzione, mentre i bit del registro destinazione 
 * nei bit 11-15.Questa funzione distribuisce i bit della sorgente nei campi designati dell'istruzione finale.
 * - Digital Signal Processing (DSP): Utilizzata per il "bit-packing" di flussi dati 
 * compressi dove i campi non sono allineati su byte o word, ma su posizioni arbitrarie 
 * definite da protocolli di trasmissione specifici.
 * - Hardware Design: Simulazione di routing su bus dove i segnali devono essere 
 * instradati da un registro sorgente a una matrice di interconnessione non lineare.
 *
 * LOGICA DI ROUTING:
 * 1. GATHER (Isolamento): Estrazione sequenziale dei bit di n.
 * 2. ROUTING (Traslazione): Spostamento del bit isolato verso l'indice di destinazione.
 * 3. MERGE (Accumulo): Unione del bit nel registro di output tramite OR bitwise.
 * =====================================================================================
 */

#include <stdio.h>

unsigned int bit_deposit_scattered(unsigned int n, unsigned int *positions, int count) 
{
    unsigned int res = 0;
    
    for (int i = 0; i < count; i++) 
    {
        unsigned int bit = (n >> i) & 1;
        unsigned int destination = positions[i] & 31;;
        
        
        res |= (bit << destination);
    }
    
    return res;
}

int main(void) 
{
    // Esempio: Sorgente 0b1011 (11), posizioni {0, 2, 4, 6}
    // Risultato atteso: Bit 0->0, 1->2, 2->4, 3->6
    // 0b1011 -> (1<<0) | (1<<2) | (0<<4) | (1<<6) = 1 | 4 | 0 | 64 = 69 (0b1000101)
    unsigned int n = 0b1011;
    unsigned int positions[] = {0, 2, 4, 6};
    
    printf("Risultato scatter: %u\n", bit_deposit_scattered(n, positions, 4));
    
    return 0;
}