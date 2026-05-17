#include <stdio.h>
#include "./popcount_swar32.c"

// L'algoritmo PDEP (parallel bits deposit) a 32 bit rappresenta
// l'operazione speculare e inversa del PEXT. Il suo scopo e'
// prendere un gruppo di bit sconnessi e ammassati sul fondo di un
// registro di partenza e distribuirli (expand) all'interno di un
// registro di destinazione, piazzandoli esattamente nelle posizioni
// in cui una maschera di riferimento presenta i bit impostati a 1.
//
// Gli ambiti di utilizzo principali comprendono la crittografia
// per le permutazioni nei cifrari a blocchi, l'elaborazione di
// database colonnari per la deserializzazione rapida di indici
// bitmap compressi, e i motori di scacchi per ricostruire le
// bitboard delle mosse partendo da valori indizzati.
//
// Le basi matematiche dell'implementazione software si poggiano
// sull'aritmetica del complemento a due. l'operazione cruciale
// mask & -mask isola istantaneamente il bit attivo piu' basso
// senza dover scansionare il registro passo dopo passo. L'uso del
// meno unario applicato a un bit booleano puro genera una maschera
// di bit completa (tutti 1 o tutti 0) che consente di evitare
// qualsiasi costrutto condizionale (if-else o operatori ternari),
// eliminando alla radice i rallentamenti da misprediction dei salti.
//
// A livello hardware, dato il costo computazionale delle varianti
// software, intel a partire dall'architettura haswell e amd da
// zen 3 hanno integrato l'istruzione nativa PDEP direttamente nel
// silicio delle cpu (set di istruzioni bmi2). La cpu esegue questa
// operazione in pochissimi cicli di clock sfruttando reti di
// commutazione logica interne sulla matrice di silicio.
// A livello di ottimizzazione avanzata esiste un retroscena hardware
// cruciale che riguarda la differenza tra i vari produttori di cpu.
// mentre intel ha introdotto PDEP in silicio fin dal 2013
// garantendo l'esecuzione in tre soli cicli di clock, amd ha fatto
// una scelta diversa per le sue prime due architetture ryzen.
//
// Nei processori amd basati su zen 1 e zen 2 l'istruzione PDEP
// non era mappata direttamente su circuiti dedicati ma veniva emulata
// tramite microcodice interno. questo causava un crollo verticale
// delle prestazioni, portando l'esecuzione a richiedere fino a ben
// 180 cicli di clock per una singola operazione.
//
// In quel periodo storico l'implementazione software branchless basata
// sul ciclo e sul complemento a due risultava paradossalmente molto
// piu' veloce dell'istruzione hardware nativa se eseguita su cpu amd.
// il problema e' stato risolto definitivamente solo a partire dalle
// architetture zen 3, dove amd ha allineato le prestazioni a intel.
//
// Oggi la regola ingegneristica impone l'uso delle funzioni native
// del compilatore solo se si ha la certezza di compilare per macchine
// moderne con supporto hardware BMI2(Bit Manipulation Instruction Set 2)
// attivo. In tutti gli altri casi, o quando si programma
// su sistemi embedded e microcontrollori privi di queste
// estensioni, la variante software branchless qui implementata rimane
// lo standard di riferimento per efficienza e portabilita'.

unsigned int bit_expand32(unsigned int n, unsigned int mask)
{
    unsigned int mask_popcount = popcount_swar32(mask);
    unsigned int res = 0;
    for (unsigned int i = 0; i < mask_popcount; i++)
    {
        unsigned int low_bit = mask & -mask;
        res |= (low_bit & -(n >> i & 1));
        mask ^= low_bit;
    }
    return res;
}
// oppure se il costo computazionale del popcount e' eccessivo
// unsigned int bit_expand32(unsigned int n, unsigned int mask)
// {
//     unsigned int res = 0;
//     while (mask != 0)
//     {
//         unsigned int low_bit = mask & -mask;
//         res |= low_bit & -(n & 1);
//         n >>= 1;
//         mask ^= low_bit;
//     }
//     return res;
// }

int main()
{
    printf("%u\n", bit_expand32(0x00000005, 0x000000C0)); // 64
}