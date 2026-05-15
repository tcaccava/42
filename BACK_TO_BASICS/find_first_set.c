#include <stdio.h>
// La tecnica basata sull'uso dei numeri di De Bruijn e' un algoritmo geniale che permette di trovare l'indice
// del primo bit acceso senza cicli,senza branch(if) e in un tempo costante.
// Un numero di De Bruijn e' un numero in cui ogni possibile sottosequenza di lunghezza k appare esattamente una volta.
// Per i 32bit usiamo un numero (0x077CB531U) che ha una proprieta' incredibile: contiene tutte le possibili sottosequenze
// a 5 bit (2 ^ 5 = 32),da 00000 a 11111, in ogni singola posizione delle 32 possibili nel numero. Poiche' quei 5 bit sono unici
// per ogni posizione(0-31) possiamo usarli come indice di una piccola tabella precalcolata di 32 elementi,specifica per il numero di
// De Bruijn impiegato,che restituira' direttamente la posizione del bit.
// Questo algoritmo e' un O(1) reale,con tempo di esecuzione costante indipendentemente dalla posizione dell'lsb acceso.
// Non c'e' branching,la pipeline della cpu non viene mai svuotata,e' pura aritmetica senza logica.
// Concettualmente e' identico a quello che fa il processore a livello hw con __builtin_ctz o l'istruzione BSF(BIT SCAN FORWARD) su x86.
// In toale sono un Bitwise AND,una moltiplicazione,uno shift e un accesso a memoria(l1 cache),quindi al massimo 5 cicli di clock,contro le
// centinaia che richiederebbe un loop while a causa dei salti condizionali e della latenza della pipeline.

int find_first_set(unsigned int n)
{
    // isolo l'ultimo bit acceso (LSB) sfruttando un bit trick basato sul complemento a due,che azzera tutti i bit tranne il piu' basso acceso
    // In sostanza n e il suo complemento a due convidividono solo il bit acceso piu' basso. L'opposto(-) di un unsigned int equivale al complemento a due di quel numero -n = ~n + 1
    // questo avviene perche' quando inverti i bit con ~n,l'ultimo bit acceso diventa 0,e gli zeri alla sua destra diventano 1
    // aggiungendo 1 quell' 1 viaggia verso sinisra come carry attraverso tutti i nuovi 1 creati,fino ad incontrare il primo 0,che era l'originale ultimo
    // bit acceso. Quel bit torna ad essere 1,e tutto quello che c'e' alla sua sinistra rimane opposto a quello che era in n.
    // quindi fare l'and di n col suo opposto restituisce solo il bit meno significativo perche' azzera tutto a sinistra(perche' i bit sono opposti) e a destra
    // (perche' erano zeri in n). Sopravvive solo quel bit.
    unsigned int lsb = n & -n;

    // se l'input è 0, non ci sono bit settati
    if (n == 0)
        return -1;

    // tabella di lookup pre-calcolata per il numero 0x077CB531U,static const cosi' che la cpu
    // la tenga nella cache l'1 con accesso istantaneo
    static const int table[32] = {
        0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
        31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9};

    // hash e lookup:
    // moltiplicare l'lsb,che e' una potenza di 2, per il De Bruijn significa essenzialmente shiftare il De Bruijn a sinistra per un numero di posizioni pari alla posizione dell'lsb,e poi isolare i 5 bit unici con lo shift a dx di 27
    // quella sequenza ci dira' esattamente di quanto si e' shiftato il De Brujin
    // infine si pesca il risultato nella tabella di lookup,usando quella sequenza di 5 bit unica come hash perfetto per indicizzare la posizione corretta
    return table[(unsigned int)(lsb * 0x077CB531U) >> 27];
}
// Per generare la tabella per il numero 0x077CB531U, si simula quello che farà la funzione per ogni possibile posizione
// del bit (da 0 a 31).Prendo una posizione i (da 0 a 31),calcolo il valore del bit isolato: 2^i, moltiplico per il numero
// di De Bruijn 0x077CB531U * 2^i,estraggo i 5 bit più significativi  >> 27.Il risultato di questa operazione sarà l'indice della tabella, e il valore da scriverci dentro sarà i.
// In binario De Bruijn è 0000011111001011010100110001.Caso i = 0 (bit in posizione 0):(0x077CB531U * 2^0) >> 27 = 0.Quindi: table[0] = 0.
// Caso i = 1 (bit in posizione 1): (0x077CB531U * 2^1) >> 27 = risultato: 1.Quindi: table[1] = 1.
// Caso i = 5 (bit in posizione 5): (0x077CB531U * 2^5) >> 27 = 28. Quindi: table[28] = 5.

int main()
{
    printf("%d\n", find_first_set(0xFF00FF00)); // 8
}