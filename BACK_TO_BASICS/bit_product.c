/* ===================================================================================
 * FILE: bit_product.c
 * 
 * 1. OBIETTIVO
 *    Implementare la moltiplicazione tra due interi a 32 bit senza usare l'operatore *
 *    usando solo shift e addizioni, con output a 64 bit per evitare overflow.
 * 
 * 2. BACKGROUND TEORICO E APPROCCI
 *    Ho utilizzato tre metodologie, dalla più lenta alla più estrema:
 *    - Versione Volgare: O(N) basata su addizioni ripetute. Inutile su larga scala.
 *    - Versione SWAR (O(K)): Isola il bit (LSB) e calcola la sua posizione tramite 
 *      popcount. Esegue il ciclo solo per i bit accesi (K).
 *    - Versione Black Magic (Float): Sfrutta la FPU (Floating Point Unit) per estrarre 
 *      l'esponente del numero, equivalente a un calcolo logaritmico istantaneo.
 * 
 * 3. SPIEGAZIONE DEL TRICK IEEE 754 (FLOAT BLACK MAGIC)
 *    Perché la versione "black magic" funziona?
 *    Un float a 32 bit segue lo standard IEEE 754,diviso in 3 sezioni: [1 bit Segno] [8 bit Esponente] [23 bit Mantissa].
 *    Il numero viene salvato come: 1.Mantissa * 2^(Esponente - 127).
 *    
 *    ESEMPIO: Come viene salvato 16.0?
 *    16.0 in binario è 10000, ovvero 1.0 * 2^4.
 *    - Segno: 0 (positivo).
 *    - Esponente: 4 + 127 (bias) = 131. In binario: 10000011.
 *    - Mantissa: .0 (tutti zeri).
 *    Mappa bit-a-bit (32 bit totali):
 *    [0] [10000011] [00000000000000000000000]
 * 
 *    PERCHÉ IL BIAS +127?
 *    Gli 8 bit dell'esponente possono andare da 0 a 255. Lo standard riserva 0 e 255 
 *    per casi speciali (zero, inf, NaN). Per rappresentare esponenti negativi (es. 2^-5), 
 *    si usa un "offset" (bias). Sottrarre 127 centra l'intervallo: 
 *    127 -> 2^0, 131 -> 2^4, 126 -> 2^-1. È un sistema per gestire esponenti positivi 
 *    e negativi senza usare un bit di segno dedicato solo per l'esponente.
 *
 *    
 *    Quando castiamo una potenza di 2 (es. 16.0 = 1.0 * 2^4) a float, la FPU calcola 
 *    automaticamente l'esponente. Nel caso di 16, l'esponente memorizzato è 131 
 *    (ovvero 4 + 127 bias). Perché aggiungiamo 127? L'esponente deve poter essere negativo 
 *    (es. 2^-5), ma gli 8 bit del campo esponente sono unsigned (vanno da 0 a 255).
 *    Sottraendo 127, creiamo un range che va da -127 a +128.Se l'esponente salvato è 127, 
 *    il valore reale è 127 - 127 = 0 (2^0 = 1).Se l'esponente salvato è 131, il valore reale è 
 *    131 - 127 = 4 (2^4 = 16).Quando tu prendi b_lsb (che è una potenza di 2 perfetta, es: 16) 
 *    e lo casti a float, la CPU calcola matematicamente 1.0 * 2^4. Scrive 131 nel campo esponente. 
 *    Quando tu leggi 131 come intero e togli 127, ottieni magicamente 4.
 *    Leggendo i bit del float come intero e shiftando a destra di 23, scartiamo la mantissa 
 *    e ci resta 131. Sottraendo 127, otteniamo esattamente 4, ovvero la posizione del bit.
 * 
 * 4. IMPLEMENTAZIONE HARDWARE
 *    Tutte le operazioni (&, |, <<, >>, ^) mappano direttamente su porte 
 *    logiche delle ALU (AND, OR, NOT, Barrel Shifter). 
 *    La versione Black Magic è particolarmente veloce perché 
 *    l'istruzione di cast (float) è pipelined nell'hardware moderno (FPU), permettendo 
 *    di eseguire il calcolo in parallelo a altre operazioni intere senza stalli.
 *    Non ci sono branch condizionali pesanti 
 *    (eccetto il while, che è ottimizzato dal compilatore), rendendo il codice 
 *    estremamente efficiente per la pipeline della CPU.
 * 
 * 5. APPLICAZIONI PRATICHE
 *    - Motori Grafici: Calcolo di matrici di trasformazione 4x4. Moltiplicare vettori 
 *      per matrici richiede migliaia di operazioni al secondo. L'ottimizzazione branchless 
 *      previene il "branch misprediction" (quando la CPU indovina male l'if), che è la 
 *      causa principale di rallentamento nei cicli intensivi.
 *    - Crittografia: Algoritmi come la moltiplicazione di Montgomery (usata in RSA/ECC) 
 *      si basano pesantemente su moltiplicazioni "sparse" (dove molti bit sono zero). 
 *      Ridurre le iterazioni ai soli bit accesi è il modo standard per evitare attacchi 
 *      di timing (dove l'hacker deduce la chiave misurando quanto tempo impiega la CPU).
 *    - Networking: Parsing di pacchetti dove devi estrarre flag da header bit-packed. 
 *      Estrarre l'indice di un flag con un cast float è una tecnica nota in kernel driver 
 *      estremamente ottimizzati.
 * ===================================================================================
 */

