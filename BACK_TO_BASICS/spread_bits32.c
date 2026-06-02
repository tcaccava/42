#include <stdio.h>

// Il Bit Spreading (o dilatazione dei bit) sfrutta l'algoritmo scalare Divide et Impera:a livello algebrico, l'operazione di bit spreading mappa una sequenza compatta di bit in uno
// spazio iniettivo più ampio, alterando la metrica posizionale dei dati senza corromperne l'ordinamento logico. Formalmente, dato un vettore di bit B la funzione di dilatazione f(B)
// genera un nuovo vettore V di dimensione doppia tale che V{2i} = B_i e V_{2i+1} = 0 forall i.
// Mentre un approccio iterativo ingenuo (naive) richiede un tempo lineare O(N) proporzionale al numero di bit, l'approccio ottimizzato sfrutta il paradigma Divide et Impera su base
// logaritmica O(log N): l'algoritmo esegue uno sfoltimento geometrico inverso rispetto al collasso di un Popcount SWAR: invece di raccogliere i bit, li respinge a blocchi di potenze
// di due (8 --> 4 --> 2 --> 1), raddoppiando lo spazio intermedio ad ogni passo. L'uso combinato dell'operatore x | (x << shift) clona i blocchi di bit verso sinistra, mentre la successiva
// maschera bitwise & annichilisce i bit duplicati speculari, lasciando intatti solo i bit target nelle loro nuove posizioni geometriche.
// Applicazioni Pratiche: questa tecnica non è un esercizio di stile, ma costituisce la spina dorsale di diverse ottimizzazioni SOTA nei sistemi ad alte prestazioni:
// -Curve di Morton (Z-Order Curves) e codifica spaziale: nei motori grafici (2D/3D) e nei database geografici (GIS), il Bit Spreading è fondamentale per calcolare il Codice di Morton.
// Intervallando i bit di una coordinata X con i bit di una coordinata Y tramite l'algoritmo di spreading, si mappa uno spazio multidimensionale in una singola dimensione (una stringa lineare di bit).
// Morton(X, Y) = spread_bits(X) | spread_bits(Y). Questo pattern preserva la località spaziale: due punti vicini nello spazio bidimensionale avranno codici unidimensionali vicini, ottimizzando drammaticamente
// i successivi accessi alla cache.
// -Strutture Dati Gerarchiche (Quadtree / Octree):nei motori di rendering e nei sistemi di rilevamento delle collisioni, la navigazione all'interno di un Quadtree o di un Octree sparsi può essere risolta senza
// puntatori (Pointer Chasing). Con i bit dilatati, il prefisso binario del Codice di Morton identifica univocamente la traiettoria dei nodi dall'origine dell'albero fino alla foglia desiderata. La ricerca spaziale
// si riduce così a semplici operazioni di mascheramento bitwise.
// -Texture Swizzling nelle GPU: le GPU moderne non memorizzano le texture in memoria lineare (riga per riga), poiché un campionamento verticale causerebbe continui cache miss. Le texture vengono swizzlate (ordinate
// secondo la curva di Morton). L'hardware di texturing delle schede video usa internamente unità di bit-spreading per convertire istantaneamente le coordinate (U, V) dei poligoni nell'indirizzo fisico di memoria del Texel.
// Se in ambiente software siamo costretti a ricorrere a shift e maschere esadecimali a causa dei vincoli strutturali delle CPU general-purpose, in ambiente hardware (silicio puro) l'implementazione del Bit Spreading rivela
// una verità sconvolgente: ha costo computazionale ed energetico pari a zero.
// Hard-Wiring (Costo Zero): su un chip FPGA o un circuito integrato ASIC, l'operazione di dilatazione dei bit non richiede porte logiche (XOR, AND, OR) e non consuma cicli di clock. È implementata tramite hard-wiring,
// ovvero il re-routing fisico delle tracce di rame sul silicio: i 16 pin del bus di input n vengono collegati direttamente ai pin pari del bus di output res. I pin dispari del bus di output res vengono collegati
// direttamente alla linea di terra (GND / 0 logico). Il ritardo di propagazione (Gate Delay) è rigorosamente 0 ns, e l'area occupata sul chip è pari a zero transistor.

unsigned int spread_bits_32(unsigned short n)
{
    // Castiamo a 32 bit per avere lo spazio di manovra hardware
    unsigned int x = n;

    // Allarga i due byte di 8 posizioni
    // Da: 0000000000000000abcdefghijklmnop
    // A:  00000000abcdefgh00000000ijklmnop
    x = (x | (x << 8)) & 0x00FF00FF;

    // Allarga i nibble di 4 posizioni
    // A:  0000abcd0000efgh0000ijkl0000mnop
    x = (x | (x << 4)) & 0x0F0F0F0F;

    // Allarga le coppie di 2 posizioni
    // A:  00ab00cd00ef00gh00ij00kl00mn00op
    x = (x | (x << 2)) & 0x33333333;

    // Allarga i singoli bit di 1 posizione
    // A:  0a0b0c0d0e0f0g0h0i0j0k0l0m0n0o0p
    x = (x | (x << 1)) & 0x55555555;

    return x;
}

int main()
{
    unsigned short input = 0b1111111111111111; // 16 bit tutti a 1
    printf("%#x\n", spread_bits_32(input));    // 0x5555555,cioe' una sfilza di 1 nei bits pari
}
