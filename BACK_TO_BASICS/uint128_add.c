/* ============================================================================
 * DOCUMENTAZIONE TECNICA: ADDIZIONE A 128 BIT IN PRECISIONE MULTIPLA (BIGNUM)
 * ============================================================================
 * 1. LINEAMENTI TEORICI E GEOMETRIA ALGEBRICA DEI REGISTRI
 * ----------------------------------------------------------------------------
 * Lo standard nativo dell'architettura limitizza l'ampiezza massima dei registri
 * generici (GPR) a 64 bit (unsigned long long). Per superare questo confine e
 * mappare un intero non con segno a 128 bit, la matematica discreta impone di
 * scomporre l'entita' numerica in una base polinomiale binaria:
 *
 * Valore = (High * 2^64) + Low
 *
 * Dove:
 * - Low (lo) rappresenta la porzione inferiore (i 64 bit meno significativi).
 * - High (hi) rappresenta la porzione superiore (i 64 bit piu' significativi).
 *
 * Quando andiamo a sommare due numeri strutturati in questo modo, l'operazione
 * deve seguire rigorosamente la propagazione del riporto dal blocco inferiore
 * a quello superiore.
 *
 * L'invariante fondamentale per il rilevamento del carry poggia sul principio
 * del wrap-around dei campi finiti. Dato un modulo computazionale M = 2^64,
 * l'addizione tra due addendi non con segno genera un overflow (riporto verso
 * l'alto) se e solo se il risultato troncato dal registro e' strettamente
 * inferiore a uno qualsiasi dei due addendi di partenza:
 *
 * Se (A_lo + B_lo) >= 2^64  =>  Risultato_lo < A_lo  =>  Carry = 1
 * ============================================================================
 * 2. MECCANICA BITWISE BRANCHLESS (ANALISI DEL CODICE)
 * ----------------------------------------------------------------------------
 * La riga di codice: unsigned long long carry = 1 & -(res_lo < a_lo);
 * rappresenta un'ottimizzazione mirata per forzare la CPU a eseguire il calcolo
 * senza ricorrere a istruzioni di salto condizionale (jump).
 *
 * - Valutazione Booleana: L'espressione (res_lo < a_lo) restituisce un valore
 * tipizzato dal compilatore come intero, che puo' essere solo 0 (Falso) o 1 (Vero).
 * - Negazione Algebrica (Complemento a 2):
 * * Se Falso (0):  -0 = 0x0000000000000000
 * * Se Vero (1):   -1 = 0xFFFFFFFFFFFFFFFF (Tutti i bit del registro a 1)
 * - Mascheramento Bitwise: L'operazione '& 1' estrae esclusivamente il bit meno
 * significativo (LSB) della maschera generata.
 * * 0x0000000000000000 & 1 = 0
 * * 0xFFFFFFFFFFFFFFFF & 1 = 1
 *
 * Questo stratagemma assicura l'estrazione del bit di carry in tempo costante,
 * proteggendo la pipeline da fenomeni di Branch Misprediction.
 * ============================================================================
 */

#include <stdio.h>

/**
 * @brief Esegue l'addizione a 128 bit simulata tramite due blocchi a 64 bit.
 * * @param a_hi  Parte alta del primo addendo (64 bit superiori)
 * @param a_lo  Parte bassa del primo addendo (64 bit inferiori)
 * @param b_hi  Parte alta del secondo addendo (64 bit superiori)
 * @param b_lo  Parte bassa del secondo addendo (64 bit inferiori)
 * @param res_hi Puntatore alla variabile che conterra' i 64 bit alti del risultato
 * @return unsigned long long I 64 bit bassi del risultato finale
 */
unsigned long long uint128_add(
    unsigned long long a_hi, unsigned long long a_lo,
    unsigned long long b_hi, unsigned long long b_lo,
    unsigned long long *res_hi)
{
    // Calcolo della parte bassa: se sfora 2^64 - 1 ricomincia da zero (wrap-around)
    unsigned long long res_lo = (unsigned long long)(a_lo + b_lo);

    // Estrazione del carry senza salti condizionali tramite maschera di segno
    unsigned long long carry = 1 & -(res_lo < a_lo);

    // Calcolo della parte alta incorporando il riporto generato dal blocco low
    *res_hi = (unsigned long long)(a_hi + b_hi + carry);

    return (res_lo);
}

