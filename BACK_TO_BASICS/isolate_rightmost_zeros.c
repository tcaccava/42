#include <stdio.h>
// Isola la sequenza di zeri consecutivi più a destra prima del primo bit acceso e la accende. Es: 10110100 → 00000011.
// La formula si basa sulla convergenza di due fenomeni logico-aritmetici distinti: l'isolamento dell'ibrido e la propagazione del prestito (borrow).
// Quando un registro viene convertito nel suo valore negativo, l'hardware inverte tutti i bit e somma 1. Questo processo crea una simmetria perfetta:
// tutti i bit a sinistra del primo 1 invertito cambiano stato, il primo 1 rimane identico, e tutti gli zeri a destra rimangono zeri. L'operazione di AND logico
// tra il numero e il suo negativo cancella la spazzatura a sinistra, lasciando acceso solo e soltanto il bit attivo più a destra (il bit meno significativo, o LSB).
// Una volta che e' stato selezionato un registro che contiene un singolo bit isolato (una potenza pura di 2), la sottrazione di 1 agisce come un detonatore.
// Non essendoci altri bit accesi a destra, la CPU deve propagare un prestito attraverso tutte le posizioni inferiori. Questo trasforma istantaneamente il bit isolato in
// uno 0 e accende a 1 tutti gli zeri che si trovavano alla sua destra.
// Creare una maschera che identifica gli zeri finali è una colonna portante della programmazione di sistema:
// -Bit-Packing e Bitstream Parsing: nei decoder video o nella decompressione di indici di database (come le codifiche d'élite tipo Elias-Gamma o Huffman), i dati non sono
// allineati ai byte ma fluiscono come flussi continui di bit. Questa maschera serve a calcolare istantaneamente la dimensione del padding o i salti di parola prima del payload successivo.
// -Memory Allocator (Bitmap Search): nei gestori di memoria a basso livello che tracciano i blocchi liberi tramite una bitmap (dove 0 significa libero e 1 occupato), questa formula isola
// istantaneamente la prima sequenza di blocchi disponibili a inizio pagina, permettendo un'allocazione a costo quasi nullo.
// -Algoritmi di scorrimento veloci: viene usata per calcolare il fattore di allineamento di un puntatore hardware, determinando se un indirizzo di memoria è divisibile per una determinata
// potenza di due.
// A livello di silicio, il processore esegue questa riga consumando pochissime risorse, ma con una pipeline definita:
// La CPU esegue prima una negazione logica e un'addizione (per fare il meno unario), poi un'istruzione di AND logico, e infine una sottrazione. Su architetture moderne x86 o ARM, parliamo
// di una latenza complessiva di circa 2 o 3 cicli di clock, poiché le operazioni sono sequenziali e dipendono l'una dal risultato dell'altra.
// Nelle CPU moderne, questo specifico problema è così cruciale che i progettisti di hardware hanno inserito un circuito combinatorio dedicato chiamato Priority Encoder. Questo circuito
// analizza l'intero registro in parallelo tramite una cascata di porte logiche e determina la posizione dell'LSB in un solo ciclo di clock. Sulle CPU x86 questo circuito viene attivato dalle
// istruzioni hardware TZCNT (Trailing Zero Count) o BSF (Bit Scan Forward).

unsigned int isolate_rightmost_zeros(unsigned int n)
{
    return (n & -n) - 1;
}

int main()
{
    printf("%#x\n", isolate_rightmost_zeros(0x00010000)); // 0x0000FFFF
}