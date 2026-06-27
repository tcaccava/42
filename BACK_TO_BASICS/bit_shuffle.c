/*
 * =====================================================================================
 * BIT_SHUFFLE - P-BOX CRITTOGRAFICA (PERMUTAZIONE BITWISE)
 * =====================================================================================
 *
 * BACKGROUND TEORICO:
 * La funzione implementa una P-Box (Permutation Box), componente fondamentale nei
 * cifrari a blocchi (DES/AES).A differenza delle S-Box(Substitution Boxes) l
 * a sua funzione non è modificare il valore dei bit,ma attuare la "diffusione":
 * ridistribuire spazialmente i bit di input in nuove
 * posizioni di output per annullare correlazioni statistiche. La funzione
 * associa ogni indice i del registro di output al bit sorgente perm[i] del
 * registro di input n,trasformando il registro da semplice contenitore del valore numerico
 * a un vettore di posizioni indipendenti. E' un pilastro teorico fondamentale che impedisce
 * la crittoanalisi differenziale e lineare: senza una adeguata permutazione,il cifrario sarebbe
 * facilmente scomponibile in sotto blocchi indipendenti.
 *
 * APPLICAZIONI PRATICHE:
 * - Crittografia: Secondo i criteri di Claude Shannon, un buon sistema crittografico deve garantire Confusione e Diffusione.
 * La confusione e' rendere la relazione tra chiave e testo cifrato quanto piu' oscura possibile,ovvero impedire ad un attaccante di
 * capire come la chiave influenzi il processo di cifratura. Si realizza tramite le S-Box,che operano una trasformazione non lineare
 * sostituendo gruppi di bit in input con altri bit in output,secondo la logica che se cambi un solo bit della chiave,il risultato cifrato
 * deve cambiare n modo imprevedibile e caotico. In sostanza la confusioe e' cio' che rende inutile la crittoanalisi lineare e differenziale semplice.
 * La diffusione ha invece lo scopo di distribuire l'influenza di ogni singolo bit del testo i chiaro su tutto il risultato cifrato,impedendo ad un attaccante
 * di dedurre schemi statistici da quest'ultimo(come la frequenza delle lettere o la ridondanza della lingua). La diffusione si tralizza tramite le P-Box e il mixing lineare(shift).
 * Quindi in sintesi la confusione si occupa della relazione chiave-cifrato,la rende oscura attraverso la non linearita',sfruttando le S-box,con lo scopo finale di proteggere e nascondere
 * la chiave. La diffusione si occupa di rendere oscura la relazione tra testo da cifrae e testo cifrato,la realizza attraverso il mescolamento spaziale,sfruttando le P-box e lo shift,con lo
 * scopo finale di nascondere e rendere difficilmente accessibili le informazioni e le statistiche sul testo in chiaro.
 * In sostanza la confusione serve a rendere astratta la chiave,evitando di ricavarla da una analisi delle relazione tra testo in chiaro e cifrato.
 * La diffusione rende astraggio il cifrato: se cambia un solo bit di input nel testo da cifrare l'intero cifrato diventa completamente diverso,impedendo di isolare l'informazione rappresentata da
 * quel singolo bit di input nel cifrato.
 * Un moderno cifrario deve implementare,generalmente ripetute in cicli multipli(round),sia la diffusione che la confusione,perche' nessuna delle due
 * prese singolarmente basta a fermare un attacco moderno.
 * Questa funzione bit_shuffle e' il motore della diffusione: serve alla costruzione di round di cifratura che garantiscono il "effetto valanga"
 * (un singolo bit in input cambia metà dei bit in output). Nel DES le permutazioni iniziali e finali
 * e le P-Box interne al round sono essenziali per sparpagliare i bit del testo in chiaro. Senza di esse
 * la crittografia sarebbe vulnerabili a semplici attacchi di frequenza o pattern.
 * - Digital Signal Processing (DSP): Algoritmi di bit-reversal necessari per le
 * trasformate veloci di Fourier (FFT). Interleaving per la correzione d'errore (FEC): nei sistemi di trasmissione dati,
 * si usa la permutazione per "distanziare" i bit di un messaggio. Se si verifica un burst di rumore
 * (che corrompe bit adiacenti), il de-interleaving in ricezione spalma gli errori, permettendo ai codici di correzione
 * (come i codici Reed-Solomon) di recuperarli facilmente.
 * - Hardware Design: Creazione di pattern di test per memorie e generatori di numeri pseudo-casuali (PRNG).
 * Molti generatori di segnali di test hardware utilizzano permutazioni fisse per "mescolare" l'output di un LFSR
 * (Linear Feedback Shift Register), garantendo che la sequenza appaia casuale in ogni sua parte.
 *
 * IMPLEMENTAZIONI INDUSTRIALI:
 * - ASIC/FPGA: Realizzate tramite routing fisico delle piste di rame tra i registri(latenza zero).Non costa alcun
 * ciclo di clock,e' pura propagazione del segnale elettrico nel silicio disegnato ad hoc.
 * - x86_64: Con l'estensione BMI2 queste operazioni sono accelerate dall'istruzione hardware 'PDEP' (Parallel Bits Deposit) che
 * esegue la permutazione in un singolo ciclo di clock usando,invece di iterare, maschere di permutazione
 * con cui il processore sposta tutti i bit in una singola operazione hw a livello di ALU.
 * - ARM/SIMD: Ottimizzate tramite istruzioni di shuffle vettoriale (es. VTBL/PSHUFB,Permute Shuffle Byte).
 * Anche se queste operano solitamente a livello di byte, con opportune tecniche di maschere si possono manipolare
 * i singoli bit in parallelo, permettendo di processare blocchi enormi (fino a 512 bit) in un unico colpo di clock.
 * * =====================================================================================
 */

#include <stdio.h>

/**
 * @brief Permuta i bit di n secondo l'array perm.
 * @param n Numero sorgente.
 * @param perm Array di indici che definisce la posizione sorgente per ogni bit.
 * @param bits Numero di bit da processare.
 * @return Numero con i bit permutati.
 */
unsigned int bit_shuffle(unsigned int n, int *perm, int bits)
{
    unsigned int res = 0;
    for (int i = 0; i < bits; i++)
    {
        /* * LOGICA OPERATIVA:
         * 1. GATHER: (n >> perm[i]) & 1 : Isola il bit sorgente nella posizione indicata da perm[i].
         * 2. ALIGNMENT: << i              : Sposta il bit isolato nella destinazione i.
         * 3. MERGE: res |= ...        : Accumula il bit nel registro di output.
         */
        int index = perm[i];
        res |= ((n >> index) & 1) << i;
    }
    return res;
}

int main(void)
{
    // Test: 11 (0b1011) con perm {0, 2, 1, 3} -> 13 (0b1101)
    unsigned int n = 0b1011;
    int perm[4] = {0, 2, 1, 3};

    printf("Risultato shuffle: %u\n", bit_shuffle(n, perm, 4));

    return 0;
}