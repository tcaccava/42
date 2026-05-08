#include <stdio.h>
#include "./rol32.c"
// ChaCha20 e' un algoritmo di crittografia moderna "leggera" ultra-sicura, progettato da Daniel J. Bernstein (djb), è l'evoluzione di Salsa20
// ed è diventato lo standard *de facto* dove AES risulta troppo pesante o rischioso (specialmente su dispositivi mobili o processori senza accelerazione hardware AES-NI).
// ChaCha20 appartiene alla famiglia dei cifrari ARX, un acronimo che definisce l'intero arsenale matematico impiegato:
// Addition (Addizione modulare): + (mod 2^32)
// Rotation (Rotazione bitwise): ROL
// XOR (Exclusive OR)
// In crittografia classica (come AES), si usano le S-Box (Substitution Boxes), ovvero tabelle di ricerca (lookup table) per creare entropia.
// Tuttavia, le tabelle di ricerca sono vulnerabili ai Cache-Timing Attacks, un tipo di attacco side channel in cui un attaccante può misurare
// quanto tempo impiega la CPU a leggere dalla cache e dedurre parti della chiave.
// ARX risolve il problema alla radice: le addizioni, gli XOR e le rotazioni impiegano sempre lo stesso numero di cicli di clock su ogni cpu x86,
// non ci sono salti condizionali ,cosi' che codice è intrinsecamente immune ai timing attacks (esecuzione in tempo costante).
// L'Addizione Modulare viene eseguita con  modulo 2^ 32. In C, questo accade automaticamente usando `unsigned int` a 32 bit (l'overflow viene semplicemente ignorato).
// L'addizione introduce la non-linearità: se usassimo solo XOR e rotazioni, il cifrario sarebbe un sistema di equazioni lineari facilmente risolvibile con l'eliminazione di Gauss.
// L'aggiunta del "carry" (riporto) dell'addizione rompe la linearità, rendendo il cifrario resistente alla crittoanalisi lineare.
// Lo XOR mescola i bit senza i riporti dell'addizione, mentre la rotazione assicura che i bit influenzino posizioni diverse nei calcoli successivi.
// Le Costanti (16, 12, 8, 7) non sono casuali ma scelte per massimizzare l'effetto valanga (Avalanche Effect). Dopo un Quarter Round, un singolo bit cambiato in input
// deve idealmente stravolgere metà dei bit in output nel minor tempo possibile.
// Il Quarter Round opera su 4 parole (word) a 32 bit (che chiameremo a, b, c, d).Lo stato di ChaCha20 puo' essere immaginato
// come una matrice 4 x 4 di parole a 32 bit. Il Quarter Round è l'operazione che "rimescola" una colonna o una diagonale della matrice.
// La sequenza è divisa in **due fasi identiche** nella struttura, ma con costanti di rotazione diverse:
// Fase 1:
// 1. a = a + b
// 2. d = (d ^ a) <<< 16(<<< e' il simbolo convenzionalmente utilizzato per una rotazione a sinistra)
// 3. c = c + d
// 4. b = (b ^ c) <<< 12
// Fase 2: identica alla 1 ma con rotazioni 8 e 7
// Questa struttura è una variante del Network di Feistel o, più precisamente, di una struttura a Add-Rotate-XOR.
// Il design assicura che l'operazione sia invertibile: se conosci lo stato finale e la sequenza, puoi tornare indietro (fondamentale per la decifratura
// , anche se ChaCha20 essendo uno stream cipher usa la funzione solo in avanti per generare il keystream).
// Perche' sia time costant vanno evitate le moltiplicazioni,perche' su alcuni microcontrollori non sono a tempo costante.
//  i branch perche' attivano il branch predictor della CPU, creando variazioni temporali e le tabelle che creano vulnerabilità nella cache.
// E' adottata pressoche' universalmente: WireGuard usa ChaCha20 perché è estremamente veloce in software, permettendo prestazioni notevoli anche su router economici senza accelerazione hardware,
// TLS 1.3, Google ha spinto ChaCha20-Poly1305 (la combinazione tra ChaCha20 e l'autenticatore Poly1305) per proteggere il traffico mobile, e
// QUIC, il protocollo di Google (su cui viaggia YouTube/HTTP3)per la sua bassa latenza computazionale.

void ft_chacha20_quarter_round(unsigned int *a, unsigned int *b, unsigned int *c, unsigned int *d)
{
    *a = *a + *b;
    *d = rol32(*d ^ *a, 16);
    *c = *c + *d;
    *b = rol32((*b ^ *c), 12);

    *a = *a + *b;
    *d = rol32(*d ^ *a, 8);
    *c = *c + *d;
    *b = rol32((*b ^ *c), 7);
}