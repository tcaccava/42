/*
 * =====================================================================================
 * GRAY_CODE_REFLECTED - CONVERSIONE E APPLICAZIONI IN SISTEMI DI FEEDBACK
 * =====================================================================================
 *
 * BACKGROUND TEORICO:
 * Il Codice Gray è un sistema numerico binario in cui due valori successivi differiscono 
 * per un solo bit. A differenza del binario naturale (dove il passaggio da 3 a 4, 
 * ovvero 011 -> 100, comporta la commutazione di 3 bit), il codice Gray garantisce che 
 * la transizione tra due stati adiacenti sia "atomica" a livello di singolo bit.
 * La variante "riflessa" è il metodo standard di costruzione: si specchia una sequenza 
 * esistente e si inverte il bit di ordine superiore, rendendo il codice ciclicamente 
 * continuo anche nel passaggio dall'ultimo al primo valore.
 *
 * PERCHÉ SI USA (IL PROBLEMA DELLE TRANSIZIONI):
 * In sistemi fisici (encoder ottici, magnetici o capacitivi), il posizionamento non è 
 * mai istantaneo. Se un sensore legge un valore binario naturale durante una transizione 
 * rapida, a causa di micro-disallineamenti meccanici, potrebbe leggere stati intermedi 
 * errati (es. passare da 011 a 100 potrebbe causare letture temporanee di 000 o 111). 
 * Questo genera "glitch" o rumore che portano il sistema di controllo a posizioni errate.
 * Il codice Gray elimina questo errore perché cambia solo un'informazione alla volta: 
 * non esistono stati intermedi ambigui.
 *
 * APPLICAZIONI PRATICHE:
 * - Encoder Angolari/Lineari: Utilizzati in CNC, robotica e motori brushless per la 
 * retroazione di posizione (resolver).
 * - Sistemi di Posizionamento: Quando la velocità di variazione del segnale è elevata, 
 * il Gray Code impedisce al controller di interpretare il rumore come posizione.
 * - Clock Domain Crossing (CDC): Nelle FPGA, il Gray code viene usato per passare 
 * contatori tra domini di clock diversi, evitando problemi di metastabilità.
 *
 * IMPLEMENTAZIONI HARDWARE:
 * In hardware, la conversione avviene tramite una rete combinatoria di porte XOR. 
 * Il segnale entra in parallelo e la conversione avviene con un ritardo di propagazione 
 * pari a quello di una singola porta logica (a differenza del contatore binario 
 * che richiede catene di riporto - carry chain).
 * =====================================================================================
 */

#include <stdio.h>

/**
 * @brief Genera il codice Gray riflesso su un numero limitato di bit.
 * @param n Il numero binario da convertire.
 * @param bits La larghezza del bus fisico (es. 8, 10, 16 bit).
 * @return Il valore convertito in Gray Code, troncato alla dimensione del bus.
 */
unsigned int bits_to_gray_reflect(unsigned int n, int bits)
{
    /* * Logica di conversione: n XOR (n >> 1)
     * La maschera (1U << bits) - 1 garantisce che il risultato rimanga entro
     * i confini dell'encoder, eliminando bit spuri derivanti dalla riflessione 
     * su un numero di bit non nativo per il registro.
     */
    unsigned int gray = n ^ (n >> 1);
    unsigned int mask = (1U << bits) - 1;
    
    return gray & mask;
}

int main(void)
{
    // Esempio: Encoder a 4 bit
    int bits = 4;
    printf("Binario -> Gray (Riflesso) su %d bit:\n", bits);
    for (unsigned int i = 0; i < (1U << bits); i++)
    {
        printf("Binario: %02u | Gray: %02u\n", i, bits_to_gray_reflect(i, bits));
    }
    return 0;
}