/*
 * =====================================================================================
 * GRAY_CODE_BASE.c - CONVERSIONE BINARIO-GRAY A 8 BIT
 * =====================================================================================
 * 
 * 1. CONTESTO TEORICO:
 * Il codice Gray è un sistema numerico binario non posizionale a "distanza unitaria" 
 * (Distanza di Hamming = 1). Tra due numeri consecutivi cambia sempre e solo un bit.
 * 
 * La formula matematica applicata per ogni bit i è: Gi = Bi ^ Bi+1(dove Bi+1 e' il bit alla
 * sinistra di Bi)che risolve la conversione dal binario naturale B a codice Gray G bit a bit 
 * in tempo costante O(1).
 * Shiftando il numero a destra di 1 (n >> 1), allineiamo il bit Bi+1 sopra il bit Bi. 
 * L'operatore XOR (^) esegue il calcolo su tutti i bit del registro in parallelo.
 * 
 * Tabella di transizione critica (Esempio 3 -> 4):
 * - Dec 3: Binario 011 -> Gray 010
 * - Dec 4: Binario 100 -> Gray 110 (Cambia solo il bit più a sinistra)
 * 
 * 2. APPLICAZIONI PRATICHE:
 * - FIFO Asincrone e Clock Domain Crossing (CDC): Nei chip moderni(ASIC/FPGA),spesso hai una memoria 
 *   FIFO che fa da ponte tra due circuiti che girano a frequenze di clock diverse (es. uno a 100MHz e uno a 400MHz). 
 *   Per sapere se la FIFO è piena o vuota, devi passare i puntatori di lettura/scrittura da un clock domain all'altro. 
 *   Se passi un puntatore binario, i bit che cambiano contemporaneamente possono essere campionati in momenti leggermente 
 *   diversi dal secondo clock, causando metastabilità e corruzione dei dati. Il codice Gray serve a passare 
 *   i puntatori di lettura/scrittura tra due circuiti che girano a frequenze di clock 
 *   diverse senza causare metastabilità nei flip-flop:un solo bit cambia per ciclo: il clock di destinazione leggerà o 
 *   il vecchio valore o il nuovo, senza ambiguità.
 * - Encoder Rotativi e Ottici:Immagina un disco rotante collegato all'albero di un motore.
 *   Il disco ha delle tracce lette da sensori ottici. Se il disco si trova esattamente sulla 
 *   linea di transizione tra il valore 3 (011) e il valore 4 (100), i tre sensori non scatteranno 
 *   mai nello stesso identico picosecondo a causa di tolleranze meccaniche. Il codice Gray previene 
 *   la lettura di stati fantasma durante le fasi di transizione meccanica sui dischi di posizionamento 
 *   industriale. Se usi il binario il sensore potrebbe leggere temporaneamente stati fantasma come 111 (7) o 
 *   000 (0). Il software impazzisce, credendo che il motore abbia fatto un salto quantico.
 *   Se usi il Gray, poiché tra 3 (010) e 4 (110) cambia solo il primo bit, l'incertezza fisica della transizione 
 *   può produrre solo o 010 o 110. Nessun valore fantasma. Il sistema rimane stabile.
 * 
 * 3. IMPLEMENTAZIONE HARDWARE (SILICTOR):
 * Questo codice si traduce in puro hardware combinatorio a zero cicli di clock.
 * - Lo shift a destra (>> 1) non consuma transistor: è un semplice re-wiring dei cavi(vengono
 *   saldati sfasati di una posizione).
 * - Per un registro a 8 bit (unsigned char), servono esattamente 7 porte logiche XOR in parallelo.
 * - Critical Path (Percorso critico): Il segnale attraversa un solo strato di porte 
 *   logiche, rendendo l'operazione istantanea con un ritardo pari a un singolo T_xor,cioe'
 *   al tempo di commutazione di un singolo XOR.
 * =====================================================================================
 */

#include <stdio.h>

/**
 * @brief Converte un byte da binario naturale a codice Gray.
 * @param n Il valore a 8 bit da convertire.
 * @return unsigned char Il codice Gray corrispondente.
 */
unsigned char gray_code(unsigned char n)
{
    return n ^ (n >> 1);
}

int main()
{
    unsigned char i;
    unsigned char g;

    printf("Decimale | Binario  | Codice Gray\n");
    printf("---------------------------------\n");
    
    for (i = 0; i < 8; i++)
    {
        g = gray_code(i);
        
        // Stampa formattata per mostrare la transizione a singolo bit
        printf("%8d | %c%c%c      | %c%c%c\n", 
               i,
               (i & 4) ? '1' : '0', (i & 2) ? '1' : '0', (i & 1) ? '1' : '0',
               (g & 4) ? '1' : '0', (g & 2) ? '1' : '0', (g & 1) ? '1' : '0');
    }

    return (0);
}