/*******************************************************************************
 * NEXT SET BIT (BITSCAN FORWARD FROM OFFSET) - TEORIA, SWAR ED HARDWARE
 *******************************************************************************
 * 
 * 1. DIMOSTRAZIONE TEORICA
 * 
 * Dato un intero senza segno 'n' a 32 bit e un offset 'pos':
 * 
 * Step 1: Shift a destra (n >>= pos)
 *   Elimina i primi 'pos' bit. Il bit che prima era in posizione 'pos' 
 *   ora si trova in posizione 0.
 * 
 * Step 2: Isolamento del LSB (n & -n)
 *   In complemento a due, -n equivale a (~n + 1). L'operazione (n & -n) 
 *   azzera tutti i bit tranne il bit acceso piu' basso.
 *   Risultato: una potenza di due pura 2^k, dove k e' l'offset relativo a 'pos'.
 * 
 * Step 3: Ripristino offset originale (((n & -n)) << pos)
 *   Riporta la potenza di due nella sua posizione originale: 
 *   2^k * 2^pos = 2^(k + pos) = 2^m.
 * 
 * Step 4: Maschera di scomposizione (2^m - 1)
 *   Per qualsiasi potenza di due 2^m, il valore (2^m - 1) produce un numero 
 *   in cui i bit da 0 a m-1 sono tutti impostati a 1 e dal bit m in poi a 0.
 *   Esempio per m = 4 (2^4 = 16 = 0b10000):
 *   16 - 1 = 15 = 0b001111 (4 bit accesi).
 * 
 * Step 5: Contatore di bit (popcount)
 *   popcount(2^m - 1) conta quanti bit sono accesi nella maschera, 
 *   restituendo esattamente l'indice 'm'.
 * 
 * -----------------------------------------------------------------------------
 * 2. APPLICAZIONI PRATICHE
 * -----------------------------------------------------------------------------
 * - Bitmap Allocation & Free Lists (Kernel OS):
 *   I sistemi operativi rappresentano lo stato dei blocchi di memoria o dei 
 *   disk cluster come bitmap. Trovare il "prossimo blocco libero" a partire 
 *   dall'ultimo allocato richiede esattamente questa operazione in O(1).
 * 
 * - Priority Schedulers:
 *   Nei RTOS (Real-Time Operating System), i task pronti all'esecuzione sono 
 *   indicizzati in una bitmask in base alla priorita'. next_set_bit permette 
 *   di iterare rapidamente solo sui task attivi saltando le priorita' vuote.
 * 
 * - Garbage Collection (Mark & Sweep):
 *   Pointers/Objects traversal in macchine virtuali (es. V8, JVM) per 
 *   scansionare le regioni di memoria marcate durante la fase di sweep.
 * 
 * -----------------------------------------------------------------------------
 * 3. TRANSLATION ASSEMBLY ED IMPLEMENTAZIONI HARDWARE
 * -----------------------------------------------------------------------------
 * In C standard puro usiamo SWAR + Popcount. Tuttavia, sui processori moderni 
 * questa operazione mappa direttamente su singole istruzioni hardware:
 * 
 * - x86_64: 
 *   Se la CPU supporta BMI1 (Bit Manipulation Instruction Set 1):
 *     1. ANDN + SHLX per la maschera.
 *     2. TZCNT (Trailing Zero Count) calcola 'm' direttamente in 1 ciclo di clock 
 *        senza passare per popcount(2^m - 1).
 *   Sulle CPU x86 piu' vecchie si usava BSF (Bit Scan Forward).
 * 
 * - ARM64:
 *   Combina RBIT (Reverse Bits) e CLZ (Count Leading Zeros) per ottenere 
 *   il Trailing Zero Count in 2 cicli senza branch.
 * 
 * - GCC/Clang Intrinsics:
 *   Nei compilatori di produzione, questa funzione si implementa con:
 *     unsigned int masked = n & (~0U << pos);
 *     return masked ? __builtin_ctz(masked) : -1;
 *******************************************************************************/

#include <stdio.h>
#include "./popcount_swar32.c"
int next_set_bit(unsigned int n, int pos) {
    // Controllo dei limiti per evitare Undefined Behavior o wrap con pos >= 32
    if ((unsigned int)pos >= 32)
        return -1;

    n >>= pos;
    if (!n)
        return -1;

    unsigned int lsb = (n & (-n)) << pos;
    return popcount_swar32(lsb - 1);
}

// ============================================================================
// VARIANTE HARDWARE NATIVA (In produzione con built-in del compilatore)
// ============================================================================
int next_set_bit_hw(unsigned int n, int pos) {
    if ((unsigned int)pos >= 32)
        return -1;

    // Genera una maschera con 0 nei bit < pos e 1 nei bit >= pos
    unsigned int mask = ~0U << pos;
    unsigned int masked = n & mask;

    if (!masked)
        return -1;

    // __builtin_ctz (Count Trailing Zeros) usa direttamente TZCNT/BSF della CPU
    return __builtin_ctz(masked);
}

// ============================================================================
// MAIN DI TEST
// ============================================================================
int main(void) {
    // n = 20 (Binario: 0000 0000 0000 0000 0000 0000 0001 0100) -> Bit accesi: 2 e 4
    unsigned int n = 20;

    printf("--- TEST CON N = 20 (Bit 2 e Bit 4 accesi) ---\n");
    printf("pos = 1 -> %d (Atteso: 2)\n", next_set_bit(n, 1));
    printf("pos = 2 -> %d (Atteso: 2)\n", next_set_bit(n, 2));
    printf("pos = 3 -> %d (Atteso: 4)\n", next_set_bit(n, 3));
    printf("pos = 4 -> %d (Atteso: 4)\n", next_set_bit(n, 4));
    printf("pos = 5 -> %d (Atteso: -1)\n\n", next_set_bit(n, 5));

    printf("--- TEST CASI LIMITE ---\n");
    // Bit 31 acceso (0x80000000)
    unsigned int msb_only = 0x80000000;
    printf("MSB (0x80000000), pos = 0  -> %d (Atteso: 31)\n", next_set_bit(msb_only, 0));
    printf("MSB (0x80000000), pos = 31 -> %d (Atteso: 31)\n", next_set_bit(msb_only, 31));
    printf("Out of bounds,     pos = 32 -> %d (Atteso: -1)\n", next_set_bit(n, 32));
    printf("Valore zero (n=0), pos = 0  -> %d (Atteso: -1)\n\n", next_set_bit(0, 0));

    printf("--- CONFRONTO CON HARDWARE INTRINSIC (__builtin_ctz) ---\n");
    printf("SWAR(20, 1): %d | HW(20, 1): %d\n", next_set_bit(20, 1), next_set_bit_hw(20, 1));
    printf("SWAR(20, 3): %d | HW(20, 3): %d\n", next_set_bit(20, 3), next_set_bit_hw(20, 3));

    return 0;
}