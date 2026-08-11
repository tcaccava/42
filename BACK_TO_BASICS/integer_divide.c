/*******************************************************************************
 * DIVISIONE INTERA BITWISE (BINARY LONG DIVISION) E HARDWARE
 *******************************************************************************
 *
 * 1. BACKGROUND TEORICO: BINARY LONG DIVISION
 *
 * La divisione in base 2 rispecchia esattamente la divisione in colonna che si
 * impara a scuola, ma è immensamente più semplice perché le cifre del quoziente
 * possono essere unicamente 0 oppure 1.
 *
 * Per calcolare Q = A / B:
 * Si analizza il dividendo A dal bit più significativo (MSB, 31) verso il meno
 * significativo (LSB, 0).
 * Per ogni bit i:
 *   - Si verifica se la porzione di A da bit 31 a bit i (ovvero A >> i) è >= B.
 *   - Se VERO: significa che il bit i del quoziente deve essere 1.
 *     Sottraiamo (B << i) da A e impostiamo il bit i nel quoziente Q |= (1 << i).
 *   - Se FALSO: il bit i del quoziente rimane 0.
 *

 *  Il ciclo esegue esattamente 32 controlli (o meno se si usa il clz per
 *  saltare gli zeri iniziali). Zero cicli lineari, zero attese di milioni di
 *  iterazioni.
 *
 * -----------------------------------------------------------------------------
 * 2. APPLICAZIONI PRATICHE
 * -----------------------------------------------------------------------------
 * - Processori senza Unità Divisore (MCU Embedded Low-Cost):
 *   I microcontrollori ultra-economici (es. vecchi ARM Cortex-M0 o AVR) non hanno
 *   l'istruzione hardware SDIV/UDIV. Il compilatore (GCC) emula la divisione
 *   in C/Assembly usando esattamente questo algoritmo (libgcc __udivsi3).
 *
 * - Fixed-Point Arithmetic (Game Engines / DSP):
 *   Quando si lavora in virgola fissa per evitare la FPU, la divisione di numeri
 *   scalati (es. Q16.16) richiede l'allineamento dei bit tramite shift prima
 *   della sottrazione.
 *
 * -----------------------------------------------------------------------------
 * 3. IMPLEMENTAZIONI HARDWARE E ASSEMBLY
 * -----------------------------------------------------------------------------
 * Sulle CPU moderne, l'istruzione di divisione intera hardware (es. IDIV su x86)
 * è tristemente nota per essere l'operazione aritmetica PIÙ LENTA di tutte:
 * - ADD / SUB / AND / OR: 1 ciclo di clock.
 * - MUL (Moltiplicazione): 3-4 cicli di clock.
 * - DIV (Divisione HW): 10-40 cicli di clock (a seconda della CPU)
 *
 * Per questo motivo, quando il divisore B è noto a tempo di compilazione, i
 * compilatori NON usano l'istruzione DIV, ma la sostituiscono con una
 * "Moltiplicazione per costante magica" (Reciprocal Multiplication / Barrett Reduction).
 *******************************************************************************/

/*******************************************************************************
 * RECURSIVE BINARY DIVISION (O(log N)) - BITWISE E SWAR
 *******************************************************************************
 *
 * TEORIA E APPLICAZIONI:
 * Questa e' una divisione logaritmica. Invece di sottrarre b iterativamente,
 * cerchiamo il piu' grande multiplo di b in potenze di 2 che sta dentro a.
 *
 * L'approccio ricorsivo (tail-recursive o quasi) e' molto apprezzato nei
 * linguaggi funzionali ed e' la base formale per la sintesi dei circuiti di
 * divisione (restoring division algorithm) nelle ALU (Arithmetic Logic Unit).
 *******************************************************************************/

#include <stdio.h>

