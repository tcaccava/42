/* ========================================================================== */
/* GRAY_TO_BINARY_8BIT.C - CONVERSIONE SWAR ULTRA-OTTIMIZZATA                */
/* ========================================================================== */

#include <stdio.h>

/* ------------------------------------------------------------------------- */
/* 1. BACKGROUND TEORICO: IL SEGRETO DELLO SWAR (PARALLEL PREFIX XOR)        */
/*                                                                           */
/* Il codice Gray è un sistema numerico binario riflesso in cui due numeri   */
/* consecutivi differiscono per una sola posizione di bit.                   */
/*                                                                           */
/* MATEMATICA DELLA CONVERSIONE NATURALE (SEQUENZIALE):                      */
/* Per convertire un codice Gray G nel suo binario naturale B, il bit        */
/* più significativo (MSB) rimane identico (B_n = G_n). Tutti gli altri      */
/* bit successivi si calcolano tramite un'operazione di XOR in cascata da    */
/* sinistra verso destra:                                                    */
/* B_i = B_{i+1} ^ G_i                                                       */
/* Questo significa che ogni bit binario è la somma XOR cumulativa di tutti  */
/* i bit del codice Gray alla sua sinistra. Un approccio iterativo bit-by-bit*/
/* richiederebbe un tempo lineare O(N), dove N è il numero di bit (8).       */
/*                                                                           */
/* L'INCOMPRENSIBILE ARCHITETTURA SWAR (SIMD Within A Register):             */
/* Il trucco magico a scorrimento n ^= n >> 4, 2, 1 abbatte la               */
/* complessità a tempo logaritmico O(\log_2 N). Invece di calcolare lo XOR   */
/* un bit alla volta, lo SWAR calcola lo XOR di gruppi di bit in parallelo,  */
/* sfruttando la ALU come se fosse un elaboratore vettoriale.                */
/*                                                                           */
/* Evoluzione dei bit passo dopo passo (Analisi su 8 bit):                   */
/* Sia l'input n = [g7, g6, g5, g4, g3, g2, g1, g0]                          */
/*                                                                           */
/* Passo 1: n ^= n >> 4                                                      */
/* I 4 bit superiori vengono shiftati a destra e combinati con i 4 inferiori.*/
/* Ora i bit inferiori contengono lo XOR parziale a distanza di 4 posizioni. */
/*                                                                           */
/* Passo 2: n ^= n >> 2                                                      */
/* Il risultato viene shiftato di 2. Ora la propagazione copre i blocchi di  */
/* 2 bit adiacenti.                                                          */
/*                                                                           */
/* Passo 3: n ^= n >> 1                                                      */
/* L'ultimo shift di 1 bit chiude la catena. Ogni bit i riceve la somma      */
/* XOR finale di tutti i bit da g_7 fino a g_i.                              */
/*                                                                           */
/* Grazie alla combinazione geometrica delle potenze di 2 (4, 2, 1), bastano */
/* solo 3 passi anziché 7 operazioni sequenziali.                            */
/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */
/* 2. APPLICAZIONI PRATICHE NELL'INDUSTRIA                                   */
/*                                                                           */
/* L'algoritmo Gray-to-Binary a 8 bit non è un esercizio accademico, ma una  */
/* componente critica in contesti industriali real-time:                     */
/* */
/* - ENCODER ROTATIVI ASSOLUTI: I sensori ottici o magnetici montati sui     */
/* motori industriali (es. bracci robotici) leggono la posizione angolare  */
/* espressa in codice Gray a 8 bit (256 posizioni). Se usassero il binario */
/* naturale, il passaggio da 127 (01111111) a 128 (10000000) cambierebbe   */
/* tutti gli 8 bit simultaneamente; a causa di minimi ritardi fisici di    */
/* lettura, il sensore potrebbe leggere valori spurii enormi. Il codice    */
/* Gray cambia un solo bit alla volta, garantendo l'assenza di glitch. Il  */
/* firmware del microcontroller usa poi questo algoritmo SWAR per          */
/* convertire istantaneamente il dato in binario leggibile dal software.    */
/* */
/* - INTERFACCIA FIFO ASINCRONA (CLOCK DOMAIN CROSSING - CDC): Nelle reti    */
/* e nei SoC hardware, quando i dati passano da un dominio di clock rapido */
/* a uno lento, i puntatori di lettura/scrittura della memoria FIFO        */
/* vengono convertiti in codice Gray prima di essere sincronizzati. Questo */
/* impedisce fenomeni catastrofici di metastabilità nei circuiti digitali. */
/* -------------------------------------------------------------------------- */



/* -------------------------------------------------------------------------- */
/* 3. IMPLEMENTAZIONE HARDWARE (ASIC / FPGA)                                 */
/* */
/* In logica cablata (VHDL o Verilog), la conversione può essere strutturata */
/* in due modi differenti all'interno della cella logica:                    */
/* */
/* STRUTTURA RIPPLE (A cascata standard):                                    */
/* Si collegano 7 porte XOR in serie. L'output della prima porta fa da input */
/* per la successiva.                                                        */
/* - Vantaggio: Consumo di area minimo (solo 7 porte XOR).                   */
/* - Svantaggio: Il ritardo di propagazione (Critical Path) è pari a 7 volte */
/* il ritardo di una singola porta (7 * T_{xor}).                   */
/* */
/* STRUTTURA AD ALBERO (SWAR Hardware - Parallel Prefix Tree):               */
/* Il silicio implementa la stessa identica struttura del codice qui sotto.   */
/* Le porte XOR sono disposte su 3 livelli di profondità (corrispondenti agli*/
/* shift 4, 2, 1).                                                           */
/* - Livello 1: 4 porte XOR operano in parallelo.                            */
/* - Livello 2: Porte XOR incrociate a distanza di 2.                        */
/* - Livello 3: Porte XOR finali adiacenti.                                  */
/* - Il Critical Path hardware scende a soli 3 * T_{xor}. Per una     */
/* FPGA che lavora a 500 MHz, questo dimezzamento del ritardo determina    */
/* il successo del tempismo dei segnali (Timing Closure).                  */
/* -------------------------------------------------------------------------- */

// per es. 011 gray diventa il suo binario corrispondente,cioe' 2, 010 diventa 3 ,011 4 e cosi via
unsigned char gray_to_binary(unsigned char n)
{
    n ^= n >> 4;
    n ^= n >> 2;
    n ^= n >> 1;
    return n;
}
// oppure di bruteforce con la ricorsione e binary che parte da 0
// in bitwise a == b se a ^ b non e' zero,cioe' se !(a ^ b)
// unsigned char gray_to_binary(unsigned char n,unsigned char binary){
//     return (!(n ^ (binary ^ (binary >> 1)))) ? binary : gray_to_binary(n, binary + 1);
//  }

int main()
{
    printf("Gray 0b00000001 -> Binario: %d\n", gray_to_binary(0b00000001));
    printf("Gray 0b00000010 -> Binario: %d\n", gray_to_binary(0b00000010));
    printf("Gray 0b00000011 -> Binario: %d\n", gray_to_binary(0b00000011));
    printf("Gray 0b00000110 -> Binario: %d\n", gray_to_binary(0b00000110));
    
    return 0;
}