#include <stdio.h>
// La funzione di Message Schedule è la fase di espansione dell'input dell'algoritmo SHA-256. Il blocco di messaggio iniziale da 512 bit
// viene diviso in 16 parole (words) da 32 bit ciascuna (w[0] fino a w[15]). L'algoritmo di compressione centrale richiede però 64 round
// totali, motivo per cui è necessario espandere queste 16 parole originarie nelle successive 48 parole (w[16]$ fino a w[63]).
// La formula di ricorrenza rompe le relazioni lineari tra i bit del messaggio sfruttando i principi di confusione e diffusione di Shannon:
// -Le funzioni di miscelazione sigma_0 e sigma_1 combinano la rotazione bitwise a destra (ROTR) con lo shift logico a destra (SHR) tramite l'operatore XOR.
// Lo shift logico introduce zeri a sinistra, rompendo la simmetria biunivoca della rotazione pura e distruggendo la conservazione della Hamming weight.
// La combinazione di tre distanze di shift/rotazione diverse fa sì che la modifica di un singolo bit nell'input iniziale si propaghi in modo esponenziale (effetto valanga)
// nelle parole successive.
// -La ricorrenza temporale: per calcolare la parola corrente w[i], l'algoritmo non guarda solo le parole immediatamente precedenti, ma attinge a quattro distanze storiche
// precise della timeline di espansione (i-16, i-15, i-7, i-2). Questa specifica combinazione di distanze (scelta empiricamente dai progettisti della NSA) garantisce che le
// proprietà di massima diffusione vengano raggiunte nel minor numero di stadi possibile, rendendo l'estensione del messaggio matematicamente immune ad attacchi di crittoanalisi
// differenziale lineare.

// L'espansione del messaggio è il motore che alimenta l'intera infrastruttura di sicurezza moderna basata su SHA-256:
// -Integrità del Blocco in Bitcoin (Proof-of-Work):il mining di Bitcoin esegue un doppio SHA-256 sull'intestazione del blocco. I chip ASIC dedicati passano oltre il 70% del loro tempo
// computazionale e del loro consumo energetico a calcolare questa espansione del messaggio e i successivi round di compressione per trovare un hash inferiore al target richiesto.
// -Protocolli di rete sicuri (TLS/SSL e SSH): utilizzato per derivare le chiavi di sessione simmetriche a partire dal Pre-Master Secret scambiato durante l'handshake. Se l'espansione del
// messaggio introducesse debolezze lineari, un attaccante potrebbe ricostruire parti del messaggio originale a partire dal digest osservato.
// -Firme Digitali e certificati (RSA/ECDSA): prima di firmare digitalmente un file o un certificato HTTPS, il payload viene ridotto a un'impronta SHA-256. L'efficacia della firma
// dipende totalmente dall'impossibilità di trovare collisioni durante la schedulazione del messaggio.

// In un'architettura hardware (FPGA o ASIC custom), la forza strutturale di questo algoritmo risiede nel fatto che le operazioni più complesse dal punto di vista software diventano
// virtualmente gratuite sul silicio.
// -Rotazioni e Shift a costo zero: nello spazio fisico del silicio, un'operazione di ROTR(x, 7) o x >> 3 non richiede porte logiche né cicli di clock. Si implementa tramite hard-wiring
// (instradamento fisico dei cavi). Ad esempio, per ruotare di 7 posizioni a destra, il pin di output del bit 7 viene fisicamente saldato al pin di input del bit 0 del modulo successivo.
// Lo shift logico si esegue collegando a massa (messa a terra, quindi a 0 logico) i bit più significativi.
// -Operazioni XOR in parallelo: l' applicazione dello XOR richiede un singolo strato di porte logiche XOR a due ingressi eseguite in parallelo puro su tutti i 32 bit della parola.
// La latenza hardware è minima (un solo gate delay).
// Il collo di bottiglia dell'addizione (Carry-Save Adders): la vera sfida hardware è l'addizione di quattro operandi da 32 bit modulo 2^32. In software di farebbero tre addizioni
// sequenziali. In hardware, l'uso di un addizzatore classico a propagazione di riporto (Carry-Propagate Adder) per tre volte di fila creerebbe un ritardo intollerabile a causa della catena
// dei riporti che deve viaggiare dal bit meno significativo a quello più significativo.
// Per ovviare a questo, i progettisti hardware usano un albero di Carry-Save Adders (CSA): un modulo CSA accetta tre ingressi a 32 bit e produce in output due vettori (un vettore di somme e uno di riporti)
// con la latenza di un singolo flip-flop, senza propagare il riporto lungo la catena. Con due stadi di CSA, i quattro operandi della formula vengono ridotti a due soli vettori intermedi,
// i quali vengono infine sommati da un addizionatore veloce finale (come un Carry-Lookahead Adder).
// Gestione del modulo: l'operazione mod 2^32 non richiede hardware di divisione: l'overflow del 33esimo bit viene semplicemente ignorato lasciando cadere il filo del riporto finale nel vuoto.

/*
 * Ruota i bit di un intero a 32 bit verso destra di n posizioni.
 * Essendo static inline, il compilatore la inietterà direttamente
 * nel punto di chiamata eliminando il salto hardware della CPU.
 */
static inline unsigned int rotr(unsigned int x, unsigned int n)
{
    return ((x >> n) | (x << (32 - n)));
}

/*
 * Funzione di miscelazione Sigma 0 per lo standard SHA-256.
 * Sfrutta la funzione rotr definita sopra mantenendo la sicurezza dei tipi.
 */
static inline unsigned int sigma0(unsigned int x)
{
    return (rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3));
}

/*
 * Funzione di miscelazione Sigma 1 per lo standard SHA-256.
 */
static inline unsigned int sigma1(unsigned int x)
{
    return (rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10));
}

/*
 * Calcola la parola w[i] della message schedule di SHA-256.
 * Prende in input il puntatore all'array delle parole e l'indice corrente.
 */
void sha256_schedule(unsigned int *w, int i)
{
    w[i] = w[i - 16] + sigma0(w[i - 15]) + w[i - 7] + sigma1(w[i - 2]);
}
