#include <stdio.h>

// LA FNV-1a (Fowler-Noll-Vo) e' un algoritmo di hashing che,a differenza della DJB2,inverte
// l'ordine logico dei fattori. La sua struttura infatti e' XOR-then-Multiply.
// Lo XOR ha una proprieta' fondamentale,non causa carry(riporti),tratta ogni bit
// in modo indipendente, il che e' perfetto per iniettare entropia nel numero. La moltiplicazione
// con uno specifico numero primo(16777619,detto FNV_PRIME),serve a disperdere i bit dello XOR su tutto
// il range dell'intero 32 bit. A differenza del 33 di Bernstein,le costanti di FNV non sono state
// scelte ex post perche' funzionavano bene in test empirici,ma ex ante perche' posseggono proprieta'
// matematiche specifiche:
// 1) l'Offset Basis(2166136261) e' il seed di partenza,calcolato in modo che,prima ancora
// di cominciare a xorare,i bit dell'hash siano gia' in uno stato di caos controllato. Se partissi da zero
// i primi bytes della stringa influenzerebbero solo i bit bassi del numero.
// 2) L'FNV_PRIME non e' un primo qualunque,ma un primo di FNV,cioe' primi che hanno uno specifico numero
// di bit accesi e ottimali per diffondere l'influenza di ogni singolo bit di input su quasi tutti i bit dell'output.
// I primi di FNV generano il cosidetto effetto valanga: se cambi un solo bit nell'input,cambia almeno la meta' dei bit
// nell'hash finale. Mentre la DJB2 e' ottimizzata per stringhe,la FNV-1a e' agnostica rispetto al contenuto:
// tratta ogni byte esattamente allo stesso modo,che sia un carattere ascii o un byte qualunque di un file binario.
// La combo Xor-moltiplicazione e' estremamente efficace nel rompere le sequenze regolari(come pattern ripetuti o sequenza di zeri)
// che sono estremamente comuni nei database e nel caching di oggetti binari. Quando si moltiplica per l'FNV prime,poiche'
// questo numero e' molto grande,i bit dell'hash vengono sparati verso le posizioni MSB molto velocemente: lo XOR avviene prima
// della moltiplicazione proprio per garantire che l'ultimo byte processato passi attraverso la moltiplicazione come generatore
// di entropia prima che la funzione restituisca il valore. Nella variante FNV-1,senza la 'a',l'ordine era inverso
// ma la variante con 'a'(che sta per alternate) e' dimostrato distribuisca meglio l'ultimo byte.

unsigned int hash_fnv1a(unsigned char *str, size_t len)
{
    const unsigned int fnv_prime = 16777619;
    const unsigned int seed = 2166136261U;
    unsigned int hash = seed;
    while (len--)
        hash = (hash ^ *str++) * fnv_prime;
    return hash;
}

int main()
{
    unsigned char buf[] = {0x42, 0x7f, 0xFF};
    unsigned char buf2[] = {0x42, 0x7f, 0xEF};
    printf("%u\n", hash_fnv1a(buf, 3));  // 1358122979, due buf che differiscono per un solo bit producono hash completamente diversi
    printf("%u\n", hash_fnv1a(buf2, 3)); // 1089681075
}