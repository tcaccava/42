/*******************************************************************************
 * SIGNED INTEGER OVERFLOW - TEORIA, UB E IMPLEMENTAZIONI HARDWARE
 *******************************************************************************
 * 
 * 1. BACKGROUND TEORICO E UNDEFINED BEHAVIOR (UB)
 * 
 * In C, il comportamento dell'overflow dipende dal tipo intero:
 * - Unsigned: L'overflow è BEN DEFINITO. L'aritmetica è modulare (wrap-around). 
 *   UINT_MAX + 1 torna a 0 in modo sicuro e prevedibile.
 * - Signed: L'overflow è UNDEFINED BEHAVIOR (UB). Se l'operazione supera i 
 *   limiti di INT_MAX o INT_MIN, lo standard C dice che il compilatore è 
 *   libero di fare qualsiasi cosa.
 * 
 * Il problema: Molti programmatori provano a checkare l'overflow in questo modo:
 *     if (a + b < a)  /* OVERFLOW! 
 * A causa dell'UB, i compilatori moderni (gcc/clang -O2/-O3) presumono che 
 * l'UB non accada MAI. Quindi per il compilatore (a + b < a) è matematicamente 
 * FALSO e cancella interamente l'if, lasciando il programma vulnerabile.
 * 
 * L'unico modo matematicamente sicuro in C puro è manipolare l'equazione prima 
 * che l'UB si verifichi, calcolando la "distanza" rimanente dal limite massimo 
 * (INT_MAX - b) o minimo (INT_MIN - b) e verificando se 'a' supera quella distanza.
 * 
 * -----------------------------------------------------------------------------
 * 2. APPLICAZIONI PRATICHE E SICUREZZA
 * -----------------------------------------------------------------------------
 * - Prevenzione Vulnerabilità (CWE-190): L'integer overflow è alla base di 
 *   exploit gravissimi. Se un hacker riesce a mandare in overflow un calcolo di 
 *   allocazione memoria (es. malloc(sizeA + sizeB)), il programma allocherà poca 
 *   memoria per poi scriverci troppi dati (Buffer Overflow).
 * - Sistemi Critici e MISRA C: Nel software avionico, automobilistico e medico, 
 *   ogni singola addizione tra input esterni DEVE essere protetta con costrutti 
 *   come questo per certificare il codice.
 * - Virtual Machine e Interpreti: Linguaggi come Python gestiscono interi di 
 *   grandezza arbitraria. Il motore C sottostante usa check simili a ogni somma; 
 *   se scatta l'overflow, alloca dinamicamente più memoria per il numero.
 * 
 * -----------------------------------------------------------------------------
 * 3. IMPLEMENTAZIONI HARDWARE E INTRINSICS
 * -----------------------------------------------------------------------------
 * - I flag della CPU: A livello hardware, la CPU non sa cosa sia l'UB. Fa 
 *   semplicemente la somma bit a bit. Se c'è un riporto (carry) nel bit di 
 *   segno (MSB) ma non un riporto in uscita, o viceversa, la CPU accende 
 *   l'Overflow Flag (OF su x86, V su ARM).
 * - Compiler Built-ins: GCC e Clang offrono funzioni intrinseche come:
 *     __builtin_sadd_overflow(a, b, &result)
 *   Questa funzione non fa i controlli di (INT_MAX - b). Compila direttamente 
 *   nella singola istruzione hardware di ADD seguita da un check sul flag OF (JO/BVS), 
 *   rendendo l'operazione istantanea e branchless.
 *******************************************************************************/

#include <stdio.h>
#include <limits.h>

// Ritorna 1 se la somma a + b produrrebbe overflow su int a 32 bit, 0 altrimenti. 
// Sicuro contro l'Undefined Behavior (UB).
int signed_overflow_add(int a, int b) {
    // Se b è positivo, l'overflow avviene solo superando INT_MAX
    if (b > 0)
        return a > (INT_MAX - b);
    
    // Se b è negativo, l'overflow avviene (underflow) solo scendendo sotto INT_MIN
    else if (b < 0)
        return a < (INT_MIN - b);
    
    // Se b è 0, la somma è identica ad a, nessun overflow possibile
    return 0;
}

int main(void) {
    printf("(2147483647, 1) -> %d\n", signed_overflow_add(2147483647, 1));       // 1 (Overflow)
    printf("(2147483647, -1) -> %d\n", signed_overflow_add(2147483647, -1));     // 0 (Safe)
    printf("(-2147483648, -1) -> %d\n", signed_overflow_add(-INT_MAX - 1, -1));  // 1 (Underflow)
    
    // Altri test
    printf("(10, 20) -> %d\n", signed_overflow_add(10, 20));                     // 0
    printf("(-2147483648, 1) -> %d\n", signed_overflow_add(-INT_MAX - 1, 1));    // 0

    return 0;
}