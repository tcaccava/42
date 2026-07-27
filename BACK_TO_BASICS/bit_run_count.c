/*******************************************************************************
 * TEORIA, APPLICAZIONI E IMPLEMENTAZIONI HARDWARE DEL CONTEGGIO RUN (BIT RUNS)
 *******************************************************************************
 * 
 * 1. BACKGROUND TEORICO
 * 
 * La relazione tra transizioni di bit (cambi di stato tra 0 e 1) e sequenze 
 * contigue di bit uguali (run) e' regolata dal principio combinatorio noto come 
 * "Fencepost Problem" (il problema dei paletti e delle staccionate).
 * 
 * - Definizione di Transizione (T): Un cambio di valore tra due bit adiacenti 
 *   b_i e b_(i+1) lungo il registro.
 * - Definizione di Run (R): Un blocco contiguo di uno o piu' bit aventi lo 
 *   stesso identico valore (es. 111 oppure 00).
 * 
 * Ogni transizione T agisce come un punto di taglio (un paletto) che suddivide 
 * la sequenza binaria complessiva in regioni omogenee distinte. Di conseguenza, 
 * il numero totale di run R in un registro e' sempre dato da:
 * 
 *                           R = T + 1
 * 
 * Calcolo ottimizzato di T:
 * Invece di scorrere la variabile bit per bit con un ciclo, le transizioni 
 * tra bit adiacenti possono essere individuate in parallelo tramite l'operatore 
 * XOR: il valore (n XOR (n >> 1)) presenta un bit a 1 esattamente in corrispondenza 
 * di ogni cambio di stato. Il numero totale di transizioni T e' quindi pari al 
 * conteggio dei bit accesi (popcount) di questo valore mascherato.
 * 
 * Gestione degli estremi e sicurezza aritmetica:
 * Quando n = 0 (tutti zeri) oppure n = 0xFFFFFFFF (tutti uno), non esiste alcuna 
 * transizione (T = 0). Applicando la formula R = T + 1 si ottiene R = 1, che 
 * rappresenta correttamente l'unico grande run da 32 bit. L'uso dell'addizione 
 * previene l'underflow a UINT_MAX che si verificherebbe tentando di sottrarre 1.
 * 
 * -----------------------------------------------------------------------------
 * 2. APPLICAZIONI PRATICHE
 * -----------------------------------------------------------------------------
 * - Compressione RLE (Run-Length Encoding) e Pre-Analisi: Prima di comprimere 
 *   flussi binari, bit_run_count viene usata per calcolare l'entropia del blocco. 
 *   Se R e' molto vicino alla dimensione N dei bit totali, la compressione RLE 
 *   espanderebbe i dati invece di comprimerli, consentendo al sistema di saltare 
 *   l'elaborazione o cambiare algoritmo al volo.
 * - Test di Casualita per TRNG e PRNG (NIST SP 800-22): Il "Runs Test" e' uno 
 *   dei test statistici fondamentali per verificare la bonta di un generatore 
 *   di numeri casuali. Un numero di run significativamente diverso dal valore 
 *   atteso N/2 indica la presenza di bias deterministici o disturbi periodici.
 * - Line Coding e Sincronizzazione nei Bus (CAN-bus, USB, HDLC): Nelle 
 *   comunicazioni seriali senza canale di clock dedicato, sequenze di bit 
 *   uguali troppo lunghe (run elevati) causano la perdita di sincronizzazione 
 *   del ricevitore (clock drift). La misurazione dei run permette di pilotare 
 *   le logiche di bit stuffing.
 * 
 * -----------------------------------------------------------------------------
 * 3. IMPLEMENTAZIONI HARDWARE
 * -----------------------------------------------------------------------------
 * - Istruzione POPCOUNT e XOR in ALU/CPU: Nelle moderne architetture x86_64 e 
 *   ARM64, l'intera operazione si traduce in pochissime istruzioni hardware: 
 *   uno shift a destra, una XOR, una maschera AND, l'istruzione hardware 
 *   popcount (es. POPCNT su x86 o CNT su ARM) e un incremento di 1 nell'accumulatore.
 * - Reti Combinatorie Parallele su FPGA/ASIC: Un banco di N-1 porte XOR a 2 
 *   ingressi confronta in parallelo le coppie di bit adiacenti (b_i, b_(i+1)). 
 *   Le uscite delle porte XOR entrano in un albero di sommatori parziali (Adder Tree) 
 *   che calcola T a bassissima latenza logica. Un sommatore finale aggiunge 1 
 *   per produrre il conteggio dei run R.
 * - Circuiti di Detector di Transizione nei Sensori CMOS: Nei sensori d'immagine 
 *   event-driven, la variazione di stato dei pixel viene rilevata via hardware 
 *   tramite latch differenziali per identificare istantaneamente il numero di 
 *   regioni omogenee (run) presenti in ciascuna riga di scanzione.
 *******************************************************************************/

#include <stdio.h>
#include "count_bit_transitions.c"

// Conta il numero totale di sequenze contigue di bit uguali (run) in n.
// Sfrutta la relazione matematica R = T + 1.
unsigned int bit_run_count(unsigned int n)
{
    return count_bits_transitions32(n) + 1;
}

int main(void)
{
    // Test 1: n = 0b11001101 (considerato su 32 bit)
    // Su 32 bit ha 24 zeri iniziali (1 run) + i run del byte finale.
    unsigned int val1 = 0b11001101U;
    printf("Valore: 0b11001101 (32-bit)\n");
    printf("Transizioni: %u\n", count_bits_transitions32(val1));
    printf("Run totali: %u\n\n", bit_run_count(val1));

    // Test 2: n = 0 (tutti zeri -> 0 transizioni, 1 unico grande run)
    unsigned int val2 = 0U;
    printf("Valore: 0 (tutti zeri)\n");
    printf("Transizioni: %u\n", count_bits_transitions32(val2));
    printf("Run totali: %u\n\n", bit_run_count(val2));

    // Test 3: n = 0xAAAAAAAA (bit alternati 101010... -> 31 transizioni, 32 run)
    unsigned int val3 = 0xAAAAAAAAU;
    printf("Valore: 0xAAAAAAAA (bit alternati)\n");
    printf("Transizioni: %u\n", count_bits_transitions32(val3));
    printf("Run totali: %u\n", bit_run_count(val3));

    return 0;
}