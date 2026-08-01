/*******************************************************************************
 * KRONECKER DELTA BITMASK - TEORIA, MASCHERE DI SELEZIONE ED HARDWARE
 *******************************************************************************
 * 
 * 1. BACKGROUND TEORICO E DELTA DI KRONECKER
 * 
 * In matematica, il Delta di Kronecker δ(a, b) è una funzione a due variabili:
 * - δ(a, b) = 1  se a == b
 * - δ(a, b) = 0  se a != b
 * 
 * In ambito informatico low-level e SWAR, l'obiettivo è trasformare questo 
 * valore booleano (0 o 1) in una MASCHERA DI BIT (Bitmask):
 * - Maschera TRUE:  0xFFFFFFFF (tutti i bit a 1) se a == b
 * - Maschera FALSE: 0x00000000 (tutti i bit a 0) se a != b
 * 
 * MECCANICA DELLE OPERAZIONI:
 * 1. XOR (a ^ b):
 *    Proprietà dell'XOR: x ^ x = 0.
 *    - Se a == b, tutti i bit coincidono -> il risultato è esattamente 0.
 *    - Se a != b, almeno un bit differisce -> il risultato è un valore != 0.
 * 
 * 2. LOGICAL NOT (!(a ^ b)):
 *    In C, l'operatore logico ! converte 0 in 1, e qualsiasi valore != 0 in 0.
 *    - Se a == b -> !(0) = 1
 *    - Se a != b -> !(non_zero) = 0
 * 
 * 3. NEGAZIONE ARITMETICA IN COMPLEMENTO A DUE (-x):
 *    In rappresentazione a 32 bit unsigned (complemento a due):
 *    - -1 equivale al valore binario 11111111 11111111 11111111 11111111 (0xFFFFFFFF).
 *    - -0 equivale a 00000000 00000000 00000000 00000000 (0x00000000).
 * 
 * Quindi: -(!(a ^ b)) genera istantaneamente la bitmask perfetta in 3 istruzioni.
 * 
 * -----------------------------------------------------------------------------
 * VARIANTI: APPROCCIO 100% BITWISE ARITMETICO (Senza operatore logico '!')
 * -----------------------------------------------------------------------------
 * Se si volesse evitare l'operatore logico '!' per purismo matematico, si può 
 * sfruttare la proprietà del complemento a due e del bit di segno (MSB):
 *     unsigned int diff = a ^ b;
 *     return ~((diff | (~diff + 1)) >> 31);
 * Dove (diff | -diff) imposta il Most Significant Bit (MSB) a 1 per qualsiasi 
 * numero != 0. Lo shift >> 31 porta il bit di segno in prima posizione, e l'NOT (~) 
 * inverte il risultato per ottenere 0xFFFFFFFF per 0 e 0x00000000 per non-zero.
 * 
 * -----------------------------------------------------------------------------
 * 2. APPLICAZIONI PRATICHE
 * -----------------------------------------------------------------------------
 * - Multiplexing Branchless (Conditional Select):
 *   Senza usare 'if', per fare l'equivalente di:
 *     int result = (a == b) ? val_true : val_false;
 *   Usiamo la maschera generata:
 *     unsigned int mask = kronecker_delta_bits(a, b);
 *     int result = (val_true & mask) | (val_false & ~mask);
 *   Questo seleziona uno dei due valori in tempo costante senza subire il 
 *   costo di branch mispredictions nella pipeline della CPU.
 * 
 * - Crittografia a Tempo Costante (Constant-Time Execution):
 *   Nelle librerie crittografiche (es. OpenSSL, BoringSSL), i confronti tra 
 *   chiavi o token non devono MAI usare 'if' o '==', poiché i salti condizionati 
 *   rivelano informazioni sul tempo di esecuzione permettendo attacchi di tipo 
 *   Side-Channel (Timing Attacks). Maschere come questa sono lo standard.
 * 
 * - SIMD Vectorization & Shader Blending:
 *   Le istruzioni vettoriali (AVX2/NEON) non eseguono branch per singolo elemento. 
 *   Quando si confrontano due vettori di registri, la CPU genera proprio un 
 *   vettore di maschere (0xFFFFFFFF o 0x00000000) per combinare i dati tramite 
 *   operazioni AND/OR/BLEND su tutto il registro SIMD.
 * 
 * -----------------------------------------------------------------------------
 * 3. TRADUZIONE ASSEMBLY HARDWARE
 * -----------------------------------------------------------------------------
 * I compilatori moderni (GCC/Clang con -O2/-O3) traducono '-(!(a ^ b))' in sequenze 
 * prive di salti condizionati (nessun jmp/jne).
 * 
 * Generazione tipica x86_64:
 *   xor edi, esi       ; edi = a ^ b (0 se uguali, !=0 altrimenti)
 *   sub edi, 1         ; se edi era 0, va in underflow a 0xFFFFFFFF (setta Carry Flag)
 *   sbb eax, eax       ; sbb (subtract with borrow): eax = 0 - 0 - CF -> 0xFFFFFFFF o 0
 *   ret
 * 
 * Oppure tramite SETcc / NEG:
 *   cmp edi, esi       ; confronta a e b
 *   sete al            ; al = 1 se a == b, al = 0 altrimenti
 *   movzx eax, al      ; estende a 32 bit (0x00000001 o 0x00000000)
 *   neg eax            ; 0 -> 0x00000000, 1 -> 0xFFFFFFFF (-1)
 *   ret
 *******************************************************************************/

