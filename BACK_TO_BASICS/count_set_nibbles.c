#include <stdio.h>
#include "./popcount_swar32.c"

/* ========================================================================== */
/*  COUNT_SET_NIBBLES.C - SWAR BIT-SMEARING TECHNIQUE                         */
/*                                                                            */
/* ========================================================================== */
/* -------------------------------------------------------------------------- */
/*  1. BACKGROUND TEORICO & IL FALLIMENTO DEL BORROW RIPPLE                   */
/*  Il problema richiede di determinare quanti blocchi da 4 bit in un intero  */
/*  a 32 bit contengono almeno un bit impostato a 1.                          */
/*  L'algoritmo classico di AMD/Hacker's Delight per trovare byte nulli       */ 
/*  sfrutta la sottrazione: Maschera = (x - 0x01010101) & ~x & 0x80808080.    */
/*  Questo metodo si basa sul presupposto che se un byte è 0x00,l'operazione  */ 
/*  0x00 - 0x01 genera un prestito (borrow) che si propaga verso l'alto fino  */ 
/*  al bit più significativo del byte stesso (il bit di segno 0x80).          */ 
/*  Quando si tenta di adattare questa tecnica a strutture più strette come i */
/*  nibble (0x11111111 e 0x88888888), l'algoritmo fallisce a causa della      */
/*  contaminazione inter-nibble. Avendo solo 4 bit a disposizione,il prestito */
/*  generato da un nibble nullo non si ferma all'interno del nibble stesso,   */
/*  ma scala immediatamente nel nibble adiacente a sinistra.                  */              
/*                                                                            */
/*  LA SOLUZIONE: Bit-Smearing parallelo interno al nibble.                   */
/*  Invece di sottrarre, "allarghiamo" i bit all'interno di ogni singolo      */
/*  nibble. Se un nibble contiene almeno un bit a 1, facciamo in modo che il  */
/*  suo bit meno significativo (LSB) diventi 1, senza mai toccare i vicini.   */
/*                                                                            */
/*  Passaggi logici sullo stesso registro:                                    */
/*  1. n |= (n >> 1);  -> Ogni coppia di bit riflette la presenza di un 1     */
/*  2. n |= (n >> 2);  -> L'LSB di ogni nibble ora contiene la OR di tutti    */
/*                        e 4 i bit originali del nibble stesso.              */
/*  3. n &= 0x11111111 -> Isoliàmo gli 8 LSB (uno per ogni nibble).           */
/*  4. Popcount finale -> Conta quanti LSB sono rimasti accesi.               */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*  2. APPLICAZIONI PRATICHE NEL SOFTWARE DI SISTEMA                          */
/*                                                                            */
/*  - BIOINFORMATICA:I nucleotidi ACGT sono spesso codificati in formato      */
/*    compresso 4bit(2 basi per byte).Contare i nibble set consente un        */
/*    calcolo istantaneo della lunghezza di sequenze DNA lette da un binario  */ 
/*    ignorando il padding zero.                                              */
/*  - DB COLONNARI / RANK & SELECT: Nei motori di ricerca e db colonnari,     */
/*    i vettori di bit compressi utilizzano indici di campionamento a blocchi.*/
/*    L'operazione di Rank(quanti elementi validi ci sono fino alla posizione */
/*    X)viene accellerata elaborando vettori di nibble tramite SWAR senza     */
/*    toccare la RAM e con zero branch mispredictions.                        */
/*  - GARBAGE COLLECTION: Molti allocatori di memoria avanzati gestiscono     */
/*    i metadati dei blocchi allocati (status, chunk size, marcature GC)      */
/*    usando token da 4 bit. Scansionare le bitmap di metadati con questa     */
/*    tecnica permette al GC di identificare le pagine popolate con zero      */
/*    branch mispredictions.                                                  */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*  3. IMPLEMENTAZIONE HARDWARE (MICROARCHITETTURA ASIC/FPGA)                 */
/*                                                                            */
/*  In silicio questa logica è ridicolmente economica. Non richiede un        */
/*  sommatore/sottrattore completo (niente logica di Carry/Borrow Lookahead). */
/*  - Lo smearing (OR e Shift) si traduce in sole 32 porte OR a livello       */
/*    combinatorio puro, pilotate da routing fisico (fili sfasati).           */
/*  - La maschera 0x11111111 non usa porte: i fili dei bit non LSB vengono    */
/*    collegati direttamente a massa (GND).                                   */
/*  - Il popcount finale è un semplice albero di compressione di Wallace      */
/*    (Full/Half Adders) profondo solo 3 stadi logici.                        */
/* -------------------------------------------------------------------------- */
unsigned char count_set_nibbles(unsigned int n){
    // Ors sequenziali per fare in modo che se un nibble ha ALMENO un bit a 1,
    // il suo bit meno significativo (LSB) diventi 1.
    n |= (n >> 1);
    n |= (n >> 2);
    // Isoliamo solo l'LSB di ogni nibble (bit 0, 4, 8, 12, 16, 20, 24, 28)
    n &= 0x11111111;
    // Il popcount ora conterà esattamente quanti LSB sono rimasti accesi
    return popcount_swar32(n);
}

int main() {
    printf("%u\n", count_set_nibbles(0x00000010)); // 1
}