int main(void)
{
    unsigned long long res_high;
    unsigned long long res_low;

    printf("=== TESTING ARITMETICA MULTI-PRECISIONE A 128 BIT ===\n\n");

    /* ------------------------------------------------------------------------
     * TEST CASE 1: Addizione Standard senza generazione di Carry
     * A = 0x0000000000000002_0000000000000005
     * B = 0x0000000000000001_0000000000000003
     * ------------------------------------------------------------------------
     */
    printf("[TEST 1] Addizione lineare semplice:\n");
    res_low = uint128_add(0x2ULL, 0x5ULL, 0x1ULL, 0x3ULL, &res_high);
    printf("Input A: 0x0000000000000002_%016llX\n", 0x5ULL);
    printf("Input B: 0x0000000000000001_%016llX\n", 0x3ULL);
    printf("Output:  0x%016llX_%016llX\n\n", res_high, res_low);

    /* ------------------------------------------------------------------------
     * TEST CASE 2: Generazione e Propagazione Critica del Carry
     * Scegliamo A_lo in modo che l'aggiunta di B_lo forzi l'overflow a 64 bit.
     * A = 0x0000000000000005_FFFFFFFFFFFFFFFF
     * B = 0x0000000000000002_0000000000000001
     * ------------------------------------------------------------------------
     */
    printf("[TEST 2] Propagazione del Carry (Low -> High):\n");
    unsigned long long a_lo_test2 = 0xFFFFFFFFFFFFFFFFULL; 
    unsigned long long b_lo_test2 = 0x0000000000000001ULL; // Fara' sballare a_lo a 0x00...
    
    res_low = uint128_add(0x5ULL, a_lo_test2, 0x2ULL, b_lo_test2, &res_high);
    printf("Input A: 0x0000000000000005_%016llX\n", a_lo_test2);
    printf("Input B: 0x0000000000000002_%016llX\n", b_lo_test2);
    printf("Output:  0x%016llX_%016llX\n", res_high, res_low);
    printf("Nota:    La parte alta e' diventata 0x8 (5 + 2 + 1 di carry)\n\n");

    /* ------------------------------------------------------------------------
     * TEST CASE 3: Saturazione Estrema (Overflow totale dei 128 bit)
     * A = 0xFFFFFFFFFFFFFFFF_FFFFFFFFFFFFFFFF (Valore massimo a 128 bit)
     * B = 0x0000000000000000_0000000000000001
     * ------------------------------------------------------------------------
     */
    printf("[TEST 3] Saturazione / Overflow totale del sistema a 128 bit:\n");
    res_low = uint128_add(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL, 0x0ULL, 0x1ULL, &res_high);
    printf("Input A: 0xFFFFFFFFFFFFFFFF_FFFFFFFFFFFFFFFF\n");
    printf("Input B: 0x0000000000000000_0000000000000001\n");
    printf("Output:  0x%016llX_%016llX\n", res_high, res_low);
    printf("Nota:    Il sistema ha eseguito un wrap-around completo a 0\n");

    return (0);
}

/* ============================================================================
 * 3. MAPPA ARCHITETTURALE HARDWARE E COMPILAZIONE
 * ----------------------------------------------------------------------------
 * Quando questo codice viene elaborato da compilatori ottimizzanti (GCC/Clang -O3)
 * per macchine target x86_64 o ARM64, la logica software viene tradotta in
 * istruzioni native condizionate dal silicio:
 *
 * Architettura x86_64 (Intel / AMD):
 * La CPU dispone del registro di stato RFLAGS che contiene il bit CF (Carry Flag).
 * Il compilatore mappa l'emulazione direttamente nelle seguenti istruzioni:
 * MOV RAX, [a_lo]
 * ADD RAX, [b_lo]  <- Se genera overflow, la CPU alza internamente il bit CF
 * MOV RDX, [a_hi]
 * ADC RDX, [b_hi]  <- Esegue l'addizione includendo nativamente il bit CF
 *
 * Se il codice viene scritto in ambiente strict-ANSI C privo di estensioni
 * (come __uint128_t), la funzione proposta garantisce la massima portabilita'
 * cross-platform (anche su microcontrollori a 32 o 16 bit), preservando l'esatta
 * semantica crittografica senza dipendere dal compilatore.
 * ============================================================================
 * 4. AMBITI INDUSTRIALI DI APPLICAZIONE PRATICA
 * ----------------------------------------------------------------------------
 * - Infrastrutture Crittografiche Asimmetriche:
 * I bignum sono il nucleo computazionale degli algoritmi a chiave pubblica.
 * Nel calcolo di RSA-2048 o ECDSA (Curve Ellittiche), i numeri superano i 2000
 * bit. Le librerie (es. OpenSSL) organizzano i vettori di memoria in blocchi,
 * e questa funzione rappresenta il loop fondamentale per sommare i segmenti.
 * - Algoritmi di Hashing ad Alto Throughput:
 * Sistemi di indicizzazione ultra-rapidi come MurmurHash3 o l'algoritmo di
 * smistamento di rete IPv6 richiedono calcoli continui su costanti a 128 bit
 * per la manipolazione degli indirizzi e la prevenzione delle collisioni.
 * - Motori Finanziari in Virgola Fissa:
 * Nei sistemi di trading ad alta frequenza (HFT), le approssimazioni del tipo
 * double (IEEE 754) sono inaccettabili. Si implementa l'aritmetica a 128 bit
 * in virgola fissa per garantire precisione decimale assoluta sulle valute.
 * ============================================================================
 */