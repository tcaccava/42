#include <stdio.h>

// Lo xor offre la possibilita' di fare swap in place tra due variabili senza usare nemmeno
// una temporanea di appoggio. La base matematica e' affascinante: lo Xor e' un operatore di gruppo Abeliano,cioe'
// e' l'unica operazione bitwise ad essere pienamente reversibile: se xoro x con y,e xoro di nuovo il risultato con y,
// riottengo x. In sostanza XOR e' un addizione senza carry

void xor_swap(unsigned int *a, unsigned int *b)
{
    if (*a ^ *b && a != b) // controllo di protezione per il caso a e b siano uguali o puntino allo stesso valore
    // nel primo caso la prima operazione corrisponderebbe ad a ^= a,che porterebbe all'annullamento e alla perdita del valore originale
    // nel secondo caso semplicemente evita una operazione inutile,risparmiando 3 operazioni bitwise alla cpu
    {
        *a ^= *b; // a' adesso e' la diff
        *b ^= *a; // b diventa a
        *a ^= *b; // a diventa b
    }
}
