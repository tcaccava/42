#include <stdio.h>

// L'Interleaving dei bit (noto come Morton Encoding o Z-Order Curve) nasce come tentativo geniale di risolvere un
// problema fondamentale dell'informatica: la memoria è lineare (un array monodimensionale), mentre il mondo reale è
// spesso multidimensionale (mappe, immagini, spazio 3D). Quando memorizzi una matrice (immagine) riga per riga,
// i pixel che sono vicini verticalmente finiscono lontanissimi in memoria. Questo distrugge la cache locality.
// L'interleaving dei bit risolve questo problema: prendendo i bit di due numeri (X e Y) e intrecciandoli come
// i denti di una cerniera lampo (zip), otteniamo un unico numero (Z). Se mappi questi numeri su un piano, ottieni una curva a
// forma di "Z" che ha una proprietà incredibile: preserva la località spaziale.
// Punti vicini nello spazio 2D rimangono ragionevolmente vicini nella memoria 1D.

// Applicazioni critiche:
// 1)GPU & Texture Mapping: Le schede video non memorizzano le immagini riga per riga, ma in "Morton Order" (tiled memory) per velocizzare
// il filtraggio dei pixel vicini.

// 2)Database Spaziali (Quadtrees/Octrees): permette di interrogare "tutti i punti in quest'area" usando semplici range di indici.

// 3)Ray Tracing: per organizzare le gerarchie di volumi (BVH) in modo che i raggi colpiscano oggetti vicini in memoria.

// Per intrecciare due numeri (X e Y), non puoi semplicemente sommarli. Devi "fare spazio", la cd bit dilatattion.
// Ogni bit del numero originale deve finire in una posizione specifica del risultato finale lasciando un SPAZIO vuoto accanto a sé.
// Il bit 0 di X deve andare nella posizione 0.Il bit 1 di X deve andare nella posizione 2.Il bit 2 di X deve andare nella posizione 4,
// e così via.La tecnica corretta usa le Magic Numbers (maschere bitwise): invece di spostare ogni bit singolarmente, sposti dei blocchi di bit,
// prendi i 16 bit e allontani la metà alta (8 bit) dalla metà bassa (8 bit) con uno shift, applichi una maschera per pulire il risultato,
// e ripeti il processo per i blocchi da 4, poi da 2, poi da 1. Le maschere impiegate per un interleaving a 32 bit sono:
// 0x00FF00FF,0x0F0F0F0F,0x33333333,0x55555555 che isolano rispettivamente i blocchi di 8,4,2 e 1 bit.
// Una volta dilatati X e Y bisogna unirle,tenendo conto pero' che i bit si sovrappongono,quindi di deve shiftare uno dei due di una posizione
// verso sinistra. In questo modo i bit di Y cadranno esattamente negli spazi lasciati dai bit di X.
// Questo e' lo standard industriale utilizzato nelle librerie di grafica ad alte prestazioni perché sfrutta al massimo il parallelismo a livello di bit della CPU.
// sare dati vicini tra loro.

// ESEMPIO:

// X = 15 (0x000F) → 0000 0000 0000 1111
// Y = 240 (0x00F0) → 0000 0000 1111 0000

// #### 1. PROCESSO DI DILATAZIONE DI X (15)

// Il valore viene espanso a 32 bit per fare spazio ai bit di Y.

// Initial (32-bit):00000000 00000000 00000000 00001111
// Step 1 (Shift 8, Mask 0x00FF00FF):
// x |= (x << 8) → 00000000 00000000 00001111 00001111
// & 0x00FF00FF  → 00000000 00000000 00001111 00001111
// Step 2 (Shift 4, Mask 0x0F0F0F0F):
// x |= (x << 4) → 00000000 00001111 11111111 11111111
// & 0x0F0F0F0F  → 00000000 00001111 00000000 00001111
// Step 3 (Shift 2, Mask 0x33333333):
// x |= (x << 2) → 00000000 00111111 00000000 00111111
// & 0x33333333  → 00000000 00110011 00000000 00110011
// Step 4 (Shift 1, Mask 0x55555555):
// x |= (x << 1) → 00000000 01110111 00000000 01110111
// & 0x55555555  → 00000000 01010101 00000000 01010101
// RISULTATO X_DILATATO:000000000 1010101 00000000 01010101,cioe' 0x55

// #### 2. PROCESSO DI DILATAZIONE DI Y (240)

// Initial (32-bit): 00000000 00000000 00000000 11110000
// Step 1 (Shift 8, Mask 0x00FF00FF):
// y |= (y << 8) → 00000000 00000000 11110000 11110000
// & 0x00FF00FF  → 00000000 00000000 11110000 11110000
// Step 2 (Shift 4, Mask 0x0F0F0F0F):
// y |= (y << 4) → 00000000 11111111 11111111 11111111
// & 0x0F0F0F0F  → 00000000 00000000 11110000 11110000
// Step 3 (Shift 2, Mask 0x33333333):
// y |= (y << 2) → 00000000 00000011 11110000 11110000
// & 0x33333333  → 00000000 00000000 00110000 00110000
// Step 4 (Shift 1, Mask 0x55555555):
// y |= (y << 1) → 00000000 00000000 01110000 01110000
// & 0x55555555  → 00000000 00000000 01010000 01010000
// RISULTATO Y_DILATATO: 00000000 00000000 01010000 01010000 .cioe' 0x5500

// #### 3. ASSEMBLAGGIO FINALE (CORRETTO PER X=15, Y=240)

// X_dilatato (15): 00000000 00000000 00000000 01010101 (0x55)
// Y_dilatato (240): 00000000 00000000 01010101 00000000 (0x5500)
// Y_dilatato<<1: 00000000 00000000 10101010 00000000 (0xAA00)

// X | (Y<<1): 00000000 00000000 10101010 01010101

// RISULTATO FINALE (MORTON CODE): 0x0000AA55

// Perché non un loop?Il loop è O(n)(16 iterazioni lente).
// Il metodo "dilatation" è O(log n) (solo 4 passaggi di bitwise operations).
unsigned int bit_interleave32(unsigned short a, unsigned short b)
{
    unsigned int ex_a = (unsigned int)a; // per esempio 15,cioe' 0000000000001111
    unsigned int ex_b = (unsigned int)b; // per es. 240,cioe' 0000000011110000

    ex_a = (ex_a | (ex_a << 8)) & 0x00FF00FF;
    ex_a = (ex_a | (ex_a << 4)) & 0x0F0F0F0F;
    ex_a = (ex_a | (ex_a << 2)) & 0x33333333;
    ex_a = (ex_a | (ex_a << 1)) & 0x55555555;

    ex_b = (ex_b | (ex_b << 8)) & 0x00FF00FF;
    ex_b = (ex_b | (ex_b << 4)) & 0x0F0F0F0F;
    ex_b = (ex_b | (ex_b << 2)) & 0x33333333;
    ex_b = (ex_b | (ex_b << 1)) & 0x55555555;

    return ex_a | (ex_b << 1);
}

int main()
{
    printf("%X\n", bit_interleave32(15, 240));
}