#include <stdio.h>

// Questa funzione risponde ad un problema molto comune: se ho calcolato l'hash
// della variabile x e di quella y,come posso combinarli senza distruggere l'entropia
// che faticosamente la funzione di hashing ha generato? Sommare i due hash sarebbe disastroso
// perche la somma e' un operazione commutativa,quindi l'hash combinato di (1,2) sarebbe uguale
// a quello di 2,1,generando sostanzialmente collisioni ogni qualvolta si abbia una coppia di
// variabili formata da due elementi,in qualunque ordine vengano sottoposti alla funzione di hashing.
// L'hashing combinato,usato in C++ in std::hash e in Boost,sfrutta le proprieta' matematiche
// della sezione aurea(0x9e3779b9 in hex),che deriva dal rapporto aureo phi = {1 + sqrt{5}/2,cioe' 1,618033.
// Mentre il rapporto aureo e' soltanto un numero irrazionale,in realta' il piu' irrazionale di tutti
// ,perche' e' quello piu' difficilmente approssimabile con una frazione razionale, la sezione aurea e' un concetto geometrico:
// se divido un segmento in due parti,essere sono in sezione aurea se il rapporto tra l'intero segmento e la parte
// piu' lunga e' uguale al rapporto tra la parte piu' lunga e quella piu' corta,e questo rapporto e' proprio
// il rapporto aureo. (A + B)/ A = A / B = phi. 0x9e3779b9 e' la rappresentazione numerica di quel rapporto
// all'interno di un intero a 32bit.
// La scelta della sezione aurea deriva dal fatto che in termini binari e' in assoluto uno dei numeri
// piu' irrazionali,quindi quando usata come additivo agisce come generatore di rumore. Oltre alla irrazionalita',
// essendo la sezione aurea il risultato del rapporto tra 2^32/phi,assicura che il valore dell'hash venga
// spinto uniformemente attraverso tutto lo spettro dei 32 bit,evitando che si accumulino cluster verso lo zero
// o verso il valore massimo. La componente di diffusione asimmetrica(lo shift a sx di 6 posizioni
// e a destra di 2 posizioni) costringe ogni bit originale ad influenzare i suoi vicini in entrambe le
// direzioni,il che rompe le sequenze lineari: due variabili quasi identiche in binario vedrebbero i loro
// hash combinati letteralmente smembrati e rimescolati dallo shift prima dello Xor finale.
// Lo xor finale,come negli altri algoritmi di hashing non crittografico,serve a preservare l'entropia senza
// causare carry che potrebbero appiattire i bit significativi.
// Questa funzione e' il fondamento del Multikey hashing: se immaginiamo di dover creare l'hash dell'oggetto Persona
// che ha due variabili private,nome e cognome,possiamo calcolare l'hash di entrambe e poi combinarlo,ricordando che
// l'algoritmo non e' commutativo,e quindi che hash combinato di nome,cognome e' diverso da quello di cognome,nome
// Essendo priva di moltiplicazioni e basandosi interamente su addizioni,shift e xor,e' estremamente veloce ed efficiente
// e ha una latenza minima, rendendola eccezionale per cpu vecchie o piccoli microcontrollori dove le moltiplicazioni
// sono costose in termini di performance o per essere usata on the fly dentro le collezioni di sistema,come le std::unordered_map.
// Da contraltare a questa estrema efficienza c'e,rispetto ad algoritmi piu' pesanti
// come murmur hash,il fatto che due variabili simili in binario generano,quando l'algoritmo viene eseguito commutativamente
// cioe' sia su a,b che su b,a,hash combinati che sono diversi ma spesso simili,perche' la costante aurea e' uguale in entrambi i casi
// il termine h2,che puo' essere a seconda dei casi sia a che b,e' aggiunto linearmente e soprattutto gli shift e lo xor dipendono esclusivamente
// da h1. In sostanza,poiche' l'operazione dominante e' una somma, il risultato commutativo di quella somma cambiera' pochissimo
// se a e b hanno valori numerici vicini,e gli shift non sono in grado di sconvolgere l'hash con la stessa violenza con cui
// puo' la moltiplicazione con un numero primo enorme come in MurmurHash e FNV. In sintesi hash combine ha una diffusione debole:
// se gli input sono molto simili,i bit alti del risultato tenderanno a somigliarsi indipendentemente dall'ordine degli argomenti.

unsigned int hash_combine(unsigned int h1, unsigned int h2)
{
    const unsigned int sezione_aurea = 0x9e3779b9;
    return h1 ^ (h2 + sezione_aurea + (h1 << 6) + (h1 >> 2));
}

int main()
{
    unsigned int a = 0x01;
    unsigned int b = 0x02;
    printf("%u %u\n", hash_combine(a, b), hash_combine(b, a)); // 2654435834 2654435896,hash diversi ma molto simili
}