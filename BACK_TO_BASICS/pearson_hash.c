#include <stdio.h>
// L' hashing di Pearson, introdotto da Peter Pearson nel 1990, rappresenta una pietra miliare della computer science
// per sistemi a risorse limitate (8-bit, sistemi embedded, microcontrollori). È una funzione di hash progettata per
// essere estremamente veloce su hardware che non possiede unità di calcolo complesse.
// Negli anni '90, molti processori non avevano registri a 32 o 64 bit efficienti. Calcolare algoritmi come lo SHA richiedeva troppi cicli di clock.
// Pearson cercava un modo per mappare stringhe di lunghezza variabile in un singolo byte (0-255) con una distribuzione uniforme, evitando che stringhe simili
// finissero nello stesso bucket.
// Il cuore del Pearson Hashing è una S-Box(Substitution Box) che consiste in una permutazione casuale dei numeri da 0 a 255.
// La tabella deve essere una permutazione(una bisezione): ogni numero da 0 a 255 deve apparire esattamente una volta.
// Se la tabella contenesse duplicati, perderesti entropia a ogni passaggio, portando a una collisione inevitabile (il "collasso" dell'hash).
// Una permutazione garantisce che ogni byte in ingresso possa mappare l'accumulatore in un nuovo stato unico tra i 256 disponibili.
// A differenza degli hash moderni che processano blocchi di dati (es. 64 byte alla volta), Pearson è un algoritmo iterativo e sequenziale:
// mantiene un accumulatore (uno stato interno di 1 byte),processa il messaggio un byte alla volta e
// utilizza il byte corrente e lo stato dell'accumulatore per saltare all'interno della tabella di permutazione(Logica del "Byte-at-a-time").
// Il risultato finale è un hash che dipende strettamente dall'ordine dei caratteri: scambiare "AB" con "BA" produrrà hash completamente diversi
// (resistenza alla trasposizione).
// Di solito si usa una tabella fissa derivata dalle cifre decimali di pi o numeri casuali pre-calcolati.
// Un approccio alternativo consiste nel generare una permutazione tramite operazioni bitwise,implementando un algoritmo di rimescolamento
// (come il Fisher-Yates Shuffle) o un Linear Congruential Generator (LCG) estremamente semplice, ma usando solo XOR, shift e addizioni per riempire
// l'array di 256 posizioni assicurandosi che non ci siano ripetizioni.
// Sebbene l'hash di Pearson nasca per produrre 8 bit, la teoria permette di generare hash di qualsiasi dimensione (16, 32, 64 bit) con un trucco matematico
// elegante: per ottenere un hash a 16 bit, si esegue l'algoritmo due volte. La seconda volta, però, si modifica leggermente il valore iniziale (es. `hash[0] + 1`).
// Questo produce due byte indipendenti che, concatenati, formano un hash più robusto.
// Vantaggi:
// 1)Zero moltiplicazioni: ideale per processori senza ALU aritmetica veloce.
// 2)Impronta di memoria minima: richiede solo 256 byte per la tabella.
// 3)Velocità: su un sistema a 8-bit, è l'hash più veloce in assoluto.
// Svantaggi:
// 1)Non crittografico: non è resistente alle collisioni intenzionali,ma in fondo nemmeno a quelle occasionali. Un attaccante può facilmente generare una stringa che produca un hash specifico.
// 2)Dipendenza dalla tabella: se la tabella di permutazione è "debole" (es. troppo lineare), la distribuzione degli hash sarà debole col rischio
// di clusterizzazione.

// // IL PARADOSSO DEL COMPLEANNO (Birthday Attack) E PEARSON:
// Nonostante la perfetta distribuzione (entropia) della lookup table, l'hashing di Pearson
// a 8-bit soffre intrinsecamente del "Birthday Paradox".
// In probabilità, il paradosso dimostra che in un gruppo di sole 23 persone, la chance che
// due compiano gli anni lo stesso giorno è superiore al 50%, nonostante l'anno abbia 365 giorni.
//
// Applicato a Pearson (spazio hash di 2 ^ 8 = 256 valori):
// La probabilità di una collisione non cresce linearmente con il numero di input, ma
// seguendo il numero di coppie possibili. La formula di approssimazione per la collisione è:
// N ≈ sqrt(2 * H * ln(2)), dove H è lo spazio degli hash.
// Per H = 256,cioe' 8bit, il valore critico N è circa 19,per 32 bit e' circa 77000,per 160bit(SHA-1) e' 2 ^80
//
// Ciò significa che dopo aver generato solo ~19 hash di stringhe diverse, c'è già il 50%
// di probabilità che due di esse producano lo stesso valore (collisione).
// Per questo motivo, l'hashing a 8-bit è adatto per indicizzare tabelle hash in memoria,
// ma è considerato "debole" per scopi di identificazione univoca o sicurezza.
unsigned int pearson_hash(unsigned char *data, size_t n)
{
    unsigned char lookup_table[256];
    for (int i = 0; i < 256; i++) // riempio la lookup table con numeri ordinati da 0 a 255
        lookup_table[i] = (unsigned char)i;
    unsigned char xoring = 0xF2; // scelgo un fattore di xoring,puo' essere totalmente casuale,non e' hardcodato nell'algoritmo,come d'altronde la stessa lookup table
    unsigned char j = 0;
    for (int i = 0; i < 256; i++) // invece di utilizzare una table hardcodata, genero la mia solo con bitwise,risparmiando spazio in memoria di archiviazione(footprint) e
    // evitando la bad practice dei Magic Numbers; la base matematica della permutazione degli indici senza generare doppioni ne' saltare alcun elemento
    // risiede nella Teoria degli insiemi,e piu' specificamente nella proprieta' delle funzioni biunivoche,o Biezioni.
    // Lo scambio tra due elementi di uno stesso array cambia l'ordine degli elementi ma non la loro identita',quindi l'uso del bitwise
    // serve esclusivamente a garantire la complessita',e quindi non reversibilita',della permutazione,non a garantire che sia una permutazione stessa: in sostanza lo swap in place
    // preserva l'invarianza degli insiemi, mentre il bitwise genera semplicemente entropia.
    {
        j = (j + i + xoring) ^ ((i << 3) | (i >> 5)); // addizione + xor + rotazione,il massimo dell'entropia combinata
        unsigned char temp = lookup_table[i];         // swap con temporanea di appoggio
        lookup_table[i] = lookup_table[j];            // rimescolo la table: la solidita' dell'hashing e' intimamente legata a quella del rimescolamento
        lookup_table[j] = temp;
    }
    unsigned int hash = 0; // accumulatore,di solito inizializzato a zero
    for (size_t i = 0; i < n; i++)
    {
        unsigned char index = hash ^ data[i]; // uso il risultato dello xor tra hash e data per calcolare l'indice da consultare nella table,il cui valore diventera' il nuovo valore dell'hash
        hash = lookup_table[index];
    }
    return (unsigned int)hash;
}

int main()
{
    unsigned char data[] = {0xFF, 0x00, 0xF0, 0x0F};
    printf("%u\n", pearson_hash(data, 4));
}