#include <stdio.h>

// ============================================================================
// IMPLEMENTAZIONE MIA: Idiomatica, veloce e branchless
// ============================================================================
unsigned int kronecker_delta_bits(unsigned int a, unsigned int b) {
    return -(!(a ^ b));
}

// ============================================================================
// VARIANTE PURE BITWISE: Senza l'operatore logico !
// ============================================================================
unsigned int kronecker_delta_bits_pure_bitwise(unsigned int a, unsigned int b) {
    unsigned int diff = a ^ b;
    // (diff | (~diff + 1)) imposta il bit 31 a 1 se diff != 0
    // >> 31 isola il bit 31 (diventa 1 per !=0, 0 per ==0)
    // ~ inverte tutto (0x0 per !=0, 0xFFFFFFFF per ==0)
    return ~((diff | (~diff + 1)) >> 31);
}

// ============================================================================
// ESEMPIO PRATICO: Branchless Selection (Multiplexer)
// ============================================================================
int branchless_select(unsigned int a, unsigned int b, int val_if_equal, int val_if_different) {
    unsigned int mask = kronecker_delta_bits(a, b);
    return (val_if_equal & mask) | (val_if_different & ~mask);
}

int main(void) {
    printf("--- TEST KRONECKER DELTA BITMASK ---\n");
    printf("Matching (5, 5): %#X\n", kronecker_delta_bits(5, 5)); // 0xFFFFFFFF
    printf("Mismatch (5, 6): %#X\n\n", kronecker_delta_bits(5, 6)); // 0x00000000

    printf("--- TEST VARIANTE PURE BITWISE ---\n");
    printf("Matching (5, 5): %#X\n", kronecker_delta_bits_pure_bitwise(5, 5)); // 0xFFFFFFFF
    printf("Mismatch (5, 6): %#X\n\n", kronecker_delta_bits_pure_bitwise(5, 6)); // 0x00000000

    printf("--- TEST MULTIPLEXER BRANCHLESS ---\n");
    printf("Select (5 == 5 ? 100 : 200) -> %d\n", branchless_select(5, 5, 100, 200)); // 100
    printf("Select (5 == 6 ? 100 : 200) -> %d\n", branchless_select(5, 6, 100, 200)); // 200

    return 0;
}