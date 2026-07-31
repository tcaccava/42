/*******************************************************************************
 * ALGORITMO DI STEIN (BINARY GCD) - TEORIA, HARDWARE E CONFRONTO IMPLEMENTAZIONI
 *******************************************************************************
 * 
 * 1. BACKGROUND TEORICO
 * 
 * L'algoritmo di Stein calcola il Massimo Comune Divisore (MCD) sfruttando solo 
 * shift a destra (divisione per 2), shift a sinistra (moltiplicazione per 2) e 
 * sottrazioni. E' basato su 4 identita' matematiche fondamentali:
 * 
 * 1. Caso Base: gcd(a, 0) = a, e gcd(0, b) = b
 * 2. Entrambi Pari: gcd(a, b) = 2 * gcd(a/2, b/2)
 * 3. Uno Pari, Uno Dispari: gcd(a, b) = gcd(a, b/2) (il fattore 2 e' spazzatura,
 *    cioe' non puo' essere un fattore comune)
 * 4. Entrambi Dispari: gcd(a, b) = gcd(|a - b|, min(a, b)): la differenza tra i
 *    due numeri e' sicuramente pari,quindi possiamo shiftarlo a destra fino a che
 *    non diventa dispari. Questo punto,che sembra il piu' controintuitivo, trova
 *    il suo fondamento nell'algoritmo di Euclide,una proprieta' algebrica dei 
 *    divisori che dice sostanzialmente che ,se d divide esattamente sia a che b,
 *    allora deve per forza dividere anche la loro differenza a - b,per cui l'insieme
 *    dei divisori della coppia iniziale a,b e' esattamente identico a quello dei divisori
 *    comuni della coppia b,a - b,dove b e' il piu' piccolo dei due numeri.
 * 
 * La vera magia dell'algoritmo risiede nel punto 4: la differenza tra due numeri 
 * dispari genera SEMPRE un numero pari. Questo significa che dopo ogni singola 
 * sottrazione, il risultato puo' essere immediatamente shiftato a destra 
 * (dimezzato) al ciclo successivo, abbattendo rapidamente il valore dei numeri.
 * 
 * -----------------------------------------------------------------------------
 * 2. APPLICAZIONI PRATICHE
 * -----------------------------------------------------------------------------
 * - Crittografia Asimmetrica (RSA, ECC): Usato intensivamente in OpenSSL e mbedTLS 
 *   per calcolare l'inverso moltiplicativo modulare su chiavi da 2048/4096 bit, 
 *   dove le istruzioni di divisione standard della CPU non possono arrivare.
 * - Embedded & Bare-Metal: Microcontrollori di fascia bassa (es. Cortex-M0) o 
 *   architetture custom spesso non possiedono un divisore hardware. Usare % o / 
 *   invoca routine software lentissime. Stein risolve in pochi cicli di clock.
 * - Frazioni e Aspect Ratio: Riduzione ai minimi termini di frequenze di clock 
 *   nei PLL o risoluzioni video nei driver grafici.
 * 
 * -----------------------------------------------------------------------------
 * 3. IMPLEMENTAZIONI HARDWARE
 * -----------------------------------------------------------------------------
 * - Istruzioni CLZ / CTZ: I processori moderni possiedono istruzioni hardware come 
 *   TZCNT (x86) o RBIT+CLZ (ARM) che contano i bit a zero in coda a un registro in 
 *   1 singolo ciclo di clock. I cicli while(!(x & 1)) x >>= 1; vengono sostituiti 
 *   dal compilatore con un singolo shift hardware massivo (es: x >>= __builtin_ctz(x)).
 * - Sintesi su FPGA: La totale assenza di moltiplicatori/divisori rende questo 
 *   algoritmo implementabile con poche porte logiche (XOR, Adder-Subtractor e MUX).
 * 
 * -----------------------------------------------------------------------------
 * 4. IL CONFRONTO: RICORSIONE vs ITERAZIONE 
 * -----------------------------------------------------------------------------
 * 
 * SOLUZIONE 1: RICORSIVA BRANCHLESS
 * - Pro: Estremamente elegante. L'uso delle maschere bitwise per calcolare il 
 *   minimo e la differenza assoluta in modo branchless e' un capolavoro di algebra 
 *   booleana. Matematicamente ineccepibile.
 * - Contro: 
 *   1. Stack Overhead: Ogni chiamata alloca un frame sullo stack per k, diff, min 
 *      e il return address. Numeri complessi causano decine di chiamate annidate.
 *   2. No Tail-Call Optimization (TCO): Siccome l'operazione `<< k` viene fatta 
 *      DOPO il ritorno della funzione, il compilatore non puo' appiattire la 
 *      ricorsione in un semplice JMP assembly. Deve impilare e disimpilare tutto.
 *   3. Lavoro Ridondante: Dalla seconda chiamata in poi, sappiamo per certo che 
 *      min e' dispari, eppure il codice ricontrolla `!(a & 1) && !(b & 1)` e 
 *      `!(a & 1)` sprecando cicli ALU.
 * 
 * SOLUZIONE 2: ITERATIVA 
 * - Pro: Macchina da guerra da kernel. 0 byte di stack extra. Nessun return address. 
 *   Estrae i fattori 2 comuni UNA SOLA VOLTA all'inizio. Assicura che 'a' sia 
 *   dispari UNA SOLA VOLTA. Il loop interno (do-while) esegue esclusivamente 
 *   sottrazioni e shift di 'b', limitando le operazioni al minimo teorico per il silicio.
 * - Contro: Meno elegante da leggere. Richiede il costrutto do-while per garantire 
 *   che la prima sottrazione (b - a) avvenga prima del check di uscita (b != 0).
 *******************************************************************************/

