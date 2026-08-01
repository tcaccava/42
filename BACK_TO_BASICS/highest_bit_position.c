/*******************************************************************************
 * HIGHEST BIT POSITION (MSB) E FLOOR(LOG2(N)) - TEORIA E OTTIMIZZAZIONI
 *******************************************************************************
 * 
 * 1. BACKGROUND TEORICO E IL RAPPORTO CON FLOOR(LOG2(N))
 * 
 * Trovare la posizione del bit più significativo (Most Significant Bit - MSB) 
 * acceso in un intero equivale matematicamente a calcolare il logaritmo in base 
 * 2 del numero, arrotondato per difetto: floor(log2(n)).
 * 
 * - Il log2(n) risponde alla domanda: "A quale esponente devo elevare 2 per 
 *   ottenere n?".
 * - Nei numeri binari, la posizione di un bit (contando da 0) rappresenta 
 *   esattamente quell'esponente (es. il bit in posizione 4 vale 2^4 = 16).
 * - Quando un numero non è una potenza perfetta di 2 (es. 17), il suo MSB 
 *   rimane quello della potenza di 2 immediatamente inferiore (16). Questo 
 *   corrisponde al troncamento dei decimali (floor) del logaritmo (log2(17) = 
 *   4.08 -> floor -> 4).
 * 
 * Nota matematica: log2(0) tende a meno infinito ed è indefinito. Ecco perché 
 * le implementazioni informatiche gestiscono il caso n=0 con un branch iniziale 
 * che restituisce un codice di errore (es. -1).
 * 
 * -----------------------------------------------------------------------------
 * 2. APPLICAZIONI PRATICHE
 * -----------------------------------------------------------------------------
 * - Buddy Memory Allocation: Il kernel Linux alloca la memoria RAM raggruppando 
 *   le pagine in blocchi potenza di 2. Conoscere il log2() della dimensione 
 *   richiesta permette di trovare all'istante la lista di blocchi corretta.
 * - Mipmapping Grafico: Nei motori 3D (OpenGL/Vulkan), la generazione dei 
 *   livelli di dettaglio di una texture si calcola dimezzando progressivamente 
 *   la risoluzione. Il numero di mipmap è esattamente floor(log2(max(w, h))) + 1.
 * - Hash Tables & Ring Buffers: Trovare la più vicina potenza di 2 superiore o 
 *   inferiore per dimensionare array circolari o tabelle hash e usare maschere 
 *   bitwise (n & (size - 1)) al posto del modulo (%), lentissimo.
 * 
 * -----------------------------------------------------------------------------
 * 3. IMPLEMENTAZIONI HARDWARE
 * -----------------------------------------------------------------------------
 * Le CPU moderne non usano la ALU standard per questa operazione, ma hanno 
 * circuiti digitali dedicati (Priority Encoders) che risolvono in 1 ciclo di clock:
 * - x86_64: Istruzione BSR (Bit Scan Reverse) o la più moderna LZCNT (Leading 
 *   Zero Count), parte delle estensioni BMI1/ABM.
 * - ARM: Istruzione CLZ (Count Leading Zeros).
 * I compilatori espongono questi circuiti tramite funzioni intrinseche (built-in).
 * 
 * -----------------------------------------------------------------------------
 * 4. IL CONFRONTO TRA I TRE APPROCCI
 * -----------------------------------------------------------------------------
 * A. Loop For (Ricerca Lineare - O(N)): 
 *    Usa un ciclo dipendente dai dati. Pessimo per l'hardware moderno. Il 
 *    Branch Predictor fallisce continuamente perché il numero di iterazioni 
 *    varia in base al dato in ingresso, svuotando la pipeline della CPU.
 * 
 * B. SWAR / Branchless (Ricerca Binaria - O(log N)):
 *    Il SOTA per il C ANSI puro. Usa la dicotomia sui bit. Il tempo di 
 *    esecuzione è deterministico (costante). Assenza totale di branch (salto 
 *    condizionato) nel calcolo: la CPU sputa fuori istruzioni bitwise massimizzando 
 *    il throughput.
 * 
 * C. Hardware Intrinsic (O(1)):
 *    Il SOTA assoluto. Si bypassa l'aritmetica C e si interroga direttamente 
 *    il silicio. Inarrivabile per prestazioni, ma rende il codice dipendente 
 *    dal compilatore (GCC/Clang).
 * 
 * -----------------------------------------------------------------------------
 * 5. TRACCIA STEP-BY-STEP (Ricerca Binaria Branchless)
 * -----------------------------------------------------------------------------
 * Esempio con n = 44 (Binario: 0010 1100). Cerchiamo il bit 5. Variabile pos = 0.
 * 
 * STEP 1: n > 15 ? (44 > 15 = VERO = 1).
 *         shift = 1 << 2 = 4.
 *         n >>= 4 (0010 1100 >> 4 diventa 0000 0010, ovvero 2).
 *         pos |= 4 (pos diventa 4).
 * 
 * STEP 2: n > 3 ? (2 > 3 = FALSO = 0). (Ora n vale 2).
 *         shift = 0 << 1 = 0.
 *         n >>= 0 (rimane 0000 0010, ovvero 2).
 *         pos |= 0 (pos resta 4).
 * 
 * STEP 3: pos |= (n >> 1).
 *         2 >> 1 = 1.
 *         pos = 4 | 1 = 5.
 * 
 * RISULTATO: 5. Esatto.
 *******************************************************************************/

