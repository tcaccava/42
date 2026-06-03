#include <stdio.h>

/*
 * BACKGROUND TEORICO:
 * L'algoritmo implementa un Parallel Prefix Scan (noto anche come Bit Smearing)
 * basato sull'operatore logico OR. Invece di scansionare i bit linearmente con un
 * costo O(N), sfrutta il principio del raddoppio logaritmico (pointer jumping)
 * per abbattere la complessità a O(log2 W), dove W è l'ampiezza del registro (32 bit).
 * Ogni stadio duplica l'ampiezza della propagazione: lo shift di 1 propaga il bit
 * al vicino immediato, creando coppie di bit attivi; lo shift di 2 propaga le coppie
 * creando blocchi di 4, e così via (4, 8, 16). Il primo bit acceso a sinistra (MSB)
 * agisce come un fronte d'onda, generando una reazione a catena che satura a 1
 * tutte le posizioni logiche alla sua destra, lasciando invariati i bit a sinistra.
 *
 * APPLICAZIONI PRATICHE:
 * 1. Calcolo della prossima potenza di 2: Sommando 1 al registro saturato da questa
 * funzione, si innesca un overflow controllato che azzera la scia di 1 e accende
 * esattamente il bit successivo (es. da 00111111 a 01000000). È una primitiva
 * critica nei Buddy Allocator dei kernel dei sistemi operativi per l'allineamento
 * delle pagine di memoria.
 * 2. Motori di scacchi (Bitboards): Nello sviluppo di motori computazionali di scacchi (stile Stockfish),
 * la scacchiera è rappresentata da registri a 64 bit (Bitboards). La propagazione parallela del prefisso
 * viene usata per generare istantaneamente le "maschere di attacco" o i raggi X delle torri e delle regine
 * lungo le righe o le diagonali, identificando le caselle controllate fino al primo ostacolo.
 * 3. Emulazione CLZ (Count Leading Zeros): Su architetture embedded sprovviste di
 * un'istruzione hardware nativa per contare gli zeri in testa(come clz o bsr), lo smearing
 * permette di isolare l'MSB invertendo il dato (~n) e calcolandone l'Hamming weight tramite il popcount.
 * 4. Database colonnari: Bit-packing e compressione al volo per determinare quanti
 * bit minimi servano a memorizzare un intero escludendo il padding superiore.
 *
 * IMPLEMENTAZIONE HARDWARE:
 * Nel silicio di una ALU, questa logica prefissa viene integrata tramite reti
 * combinatorie parallele chiamate Parallel Prefix Networks, strutturate secondo
 * topologie ad albero come l'albero di Kogge-Stone (ottimizzato per la minima
 * latenza di gate,minimizza il numero di porte logiche attraversate,o gate delays,al costo
 * di un elevato numero di interconnessioni fisiche(wire routing),e un alto fan out. La propagazione
 * avviene in parallelo puro,ed e' la scelta per le ALU ad altissima frequenza)
 * o l'albero di Brent-Kung (ottimizzato per risparmiare area,wire routing
 * consumi sul chip,paga lo scotto di una latenza superiore,con circa il doppio degli stadi di porte logiche
 * rispetto a Kogge-Stone).È lo stesso identico circuito che governa il calcolo del
 * riporto negli addizionatori veloci Carry Look-Ahead (CLA).
 * Negli addizionatori si calcolano i vettori di Generazione (G) e Propagazione (P) del riporto hardware;
 * l'algoritmo di parallel prefix OR non è altro che una versione degenerata di un addizionatore CLA,
 * dove la funzione di generazione è legata esclusivamente alla presenza del bit 1 originario e la
 * propagazione è una costante logica.
 */

/*
 * Analisi della variante con ciclo While:
 * L'utilizzo della condizione (shift ^ 32) arresta il ciclo non appena la variabile
 * raggiunge il valore 32. Lo spostamento dell'istruzione di shift a fondo ciclo
 * garantisce la corretta sequenza logaritmica (1, 2, 4, 8, 16). Rispetto alla
 * versione unrolled, questa variante introduce un sovraccarico (overhead) dovuto
 * alla gestione del contatore nei registri e al salto condizionale (branch).
 */
unsigned int parallel_prefix_or_loop(unsigned int n)
{
    int shift = 1;
    while (shift ^ 32)
    {
        n |= n >> shift;
        shift <<= 1;
    }
    return n;
}

/*
 * Analisi della variante con Loop Unrolling:
 * Rappresenta la soluzione ottimale per sistemi ad alte prestazioni. Eliminando il
 * ciclo, si azzera la dipendenza da branch condizionali, disinnescando alla radice
 * il rischio di Branch Misprediction nella pipeline della CPU. Il codice si traduce
 * in appena 5 istruzioni assembly consecutive di shift e OR, massimizzando l'Instruction
 * Level Parallelism (ILP) e minimizzando i cicli di clock totali.
 */
unsigned int parallel_prefix_or(unsigned int n)
{
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return n;
}

int main()
{
    printf("%08x\n", parallel_prefix_or(0x000F0000));
    return 0;
}
