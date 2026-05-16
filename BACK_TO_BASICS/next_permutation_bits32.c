#include <stdio.h>
// Gosper's Hack è un capolavoro di programmazione bitwise, accreditato a Bill Gosper e pubblicato nel leggendario HAKMEM (Memo 239 del MIT, 1972).
// Il suo scopo è generare, in tempo costante O(1) e senza alcun ciclo, la prossima permutazione lessicografica di bit avente lo stesso identico
// numero di bit accesi (peso di Hamming) dell'input.
// Per capire come la formula riesca a manipolare i bit in un colpo solo, dobbiamo scomporla nei tre fenomeni algebrici che avvengono nei registri a 32 bit.

// 1)Isolamento del Bit Meno Significativo (LSB)
// La prima operazione individua il bit a 1 più a destra del numero:
// c = n & -n
// Sfruttando la rappresentazione in complemento a due, l'operatore unario -n equivale a invertire tutti i bit di n e sommare 1 (ovvero ~n + 1).
// Quando sommi 1 a un numero invertito, tutti i bit a 1 a destra "scattano" e ritornano a 0, finché il riporto non incontra il primo 0 e lo trasforma in 1.
// Questo 1 corrisponde esattamente all'ultimo bit a 1 del numero originale. Di conseguenza, il numero originale e il suo negativo hanno un solo bit in comune: l'ultimo bit acceso a destra.
// L'operazione di AND cancella tutto il resto, isolando quel singolo bit (che sarà necessariamente una potenza di 2).

// 2)La Catena di Riporto (Carry Chain)
// Il passo successivo crea lo "scalino" superiore:
// r = n + c
// Sommando il bit isolato c al numero originale n, si innesca una reazione a catena(effetto ripple) di riporti binari. Se c'e' una catena di 1 consecutivi,il ripple azzera tutti i bit a 1 contigui.
// a partire dal LSB e imposta a 1 il primo bit a 0 immediatamente a sinistra di questo blocco. Abbiamo appena spostato il bit più significativo del blocco basso verso l'alto.
// Se avevamo un blocco di m bit a 1 consecutivi, la situazione adesso è questa:quei m bit a 1 sono diventati tutti zero.Un nuovo bit a 1 si è acceso a sinistra del blocco.
// Se W era il peso di Hamming del numero originale,dopo la somma il peso di Hamming di r sara' W - m + 1. Ma noi ne vogliamo esattamente W,quindi abbiamo perso per strada esattamente m - 1 bit accesi.

// 3)Il Colpo di Pettine (Spostamento a Destra)
// Per recuperarli e preservare lo stesso peso di Hamming e ottenere la permutazione immediatamente successiva, dobbiamo recuperare quei bit persi, sottrarne uno (quello che è scalato in alto in r)
// e posizionarli all'estrema destra del numero (le posizioni meno significative).
// Per recuperarli usiamo lo XOR tra il vecchio e il nuovo numero,che evidenzia solo i bit modificati,e nello specifico ad essere stati modificati sono stati gli m bit a 1(diventati zero) e il bit a 1 che
// si e' acceso alla loro sinistra. Quindi lo XOR ci restituisce un blocco solido di bit a 1 lungo esattamente m + 1. Quindi noi dobbiamo recuperare m - 1 bit ma lo xor ce ne da m + 1.
// Ce ne sono due di troppo,e questo e' il motivo per cui shiftiamo r ^ n a destra di due posizioni,cioe' lo dividiamo per 4.
// Questo blocco di bit recuperati si trova pero' fluttuante a meta' del registro. Per ottenere la permutazione lessicografico immediatamente successiva,la matematica ci dice che questi bit rimasti devono
// stare il piu' a destra possibile. Poiche' sappiamo che il blocco originario partiva dalla posizione del primo bit acceso da destra,cioe' c, dividendo il blocco per c,che e' una potenza di 2,shiftiamo matematicamente
// il numero a destra fino alla posizione zero. Infine uniamo con OR questo blocco ripulito e spostato a destra con lo scheletro del numero che avevamo calcolato con la somma iniziale n + c.
// La formula fa questo calcolo geometrico:
// next = ((r ^ n)/ 4c) | r

// r ^ n (XOR): produce una maschera di 1 che va dal vecchio LSB fino al bit appena acceso in r. Questa maschera contiene esattamente due bit a 1 in più rispetto al numero di bit che dobbiamo ripristinare.
// Dividere per c : poiché c è una potenza di 2, dividere per c equivale a effettuare un bitwise shift a destra pari alla posizione del vecchio LSB. Questo sposta la maschera all'estrema destra.
// Dividere per 4 compensa i due bit in più generati dallo XOR, scalando perfettamente il blocco di bit rimanenti a ridosso del bit zero.
// OR finale con r: fonde il blocco basso riposizionato con lo scheletro alto calcolato in r.

unsigned int next_permutation_bits32(unsigned int n)
{
    // Se l'input è 0, non ci sono bit da permutare.
    if (n == 0)
        return (0);

    // Isola il bit a 1 più a destra (LSB)
    unsigned int c = n & -n;

    // Genera la maschera di riporto strutturale superiore
    unsigned int r = n + c;

    // Gestione dell'overflow hardware: se i bit hanno raggiunto la massima
    // posizione a sinistra (es. 0xC0000000 per 2 bit), r diventerà minore o uguale a n.
    if (r <= n)
        return (0);

    // Ripristina i bit rimanenti portandoli all'estrema destra.

    unsigned int next = (((r ^ n) >> 2) / c) | r;

    return (next);
}

int main(void)
{
    // Generiamo tutte le combinazioni di 3 bit accesi su uno spazio di 5 bit (K=3, N=5)
    // Partiamo dal valore minimo possibile con 3 bit accesi: 7 (in binario: 00111)
    unsigned int pattern = 7;
    int count = 1;

    while (pattern != 0 && pattern < 32)
    {
        for (int i = 4; i >= 0; i--)
            printf("%d", (pattern >> i) & 1);
        printf("\n");

        pattern = next_permutation_bits32(pattern);
    }
    return (0);
}

// Tracciamento Matematico (Dry Run)

// Vediamo cosa succede nei registri quando passiamo alla funzione il numero n = 11 (che ha 3 bit accesi).

// n in binario (32 bit): `00...0001011
// Passo 1 (c = n & -n):
// -n in complemento a due: 11...1110101
// c = 00...0001011 & 11...1110101` ---> $ 00...0000001 (Isolato l'ultimo bit a destra).

// r = 11 + 1 = 12 ---> 00...0001100 (La catena di riporto ha spianato i due bit bassi e ha acceso il bit in terza posizione).

// r ^ n ---> 00...0001100 ^ 00...0001011 = 00...0000111(Isolati i bit alterati dal riporto).
// (r ^ n) >> 2 ---> 7 >> 2 = 00...0000001(Eliminati i due bit di offset).
// / c ---> 1 / 1 = 00...0000001 (In questo caso specifico lo shift dovuto a c è 0 posizioni).
// | r ---> 00...0000001 | 00...0001100 = 00...0001101 (Fusi insieme).

// Risultato Finale:13 (00...0001101).
// Il numero 13 ha esattamente 3 bit accesi, ed è matematicamente la permutazione lessicografica successiva di 11. Il tutto calcolato senza un solo ciclo `while` o `for`.
// unsigned int ft_next_permutation_bits32(unsigned int n)