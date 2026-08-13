#include <stdio.h>
/*******************************************************************************
 * BITFIELD ROTATION 
 *******************************************************************************
 * 
 * 1. BACKGROUND TEORICO E UNDEFINED BEHAVIOR
 * 
 * Ruotare un singolo campo di bit all'interno di un registro (mantenendo 
 * invariato il resto) richiede 4 fasi logiche:
 *   1. Estrazione: Isolare i bit e allinearli a destra (LSB).
 *   2. Rotazione interna: Shift combinati (Sinistra e Destra) per simulare un buffer circolare.
 *   3. Pulizia: Azzzerare il buco originale nel numero di partenza.
 *   4. Merge: Riposizionare il campo ruotato e fonderlo (OR) col resto.
 * 
 * L'insidia del C Standard (C99 §6.5.7):
 * Se il valore dell'operando di destra è negativo o è maggiore o uguale alla 
 * larghezza in bit dell'operando di sinistra, il comportamento è indefinito (UB).
 * Questo significa che shiftare un uint32_t di 32 posizioni (es. val >> 32)
 * NON produce 0. Su x86, l'istruzione SHR maschera l'operando a 5 bit (32 & 31 = 0), 
 * eseguendo di fatto val >> 0 e lasciando il numero invariato!
 * 
 * Per generare in sicurezza una maschera di len bit (es. 0b00001111 per len=4), 
 * invece di fare (1 << len) - 1 (che esplode se len=32), si fa:
 *    ~0U >> (32 - len)
 * Questo popola la maschera da sinistra verso destra in totale sicurezza finché len > 0.
 * 
 * -----------------------------------------------------------------------------
 * 2. APPLICAZIONI PRATICHE
 * -----------------------------------------------------------------------------
 * - Crittografia e Hashing:
 *   Algoritmi come SHA-256, ChaCha20 o i cifrari a blocchi (Simon, Speck) basano 
 *   la loro confusione logica quasi interamente sulle rotazioni circolari 
 *   (ARX: Add-Rotate-Xor). A volte la rotazione avviene su sotto-blocchi (bitfields).
 * 
 * - Gestione di State Machines compresse:
 *   Nei videogiochi o nei protocolli di rete, se hai un contatore circolare 
 *   (es. sequence number da 0 a 7) impacchettato insieme ad altre flag in un 
 *   singolo intero, ruotare il bitfield equivale a scorrere lo stato senza 
 *   toccare la memoria adiacente.
 * 
 * -----------------------------------------------------------------------------
 * 3. HARDWARE ED ASSEMBLY (TRANSLATION)
 * -----------------------------------------------------------------------------
 * Esiste un'istruzione hardware per questo?
 * - No per bitfield arbitrari: I processori (x86, ARM) hanno istruzioni di 
 *   rotazione molto veloci (ROL, ROR, EXTR), ma operano SOLO su registri interi 
 *   (8, 16, 32 o 64 bit). Non esiste un'istruzione "Ruota 5 bit partendo dal bit 12".
 *   Per farlo, la CPU deve eseguire esattamente le istruzioni maschera-shift-OR 
 *   che hai scritto in C.
 * 
 * E negli ASIC/FPGA?
 * - Nello sviluppo hardware (Verilog/VHDL), ruotare bit è "gratis" (0 clock cycles) 
 *   se la rotazione è fissa, perché è solo una questione di fili (routing). 
 *   Se start, len e k sono variabili a runtime, il silicio richiede un 
 *   massiccio "Barrel Shifter", una costosa rete combinatoria di multiplexer.
 *******************************************************************************/

unsigned int bit_rol_field(unsigned int n, int start, int len, int k)
{
    if (len <= 0)// se la lunghezza e' zero ritorno semplicemente n
        return n;
    start &= 31;// start deve essere una posizione valida,quindi da 0 a 31,per cui la calcolo come resto di 32
    // se la somma di len e start e' > 32,dichiaro len = a 32 - start;in caso contrario rimane len
    len = len ^ ((len ^ (32 - start)) & -((len + start) > 32));
    k %= len;// una volta aggiustato len posso aggiustare k come k % len,per evitare inutili cicli:
    // se len fosse 7 e k 50,non ruotero' il field di 50 volte ,ma di 50 % 7,cioe' 1,evitando
    // inutili e costose rotazioni circolari
    if (!k)// se non devo applicare alcuna rotazione,ritorno il numero as is
        return n;
    unsigned int mask = ~0U >> (32 - len);// la maschera sara' 0xFFFFFFFF shiftata a destra di 32 - len
    unsigned int field = (n >> start) & mask;// isolo da n il campo da ruotare
    unsigned int field_rolled = 0;
    field_rolled = (field >> (len - k)) | (field << k);// applico il rol a field,adeguandolo alla lunghezza len
    field_rolled &= mask;// seleziono i bit utili del prodotto della rotazione
    // sposto la maschera e la inverto così che n venga azzerata nella parte corrispondente al field da rollare
    mask <<= start;
    mask = ~mask;
    n &= mask;// 
    return n | (field_rolled << start);// inserisco il field rollato nel numero originale,che ritorno
}

int main()
{
    printf("%#X\n", bit_rol_field(0xFF00F0F, 8, 16, 8));
}