#include <stdio.h>

// ============================================================================
// SOLUZIONE 1: La versione Ricorsiva Branchless
// ============================================================================
unsigned int binary_gcd_recursive(unsigned int a, unsigned int b)
{
    // caso base: se uno dei due parametri e' zero, ritorno l'altro
    if (!b)
        return a;
    if (!a)
        return b;

    unsigned int k = 0;
    // divido entrambi per due se entrambi sono pari e conto il numero di shift a destra con k
    while (!(a & 1) && !(b & 1))
    {
        a >>= 1;
        b >>= 1;
        k++;
    }
    // mi assicuro di dividere per garantire che sia a che b siano dispari
    while (!(a & 1))
        a >>= 1;
    while (!(b & 1))
        b >>= 1;

    // calcolo la differenza tra il maggiore e il minore in modo branchless
    unsigned int diff = (b - a) ^ (((b - a) ^ (a - b)) & -(a > b));
    // trovo il minore tra i due in modo branchless
    unsigned int min = b ^ ((b ^ a) & -(a < b));

    // ricorsione con parametri min e diff, shiftati di k
    return binary_gcd_recursive(min, diff) << k;
}

// ============================================================================
// SOLUZIONE 2: Versione Iterativa (Low-Level Systems Approach)
// ============================================================================
unsigned int binary_gcd_iterative(unsigned int a, unsigned int b)
{
    if (!a) return b;
    if (!b) return a;

    unsigned int k = 0;

    // 1. Fase di setup iniziale eseguita UNA SOLA VOLTA.
    // Togliamo i 2 comuni.
    while (((a | b) & 1) == 0)
    {
        a >>= 1;
        b >>= 1;
        k++;
    }

    // Togliamo i 2 rimanenti dal primo operando.
    // Ora siamo certi al 100% che 'a' e' dispari.
    while ((a & 1) == 0)
        a >>= 1;

    // 2. Loop interno pesantemente ottimizzato per CPU.
    do
    {
        // Togliamo i 2 dal secondo operando.
        while ((b & 1) == 0)
            b >>= 1;

        // Ora sia 'a' che 'b' sono dispari. Li ordiniamo.
        if (a > b)
        {
            unsigned int temp = a;
            a = b;
            b = temp;
        }

        // Differenza: dispari - dispari = pari.
        // b diventa pari e al prossimo giro il while lo shriftra' a destra.
        b = b - a;

    } while (b != 0);

    return a << k;
}

// ============================================================================
// ENTRY POINT & TESTS
// ============================================================================
int main(void)
{
    printf("--- TEST RICORSIVA BRANCHLESS ---\n");
    printf("GCD(24, 40): %u\n", binary_gcd_recursive(24, 40));
    printf("GCD(24, 40): %u\n", binary_gcd_recursive(24, 40));
    printf("GCD(77, 11): %u\n", binary_gcd_recursive(77, 11));
    printf("GCD(77, 14): %u\n", binary_gcd_recursive(77, 14));
    printf("GCD(35, 15): %u\n", binary_gcd_recursive(35, 15));
    printf("GCD(0, 15): %u\n", binary_gcd_recursive(0, 15));

    printf("--- TEST ITERATIVA DO-WHILE ---\n");
    printf("GCD(24, 40): %u\n", binary_gcd_iterative(24, 40));
    printf("GCD(24, 40): %u\n", binary_gcd_iterative(24, 40));
    printf("GCD(77, 11): %u\n", binary_gcd_iterative(77, 11));
    printf("GCD(77, 14): %u\n", binary_gcd_iterative(77, 14));
    printf("GCD(35, 15): %u\n", binary_gcd_iterative(35, 15));
    printf("GCD(0, 15): %u\n", binary_gcd_iterative(0, 15));

    return 0;
}