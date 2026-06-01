#include <stdio.h>
#include "./reverse_bits_32.c"

// Determina se la configurazione binaria di un intero a 32 bit è palindroma (leggibile nello stesso modo da sinistra a destra).
//
// BACKGROUND TEORICO & EFFICIENZA:
// L'approccio tradizionale prevede di scansionare il numero dai due lati con due puntatori/indici hardware che convergono al centro,
// introducendo salti condizionali (branch) ad ogni bit verificato.
// Sfruttando la funzione SWAR 'reverse_bits32', il controllo diventa O(1) e TOTALMENTE BRANCHLESS.
// Non ci sono jump strutturali: la CPU esegue l'inversione parallela e un singolo confronto di uguaglianza ('==').
// Se il numero originario è identico al suo specchiato, l'uguaglianza logica restituisce 1, altrimenti 0.
//
// APPLICAZIONI PRATICHE:
// - Crittografia e hashing: Generazione di pattern simmetrici distribuiti o validazione rapida di token crittografici basati su proprietà geometriche dei bit.
// - Teoria dei Codici ed Error Detection: Rilevamento di distorsioni di fase speculari nei flussi di dati seriali ad altissima frequenza, dove i pacchetti corrotti
//   mostrano inversioni simmetriche dovute a riflessioni del segnale elettrico sulle linee di trasmissione.
int is_palindrome32(unsigned int n)
{
    return n == reverse_bits32(n);
}

int main()
{
    // 0x80000001 = 10000000 00000000 00000000 00000001 -> Palindromo reale (Ritorna 1)
    // 0x00000001 = 00000000 00000000 00000000 00000001 -> Non palindromo, invertito diventa 0x80000000 (Ritorna 0)
    printf("%d %d\n", is_palindrome32(0x80000001), is_palindrome32(0x00000001));
}