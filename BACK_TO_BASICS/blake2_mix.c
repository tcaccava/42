#include <stdio.h>
/*
Il Quarter-Round di BLAKE2: la Filosofia ARX e il grafo di permutazione.
La funzione blake2_mix si basa interamente sul paradigma ARX(Addition, Rotation, XOR). A differenza di algoritmi classici come l'AES, che utilizzano le S-Box
(tabelle di sostituzione in memoria) per garantire la confusione dei dati, gli algoritmi ARX usano solo operazioni primitive dell'ALU.
-Addizione (+): fornisce la non-linearità matematica (fondamentale per spezzare la crittanalisi lineare).
-Rotazione (ROL):diffonde i bit modificati all'interno della stessa parola numerica.
-XOR (^):fornisce una combinazione lineare ad alta velocità senza propagazione di riporti.

Dalla Cifratura all'Hashing: l'eredità di ChaCha20
Questa specifica funzione è quasi identica al quarter-round del cifrario a flusso ChaCha20 progettato da Daniel J. Bernstein. La differenza cruciale risiede nell'
introduzione delle variabili esterne x e y.
Mentre ChaCha20 fa girare lo stato interno su se stesso in modo isolato per generare una sequenza pseudorandomica, BLAKE2 prende i blocchi del messaggio da hashare (x e y) e 
li inietta di forza direttamente nel cuore delle addizioni. Questo trasforma una funzione di permutazione in una funzione di compressione crittografica altamente resistente 
alle collisioni.

Lo stato interno di BLAKE2 è una matrice 4 * 4 di interi a 32 bit (per BLAKE2s) o a 64 bit (per BLAKE2b), rappresentata da un vettore v di 16 elementi. 
La funzione riceve quattro indici di questo vettore (a, b, c, d), che rappresentano una colonna o una diagonale della matrice.
L'algoritmo si divide in due macro-fasi identiche nella struttura, ma asimmetriche nei valori di shift e negli input del messaggio.
Fase 1: iniezione di x (Rotazioni a 16 e 12 bit)
v[a] += v[b] + x; --> il valore v[b] e il pezzo di messaggio x entrano in v[a]. L'addizione classica propaga i riporti verso i bit più significativi.
v[d] = ROL(v[d] ^ v[a], 16); --> si fa lo XOR tra il nuovo v[a] e v[d], poi si ruota di 16 posizioni. Su un intero a 32 bit, ruotare di 16 significa scambiare esattamente i due byte alti con i due byte bassi, una mossa devastante per la localizzazione del dato.
v[c] += v[d]; --> la perturbazione si propaga su v[c].
v[b] = ROL(v[b] ^ v[c], 12); --> lo XOR e la rotazione di 12 bit distribuiscono il caos generato nei passaggi precedenti anche su v[b].

Fase 2: iniezione di y (Rotazioni a 8 e 7 bit)
Ora la catena di dipendenze si ripete, ma rimescolando le carte:
v[a] += v[b] + y; --> viene iniettato il secondo pezzo di messaggio y.
v[d] = ROL(v[d] ^ v[a], 8); --> rotazione di 8 bit (allineamento perfetto ai confini dei byte).
v[c] += v[d]; --> ulteriore accumulo su v[c].
v[b] = ROL(v[b] ^ v[c], 7); --> l'ultima rotazione di 7 bit (un numero dispari non allineato alle potenze di due) serve a spezzare definitivamente la simmetria dei bit, distruggendo qualsiasi pattern geometrico residuo.
Al termine di questi 8 passaggi, anche la minima variazione di un singolo bit in x, y o in uno dei vettori originari si sarà propagata (effetto valanga) su tutti e quattro gli elementi v[a], v[b], v[c], v[d].

Applicazioni pratiche: lo standard di velocità e sicurezza
Grazie all'efficienza di questa funzione di mix, BLAKE2 è diventato lo standard di fatto ovunque la velocità software sia un fattore critico:
-WireGuard: il protocollo VPN moderno integrato nel kernel Linux utilizza BLAKE2s per l'hashing dei pacchetti e la derivazione delle chiavi, garantendo latenze inferiori rispetto allo SHA-256 tradizionale.
-libsodium: la libreria crittografica di riferimento per gli sviluppatori software espone BLAKE2b come funzione di hashing predefinita (crypto_generichash).
-Sistemi di file e autenticazione: utilizzato nei moduli di integrità di file system avanzati (come ZFS o Btrfs) e nei gestori di password per verificare l'integrità dei dati a tempo di record.

Implementazione Hardware e software: il trionfo del parallelismo
Dal punto di vista architetturale, la funzione G è un capolavoro di ottimizzazione per le CPU moderne:
Ottimizzazione Software (SIMD / Vectorization):se si guarda la funzione da vicino, c'e' una dipendenza sequenziale stretta (l'operazione 2 dipende dalla 1, la 3 dalla 2, ecc.). Sembrerebbe un collo di bottiglia per la pipeline 
della CPU.Tuttavia, BLAKE2 esegue questo mix su quattro colonne indipendenti contemporaneamente, oppure su quattro diagonali indipendenti contemporaneamente.
Questo significa che uno sviluppatore low-level può mappare l'intero stato dentro i registri vettoriali della CPU (SSE4.1, AVX2, o AVX-512). Le CPU moderne possiedono istruzioni hardware dedicate alla rotazione vettoriale 
(come _mm_roti_epi32 in AVX-512 o emulazioni ultra-rapide tramite shift combinati in AVX2), permettendo di calcolare quattro istanze di blake2_mix in parallelo nello stesso identico ciclo d'orologio.
Semplicità Hardware (FPGA / ASIC):a livello hardware puro, non essendoci memorie S-Box da interrogare (che richiederebbero un'architettura bus e cicli di latenza per l'accesso alla cache), il circuito si traduce in un blocco di silicio puramente combinatorio.
-Le addizioni usano addizionatori standard a propagazione rapida.
-Lo XOR richiede una singola barriera di porte logiche elementari.
-La rotazione (ROL) in hardware è gratis: non richiede transistor o porte logiche, ma solo un re-indirizzamento fisico dei fili di rame (un hardwired shifting) che collegano l'uscita dello XOR all'ingresso dell'addizionatore successivo. 
Questo riduce l'area del chip e abbatte i consumi energetici a livelli minimi.
*/

