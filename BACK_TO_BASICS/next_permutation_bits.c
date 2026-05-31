#include <stdio.h>
// Chiamato anche Gosper's Hack,dato un numero n ritorna il prossimo numero con lo stesso numero di bit accesi.
// Il problema di trovare il successivo numero intero con lo stesso numero di bit accesi (stesso "peso di Hamming") 
// e' alla base del calcolo combinatorio computazionale. Serve per iterare su tutte le possibili k-combinazioni di un insieme di n elementi.
// L'implementazione naif con il while(bit_count(++n) != count) ha una complessita' pessima perche' testa iterativamente 
// ogni singolo numero successivo, sprecando cicli di clock della CPU su numeri che hanno un peso di Hamming diverso.
//
// L'alternativa allo stato dell'arte e' il Gosper's Hack (scoperto da Bill Gosper e pubblicato nel leggendario MIT HAKMEM del 1972).
// Questo algoritmo calcola il risultato in tempo costante O(1) usando esclusivamente operazioni bitwise primitive, 
// senza cicli e senza salti condizionali (completamente branchless).
//
// APPLICAZIONI PRATICHE:
// - Motori scacchistici (Chess Engines): usano i bitboard (interi a 64 bit) per rappresentare la scacchiera. 
// Il Gosper's Hack permette di generare istantaneamente tutte le combinazioni di piazzamento di N pezzi identici 
// sui quadrati vuoti (es. generare tutte le disposizioni possibili per la difesa di 3 pedoni in un finale di partita).
// - Crittografia e Sicurezza: usato negli attacchi a forza bruta su algoritmi crittografici per iterare su chiavi 
// che hanno un peso specifico, o per generare maschere di collisione negli hash.
// - Ottimizzazione Combinatoria: risoluzione del problema del commesso viaggiatore (TSP) basata su programmazione dinamica,
// dove si devono testare sistematicamente tutti i sottoinsiemi di k nodi in un grafo di n nodi.
//
// IMPLEMENTAZIONE HARDWARE:
// Anche se scritto in C puro, il Gosper's Hack viene tradotto dai compilatori moderni in istruzioni hardware dedicate.
// Sulle architetture x86_64 moderne, le estensioni BMI1 e BMI2 (Bit Manipulation Instruction Sets) introdotte da Intel/AMD 
// forniscono moduli in silicio progettati appositamente per queste operazioni:
// - L'operazione n & (-n) per isolare il bit piu' basso viene tradotta direttamente nell'istruzione assembly BLSI, 
// eseguita in un singolo ciclo di clock.
// In hardware dedicato (FPGA o ASIC usati nel networking per il calcolo di percorsi di routing ottimali), 
// questo algoritmo non richiede sequenziatori: viene "cablato" come puro circuito combinatorio. 
// Basta un sommatore (per ripple = n + lowest), un set di porte XOR e un barrel shifter per riposizionare 
// i bit accesi a destra. Il risultato esce dal circuito in una frazione di nanosecondo con latenza zero.
int bit_count(unsigned char n)
{
    int count = 0;
    while (n)
    {
        n &= n - 1;
        count++;
    }
    return count;
}
unsigned char next_permutation_bits(unsigned char n)
{
    int count = bit_count(n);
    while (bit_count(++n) != count)
        ;
    return n;
}
// oppure direttamente il gosper hack
//  unsigned char next_permutation_bits(unsigned char n) {
//      unsigned char lowest  = n & (-n);           // isola bit più basso
//      unsigned char ripple  = n + lowest;          // propaga il carry
//      unsigned char ones    = n ^ ripple;          // trova i bit cambiati
//      ones = (ones >> 2) / lowest;                 // risistemalì a destra
//      return ripple | ones;
//  }
int main()
{
    printf("%d\n", next_permutation_bits(0b00010011));
    printf("%d\n", next_permutation_bits(0b00010101));
    printf("%d\n", next_permutation_bits(0b00010110));
}