#include <stdio.h>

// ============================================================================
// 1. APPROCCIO LINEARE (Loop) - Lento e con branch mispredictions
// ============================================================================
int highest_bit_position(unsigned char n) {
    if (!n) return -1;
    int i = 7;
    for (; !(n & (1 << i)); i--);
    return i;
}

// ============================================================================
// 2. APPROCCIO SWAR (Branchless Binary Search) - SOTA per il C puro
// ============================================================================
int highest_bit_position_branchless(unsigned char n) {
    if (!n) return -1; // Unico branch accettabile per il caso limite

    int pos = 0;
    int shift;

    // 1. Il bit e' nei 4 bit piu' alti (nibble superiore)?
    // (n > 0xF) restituisce 1 se vero, 0 se falso. Shiftato di 2 diventa 4 o 0.
    // Perche' shiftiamo di 4? perche', se il bit e' nel nibble alto, la sua 
    // posizione e' almeno nel quinto bit, cioe' nel bit 4.
    shift = (n > 0xF) << 2; 
    n >>= shift;            // Abbiamo gia' processato il nibble basso, ora ci occupiamo di quello alto: shiftiamo tutto di 4 o 0.
    pos |= shift;           // Aggiorniamo la posizione base (+4)

    // 2. Il bit e' nei 2 bit piu' alti del nibble rimasto?
    shift = (n > 0x3) << 1; 
    n >>= shift;
    pos |= shift;

    // 3. Ultimo bit rimasto
    pos |= (n >> 1);

    return pos;
}
// ============================================================================
// 3. APPROCCIO SWAR (Branchless Binary Search) su un int- SOTA per il C puro
// ============================================================================

int highest_bit_position_branchless_32(unsigned int n) {
    if (!n) return -1; // Unico branch accettabile per il caso limite

    int pos = 0;
    int shift;

    // 1. Spacchiamo a metà i 32 bit: il bit è nei 16 bit più alti?
    // 0xFFFF equivale a 65535 (i 16 bit bassi tutti accesi).
    // Se n > 0xFFFF, shiftiamo di 16 (1 << 4).
    shift = (n > 0xFFFF) << 4; 
    n >>= shift;
    pos |= shift;

    // 2. Spacchiamo a metà i 16 bit rimasti: il bit è negli 8 bit più alti?
    // 0xFF equivale a 255.
    // Se n > 0xFF, shiftiamo di 8 (1 << 3).
    shift = (n > 0xFF) << 3;
    n >>= shift;
    pos |= shift;

    // 3. Spacchiamo a metà gli 8 bit rimasti (n > 15) -> shift di 4
    shift = (n > 0xF) << 2;
    n >>= shift;
    pos |= shift;

    // 4. Spacchiamo a metà i 4 bit rimasti (n > 3) -> shift di 2
    shift = (n > 0x3) << 1;
    n >>= shift;
    pos |= shift;

    // 5. Ultimo bit rimasto
    pos |= (n >> 1);

    return pos;
}

// ============================================================================
// 4. APPROCCIO HARDWARE (Built-in Intrinsics) - SOTA Assoluto
// ============================================================================
int highest_bit_position_hw(unsigned char n) {
    if (!n) return -1;
    
    // __builtin_clz lavora su int a 32 bit. 
    // Conta quanti "zeri iniziali" ci sono partendo dal 31esimo bit verso il basso.
    // Sottraendo questo numero da 31, ottieni esattamente la posizione del bit piu alto!
    return 31 - __builtin_clz(n);
}

// ============================================================================
// ENTRY POINT & TESTS
// ============================================================================
int main(void) {
    unsigned char test1 = 0b01000000; // 64 (Bit 6)
    unsigned char test2 = 0b11001100; // 204 (Bit 7)
    unsigned char test3 = 44;         // 0b00101100 (Bit 5)

    printf("--- TEST LOOP (O(N)) ---\n");
    printf("0b01000000 -> %d\n", highest_bit_position(test1));
    printf("0b11001100 -> %d\n", highest_bit_position(test2));
    printf("44 (0b00101100) -> %d\n\n", highest_bit_position(test3));

    printf("--- TEST BRANCHLESS (O(log N)) ---\n");
    printf("0b01000000 -> %d\n", highest_bit_position_branchless(test1));
    printf("0b11001100 -> %d\n", highest_bit_position_branchless(test2));
    printf("44 (0b00101100) -> %d\n\n", highest_bit_position_branchless(test3));

    printf("--- TEST HARDWARE INTRINSIC (O(1)) ---\n");
    printf("0b01000000 -> %d\n", highest_bit_position_hw(test1));
    printf("0b11001100 -> %d\n", highest_bit_position_hw(test2));
    printf("44 (0b00101100) -> %d\n", highest_bit_position_hw(test3));

    return 0;
}