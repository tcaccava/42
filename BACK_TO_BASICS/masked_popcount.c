#include <stdio.h>
#include "./popcount_swar32.c"
/*
Il masked_popcount non è un esercizio di stile; è un'operazione fondamentale in diverse architetture software ad altissime prestazioni:
-Motori di Scacchi (Bitboards):nei motori di scacchi moderni (come Stockfish), la scacchiera non è rappresentata da matrici di stringhe o strutture dati complesse,
ma da una serie di unsigned long a 64 bit chiamati Bitboards. Ogni bit rappresenta una casella.n = la mappa di tutti i pedoni bianchi sulla scacchiera.
mask = la mappa delle caselle che compongono il "lato di donna". masked_popcount(n, mask) restituisce istantaneamente quanti pedoni bianchi ci sono sul lato di donna per valutare
la sicurezza del re, il tutto in un singolo ciclo CPU.
-Database colonnari e indici bitmap: i database moderni (come ClickHouse o i sistemi che usano i Roaring Bitmaps) usano gli indici bitmap per fare query fulminee.
Se devi cercare quanti utenti sono Attivi (n) E Abbonati (mask), il database esegue un AND tra i due flussi di bit. Il popcount del risultato ti dice esattamente quante righe soddisfano la query
WHERE attivo = 1 AND abbonato = 1 a velocità prossime a quelle della cache della CPU.
-Cifrari a Flusso e criptanalisi: nello studio delle funzioni booleane applicate alla crittografia, il calcolo del masked popcount serve a misurare la correlazione lineare tra i bit di stato interni e
i bit di output, fondamentale per testare la resistenza di un algoritmo contro gli attacchi di criptanalisi lineare.
*/
unsigned int masked_popcount(unsigned int n, unsigned int mask)
{
    return (popcount_swar32(n & mask));
}

int main()
{
    printf("%u\n", masked_popcount(0xFF00FF00, 0xFF00F000)); // 12
}