#include <stdio.h>
#include "./popcount_swar32.c"

// Versione volgare
// unsigned long long adder(unsigned int a, unsigned int n)
// {
//     unsigned long long res = 0;
//     while (n--)
//         res += a;
//     return res;
// }

// unsigned long long bit_product(unsigned int a, unsigned int b)
// {
//     unsigned long long res = 0;
//     while (b)
//     {
//         unsigned int b_lsb = b & (-b);
//         res += adder(a, b_lsb);
//         b ^= b_lsb;
//     }
//     return res;
// }

/**
 * APPROCCIO SWAR (O(K))
 * Calcola il prodotto isolando i bit accesi tramite (b & -b) e calcolando 
 * la loro posizione (indice) tramite la funzione popcount_swar32.
 * E' branchless ed estremamente portabile.
 */
unsigned long long bit_product(unsigned int a, unsigned int b)
{
    unsigned long long res = 0;
    while (b)
    {
        /* Isola il bit meno significativo (es. 00010000) */
        unsigned int b_lsb = b & (-b);
        
        /* Calcola la posizione (es. 4) usando il popcount sul bit - 1 */
        unsigned int b_lsb_pos = popcount_swar32(b_lsb - 1);
        
        /* Casting a 64 bit prima dello shift per evitare overflow di registro */
        res += (unsigned long long)a << b_lsb_pos;
        
        /* Rimuove il bit appena processato */
        b ^= b_lsb;
    }
    return res;
}

/**
 * APPROCCIO BLACK MAGIC (O(K))
 * Sfrutta il layout di memoria IEEE 754 per estrarre la posizione del bit 
 * sfruttando la logica hardware della FPU.
 */
unsigned long long bit_product_black_magic(unsigned int a, unsigned int b)
{
    unsigned long long res = 0;
    
    /* Union per leggere i bit del float come intero (punta alla stessa area memoria) */
    union { float f; unsigned int i; } u;

    while (b)
    {
        unsigned int b_lsb = b & (-b);
        
        /* Cast a float: l'hardware calcola automaticamente la notazione scientifica */
        u.f = (float)b_lsb;
        
        /* Estrazione esponente: (u.i >> 23) toglie la mantissa, -127 toglie il bias */
        unsigned int b_lsb_pos = (u.i >> 23) - 127;
        
        res += (unsigned long long)a << b_lsb_pos;
        
        /* Algoritmo di Kernighan per pulire il bit (b & b-1 è più veloce dello XOR) */
        b &= b - 1; 
    }
    return res;
}

int main()
{
    // Test di verifica
    unsigned int a = 34;
    unsigned int b = 11;
    
    printf("Risultato SWAR: %llu\n", bit_product(a, b));
    printf("Risultato Black Magic: %llu\n", bit_product_black_magic(a, b));
    
    return 0;
}