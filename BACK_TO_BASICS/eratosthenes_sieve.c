#include <stdio.h>
#include "./printbits.c"
// Il crivello di Eratostene e' il piu' antico algoritmo per la ricerca dei numeri primi inferiori ad n.
// L'algoritmo si basa su un concetto semplice : scrivi tutti i numeri da 2 ad n,prendi il primo numero non
// segnato,partendo da 2,e dichiaralo primo,quindi elimina tutti i suoi multipli,e itera finche' non arrivi alla radice
// quadrata di n. Cio' che resta non segnato e' primo. Una implementazione elementare userebbe un array di int o di char
// , uno per ogni numero,ma si tratta di un enorme spreco di risorse: un numero o e' primo o non lo e',ci basta un singolo bit.
// Con una bitmap si puo' ridurre il consumo di memoria di un fattore pari ad 8. Per mappare il numero n nella bitmap l'indice del byte sara
// n/8 e la posizione del bit all'interno del byte sara' n % 8.
// Si parte con la bitmap con tutti i bit accesi,assumendo che ogni numero sia potenzialmente primo,tranne 0 e 1 che vanno forzati a 0 da subito.
// Per ogni numero n da 2 a radice quadrata di n si controlla se il bit corrispondente e' ancora 1,se lo e' trattasi di un primo,quindi vanno spenti
// tutti i suoi multipli partendo da n^2,perche' i multipli piu' piccoli di n sono gia' stati eliminati dai primi precedenti.
// Applicazioni reali:
// -Crittografia (RSA): La generazione di chiavi RSA richiede la ricerca di grandi numeri primi.
//  Il crivello è il primo passo per filtrare i candidati prima di test di primalità più costosi (come Miller-Rabin).
// -Teoria dei Numeri: Calcolo della funzione di conteggio dei primi $\pi(x)$ o studio della distribuzione delle lacune spettrali tra i primi.
// -Sistemi Embedded: Quando hai pochi KB di RAM ma devi generare sequenze pseudo-casuali basate su numeri primi per protocolli di comunicazione.

void sieve_of_eratosthenes(unsigned char *bitmap, int limit)
{
    for (int i = 0; i < (limit + 7 >> 3); i++) // setto tutti i bit a 1,arrotondamento dei byte per eccesso per essere sicuri di coprire ogni bit
        bitmap[i] = 0xFF;
    bitmap[0] &= ~3;                                    // i primi due bit sono settati a 0 perche' 0 e 1 non sono primi
    for (int prime = 2; prime * prime < limit; prime++) // parto da 2 e avanzo fino al quadrato del numero
    {
        if ((bitmap[prime >> 3] >> ((prime & 7)) & 1)) // se quel bit e' acceso,allora spengo tutti i suoi multipli,partendo dal suo quadrato
            for (int i = prime * prime; i < limit; i += prime)
                bitmap[i >> 3] &= (~(1 << (i & 7)));
    }
}

int main()
{
    unsigned char bitmap[10];
    sieve_of_eratosthenes(bitmap, 80);
    for (int i = 0; i < 10; i++)
        printbits(bitmap[i]);
}