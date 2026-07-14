/**
 * =====================================================================================
 * @file        keep_highest_bit.c
 * @brief       Isolamento branchless del Most Significant Bit (MSB).
 *
 * 1. BACKGROUND TEORICO E MATEMATICO:
 *    Il problema richiede di trovare la posizione del bit a 1 più a sinistra (MSB) 
 *    in una word, azzerando tutti gli altri. L'algoritmo naïf (implementato con il 
 *    ciclo while commentato) esegue una scansione lineare: costa O(N) nel caso 
 *    peggiore, dove N è la larghezza in bit del registro.
 *
 *    La soluzione ottimizzata utilizza il "Parallel Prefix OR" (noto anche come 
 *    Bit Smearing). Questa tecnica sfrutta una progressione geometrica delle 
 *    operazioni di shift, riducendo la complessità da O(N) a O(log2(N)).
 *    Per un byte (8 bit), log2(8) = 3 operazioni sono matematicamente sufficienti 
 *    per propagare il bit più alto fino alla posizione 0 (shift di 1, 2 e 4 posizioni).
 * 
 *    Matematicamente, se l'MSB è in posizione 'k', il Bit Smearing trasforma il 
 *    valore originale in (2^(k+1)) - 1, che è una sequenza ininterrotta di '1'.
 *    L'ultimo passaggio (n - (n >> 1)) è pura aritmetica delle potenze di due:
 *    sottraendo (2^k) - 1 da (2^(k+1)) - 1 si ottiene esattamente 2^k, ovvero il 
 *    valore del singolo bit originale.
 *
 * 2. APPLICAZIONI PRATICHE:
 *    - Calcolo del logaritmo in base 2: L'MSB isolato corrisponde a floor(log2(n)).
 *      Utile in algoritmi di grafica (es. calcolo dei livelli di Mipmap).
 *    - Memory Allocators (es. Buddy System): Trovare il "bucket" di memoria 
 *      più grande che può essere diviso per soddisfare una richiesta.
 *    - Network Routing (CIDR): Trovare il "Longest Prefix Match" mascherando gli 
 *      indirizzi IP.
 *    - Scheduler del Kernel: Trovare il task a priorità più alta pronto per 
 *      l'esecuzione in una bitmap dei processi.
 *
 * 3. IMPLEMENTAZIONE HARDWARE E PIPELINE CPU:
 *    A livello microarchitetturale, il ciclo 'while' originale è un incubo: genera 
 *    un salto condizionale (branch) a ogni iterazione. Se il branch predictor della
 *    CPU sbaglia, l'intera pipeline di esecuzione deve essere svuotata (flush), 
 *    sprecando preziosi cicli di clock.
 *    L'approccio Bit Smearing è 100% Branchless e Data-Independent: richiede 
 *    esattamente lo stesso tempo di esecuzione sia che il bit sia nella posizione 
 *    7, sia che sia nella posizione 0.
 *
 *    [Nota Hardware ISA]: Nelle moderne CPU, questa operazione è talmente critica
 *    che esistono istruzioni assembly in silicio dedicate (BSR/BSRQ su x86, 
 *    CLZ - Count Leading Zeros su ARM). Tuttavia, questa implementazione in C 
 *    garantisce le massime prestazioni fallback ed è lo standard assoluto per 
 *    il codice portabile a basso livello (ANSI C).
 * =====================================================================================
 */

#include <stdio.h>

// unsigned char keep_highest_bit(unsigned char n) {
//     int i = 0;
//     while(!((n << i++) & 128));
//     return 1 << 8 - i;
// }

// keep_highest_bit: azzera tutti i bit tranne il piu' alto acceso
//
// esempio: 00101000 → 00100000
//
// FASE 1: propaga i bit verso destra fino a riempire tutti i bit
//         sotto il bit piu' alto. ogni passo raddoppia la propagazione.
//         1+2+4 = 7, che e' la distanza massima dal bit piu' alto al bit 0.
//
// 00101000  (n originale)
// ----------------------------------------------------------------
// n |= n >> 1   → propaga di 1
// 00101000
// 00010100  (n >> 1)
// --------
// 00111100
// ----------------------------------------------------------------
// n |= n >> 2   → propaga di 2 (ora copre 3 posizioni: 1+2)
// 00111100
// 00001111  (n >> 2)
// --------
// 00111111
// ----------------------------------------------------------------
// n |= n >> 4   → propaga di 4 (ora copre 7 posizioni: 1+2+4)
// 00111111
// 00000011  (n >> 4)
// --------
// 00111111  (gia' completo in questo caso, ma serve per bit alti)
// ----------------------------------------------------------------
//
// FASE 2: ora n e' una maschera di 1 continua dal bit piu' alto fino al bit 0.
//         sottrai n >> 1 — cioe' la stessa maschera senza il bit piu' alto.
//         rimane solo il bit piu' alto.
//
// 00111111  (n)
// 00011111  (n >> 1)
// --------
// 00100000  ← solo il bit piu' alto ✓
 
unsigned char keep_highest_bit(unsigned char n) {
    n |= n >> 1;  // propaga di 1
    n |= n >> 2;  // propaga di 2
    n |= n >> 4;  // propaga di 4 → tutti i bit sotto il piu' alto sono accesi
    return n - (n >> 1);  // rimuovi tutto tranne il bit piu' alto
}
 

// int main() {
//     printf("%d\n", keep_highest_bit(0b00101001));// 00100000
//     printf("%d\n", keep_highest_bit(0b00010000));// 00010000quindi
// }