/* ========================================================================== */
/* MASK_BETWEEN.C - GENERAZIONE MASCHERE DI BIT SICURE (ZERO UB)              */
/* 
/* ========================================================================== */

#include <stdio.h>

/* -------------------------------------------------------------------------- */
/* 1. BACKGROUND TEORICO: IL PROBLEMA DEI SPOSTAMENTI FUORI SCALA             */
/* 
/* Nello standard del linguaggio C, shiftare un intero a 32 bit di 32 o più   */
/* posizioni è un Undefined Behavior (UB). Le CPU x86 e ARM applicano un      */
/* modulo 32 implicito sulla quantità di shift, quindi uno shift di 32        */
/* diventa uno shift di 0, lasciando il valore invariato e distruggendo la    */
/* logica delle maschere.                                                     */
/* 
/* Per aggirare il problema, l'espressione strutturata come:                  */
/* mask = ((1U << (high - low)) << 1) - 1                                     */
/* Sfrutta le regole matematiche dei tipi unsigned. Poiché low e high sono    */
/* validati tra 0 e 31, la differenza massima (high - low) è 31.              */
/* Lo shift (1U << 31) è totalmente legale. Il secondo shift automatico       */
/* (<< 1) spinge l'unico bit acceso oltre il limite dei 32 bit. Nei tipi      */
/* unsigned questo comportamento è definito come un'operazione di modulo      */
/* 2^32, che azzera il registro. Sottraendo 1 a zero, per l'underflow dei     */
/* numeri unsigned, si ottiene istantaneamente 0xFFFFFFFF (tutti i bit a 1).  */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* 2. APPLICAZIONI PRATICHE: COSTRUZIONE DI DRIVER HARDWARE                  */
/* 
/* Nei sistemi embedded e nello sviluppo di kernel (es. Linux Driver), i      */
/* registri hardware dei chip di periferica (GPIO, antenne Wi-Fi, controller  */
/* di rete) sono composti da un singolo blocco a 32 bit diviso in sottocampi.  */
/* */
/* Ad esempio, un registro di configurazione potrebbe avere:                  */
/* - Bit 0-2: Velocità di clock                                               */
/* - Bit 3-7: Modalità operativa (I2C, SPI, UART)                             */
/* - Bit 8-31: Indirizzo di memoria buffer                                    */
/* */
/* Per scrivere o leggere la "Modalità operativa" senza sporcare gli altri bit*/
/* del chip, il driver deve isolare l'area di interesse. La funzione          */
/* mask_between genera la maschera perfetta (0xF8 nel caso dei bit 3-7) per   */
/* ripulire il registro tramite un'operazione AND prima di iniettare il       */
/* nuovo comando.                                                             */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* 3. IMPLEMENTAZIONE HARDWARE                                               */
/* */
/* Nel processore, la funzione mask_between non viene eseguita tramite cicli  */
/* ma viene ridotta a pochissime istruzioni elementari grazie al Barrel       */
/* Shifter (un modulo interno della ALU in grado di ruotare o spostare i bit  */
/* in un singolo ciclo di clock).                                             */
/* */
/* - Sottrazione: Viene calcolato lo scarto (high - low).                     */
/* - Primo Shift: Il bit 1 viene spostato nel Barrel Shifter della quantità   */
/* calcolata.                                                               */
/* - Secondo Shift: Il registro viene fatto scattare di una posizione.        */
/* - Decremento: Un'unità di calcolo sottrae 1, invertendo tutti i bit a      */
/* valore inferiore.                                                        */
/* - Allineamento: L'ultimo shift sposta l'intero blocco di bit accesi        */
/* nella posizione "low" corretta richiesta dal registro hardware.          */
/* -------------------------------------------------------------------------- */

unsigned int mask_between(int low, int high) {
    // Validazione dei confini per evitare input distruttivi o impossibili
    if (low < 0 || low >= 32 || high < 0 || high >= 32 || high < low) {
        return 0; // Ritorna 0 se i parametri non hanno senso logico
    }

    // Generazione della maschera protetta da Undefined Behavior:
    // in teoria la lunghezza della maschera dovrebbe essere high - low + 1;
    // ma nel caso di high 31 e low 0,quel calcolo porterebbe ad UB
    // per uno shift << 32. Separando il + 1 come secondo shift << 1
    // non e' possibile piu' generare UB nemmeno in quell'edge case
    unsigned int mask = ((1U << (high - low)) << 1) - 1;
    
    // Sposta la maschera creata nella posizione di partenza richiesta
    return mask << low;
}

int main(void) {
    // Test 1: Caso standard (Bit da 3 a 7) -> Atteso: 0xF8 (0b11111000)
    printf("Maschera 3-7:   0x%X\n", mask_between(3, 7));

    // Test 2: Caso limite inferiore (Bit 0) -> Atteso: 0x1
    printf("Maschera 0-0:   0x%X\n", mask_between(0, 0));

    // Test 3: Caso limite critico (Tutto il registro 0-31) -> Atteso: 0xFFFFFFFF
    printf("Maschera 0-31:  0x%X\n", mask_between(0, 31));

    return 0;
}