// ============================================================================
// POPCOUNT SWAR 32-BIT
// ============================================================================
static inline int popcount_swar32(unsigned int i)
{
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

// ============================================================================
// MSB SWAR 32-BIT
// ============================================================================
static inline unsigned int msb(unsigned int n)
{
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n - (n >> 1);
}

// ============================================================================
// 1. APPROCCIO STANDARD ITERATIVO (32 Iterazioni fisse, 0 risk overflow, O(32))
// ============================================================================
unsigned int integer_divide_bitwise(unsigned int a, unsigned int b)
{
    if (!b)
        return 0; // Gestione divisione per zero
    if (a < b)
        return 0;

    unsigned int quotient = 0;

    // Scorriamo tutti i bit dal 31 al 0
    for (int i = 31; i >= 0; i--)
    {
        // Shiftiamo a a destra per evitare l'overflow di b << i
        if ((a >> i) >= b)
        {
            quotient |= (1U << i); // Impostiamo il bit 'i' nel quoziente
            a -= (b << i);         // Sottraiamo la parte calcolata dal dividendo
        }
    }

    return quotient;
}

// ============================================================================
// 2. APPROCCIO RICORSIVO O(log n)
// ============================================================================
unsigned int integer_divide_recursive(unsigned int a, unsigned int b)
{
    if (!b)
        return 0;
    if (!a || a < b)
        return 0;

    if (a == b || (a - b < b))
        return 1;

    unsigned int a_or = a;
    unsigned int shift = 0;

    // Questo ciclo calcola la differenza di lunghezza bit tra a e b
    while (a > b)
    {
        a >>= 1;
        shift++;
    }

    if (a == b && (a_or == (b << shift)))
        return 1U << shift;

    unsigned int diff = a_or - (b << (shift - 1));
    return (1U << (shift - 1)) + integer_divide_recursive(diff, b);
}

// ============================================================================
// 3. VERSIONE FULL SWAR CHE USA POPCOUNT E MSB in O(1)) per eliminare il ciclo while.
// ============================================================================
unsigned int integer_divide_swar(unsigned int a, unsigned int b)
{
    if (!b || a < b)
        return 0;

    // Troviamo istantaneamente la differenza di posizione tra l'MSB di a e l'MSB di b.
    // Sottraendo 1 a msb e facendo popcount otteniamo l'indice del bit!
    unsigned int idx_a = popcount_swar32(msb(a) - 1);
    unsigned int idx_b = popcount_swar32(msb(b) - 1);
    unsigned int shift = idx_a - idx_b;

    // Vediamo se scalando b dello shift calcolato restiamo dentro a
    unsigned int b_shifted = b << shift;

    // Se b allineato supera a, dobbiamo scendere di 1 potenza di due
    if (b_shifted > a)
    {
        shift--;
        b_shifted >>= 1;
    }

    // Ricorsione: accumulo (1 << shift) al quoziente e passo il resto
    return (1U << shift) + integer_divide_swar(a - b_shifted, b);
}

// ============================================================================
// MAIN DI TEST
// ============================================================================
int main(void)
{
    printf("--- TEST TUA VERSIONE CORRETTA ---\n");
    printf("%u %u %u %u\n",
           integer_divide_recursive(64, 7),
           integer_divide_recursive(51, 3),
           integer_divide_recursive(59, 3),
           integer_divide_recursive(64, 8));

    printf("%u %u %u %u\n",
           integer_divide_recursive(3000, 25),
           integer_divide_recursive(2000, 12),
           integer_divide_recursive(119, 5),
           integer_divide_recursive(1200, 60));

    printf("\n--- TEST VERSIONE FULL SWAR (Senza While) ---\n");
    printf("%u %u %u %u\n",
           integer_divide_swar(64, 7),
           integer_divide_swar(51, 3),
           integer_divide_swar(59, 3),
           integer_divide_swar(64, 8));

    // Test caso limite: Numeri enormi che rompevano il check b << 1
    printf("\n--- TEST LIMITI ---\n");
    printf("4000000000 / 1 = %u\n", integer_divide_swar(4000000000U, 1U));
    printf("4000000000 / 3000000000 = %u\n", integer_divide_swar(4000000000U, 3000000000U));

    return 0;
}