#include <stdio.h>

// Rotazione circolare a sinistra a 32-bit, sicura e type-safe
static inline unsigned int rol32(unsigned int val, unsigned int shift)
{
    return ((val << (shift & 31)) | (val >> ((-shift) & 31)));
}

void fblake2_mix(unsigned int *v, int a, int b, int c, int d, unsigned int x, unsigned int y)
{
    // Primo Round: Iniezione di x
    v[a] += v[b] + x;
    v[d] = rol32(v[d] ^ v[a], 16);
    v[c] += v[d];
    v[b] = rol32(v[b] ^ v[c], 12);

    // Secondo Round: Iniezione di y
    v[a] += v[b] + y;
    v[d] = rol32(v[d] ^ v[a], 8);
    v[c] += v[d];
    v[b] = rol32(v[b] ^ v[c], 7);
}

int main(void)
{
    unsigned int v[16] = {
        0x11111111, 0x22222222, 0x33333333, 0x44444444,
        0x55555555, 0x66666666, 0x77777777, 0x88888888,
        0x99999999, 0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC,
        0xDDDDDDDD, 0xEEEEEEEE, 0xFFFFFFFF, 0x00000000
    };

    ft_blake2_mix(v, 0, 4, 8, 12, 0xDEADBEEF, 0xCAFEBABE);

    printf("v[0]:  0x%08X\n", v[0]);
    printf("v[4]:  0x%08X\n", v[4]);
    printf("v[8]:  0x%08X\n", v[8]);
    printf("v[12]: 0x%08X\n", v[12]);
}