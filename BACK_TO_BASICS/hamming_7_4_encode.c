#include <stdio.h>

// Il codice di Hamming 7 4 e' un sistema di correzione d'errore di tipo SEC
// ovvero single error correction. Prende un blocco di 4 bit di dati netti
// e genera un blocco finale di 7 bit inserendo 3 bit di parita'.
// Questo formato e' lo standard fondamentale nei sistemi in cui i guasti
// indotti da radiazioni o interferenze elettromagnetiche colpiscono un solo
// bit alla volta. I campi di utilizzo storici e moderni includono le memorie
// ram di tipo ecc (error correcting code), i sistemi di storage enterprise,
// le comunicazioni satellitari a corto raggio e i canali di bus interni
// dei microprocessori dove la latenza di rinvio del pacchetto e' inaccettabile.

// La struttura matematica si basa sullo spazio lineare dei vettori e sulle
// matrici di controllo di parita' in algebra booleana modulo 2. La posizione
// geometrica dei bit all'interno del byte finale segue rigorosamente le
// potenze di due per i bit di parita' (posizioni 1, 2, 4) mentre i bit dei
// dati occupano i restanti indici (posizioni 3, 5, 6, 7).
// Questa precisa disposizione geometrica fa si' che le aree di controllo
// dei tre bit di parita' si sovrappongano secondo una logica a diagramma di Venn.
// L'indice binario di ogni posizione (da 1 a 7) rivela quali parita' lo controllano:
// la posizione 3 (binario 011) e' controllata da p1 e p2.
// la posizione 5 (binario 101) e' controllata da p1 e p3.
// la posizione 6 (binario 110) e' controllata da p2 e p3.
// la posizione 7 (binario 111) e' controllata da tutti e tre (p1, p2, p3).
// In sostanza il bit di parità 1 controlla i bit la cui posizione in binario ha il bit 0 acceso(3,5,7)
// Il bit di parità 2 controlla i bit la cui posizione in binario ha il bit 1 acceso(3,6,7)
// Il bit di parità 3 controlla i bit la cui posizione in binario ha il bit 2 acceso(5,6,7)
// Nella programmazione allo stato dell'arte per cpu general purpose, l'algoritmo
// evita completamente salti condizionali (if, else) sfruttando l'operatore xor.
// Il dato originale a 8 bit viene ridotto a 4 bit tramite mascheratura & con 0x0f.
// Il posizionamento dei bit di dati avviene in un'unica operazione sfruttando la
// distanza geometrica costante di 4 posizioni tra lo stato iniziale e finale dei bit
// superiori (shift uniforme << 4). La normalizzazione dei singoli bit tramite
// isolamento and e successivo shift a destra garantisce che ogni operazione di xor
// logico avvenga su valori booleani puri (0 o 1), prevenendo errori di peso binario.

// A livello hardware nativo, il codice di hamming viene eseguito in un singolo
// ciclo di clock tramite reti di porte logiche xor cablate direttamente nel silicio
// dei controllori di memoria ram ecc. I circuiti integrati calcolano la parita'
// in parallelo durante la fase di scrittura. In fase di lettura, i circuiti calcolano
// i tre bit di controllo generandone la sindrome. Se la sindrome e' diversa da zero,
// il valore binario della sindrome stessa attiva una porta logica not sulla linea
// del bit corrotto, invertendolo e correggendolo istantaneamente prima che il dato
// raggiunga i registri della cpu.

void hamming_encode(unsigned char data, unsigned char *out)
{
    data &= 0x0F;                                                                         // isolo il nibble basso
    *out = 0;                                                                             // azzero out
    *out = ((data & 1) << 3) | ((data & 2) << 4) | ((data & 4) << 4) | ((data & 8) << 4); // i bit del nibble basso di data vanno in posizione 3,5,6,7 di out
    *out |= ((data & 1) ^ ((data & 2) >> 1) ^ ((data & 8) >> 3)) << 1;                    // il primo bit di parità in posizione 1 di out
    *out |= ((data & 1) ^ ((data & 4) >> 2) ^ ((data & 8) >> 3)) << 2;                    // secondo bit di parità in posizione 2
    *out |= (((data & 2) >> 1) ^ ((data & 4) >> 2) ^ ((data & 8) >> 3)) << 4;             // terzo in posizione 4
    // il bit 0 rimane impostato a zero per convenzione
}

// int main()
// {
//     unsigned char out;
//     hamming_encode(213, &out);
//     printf("%u\n", out);
// }