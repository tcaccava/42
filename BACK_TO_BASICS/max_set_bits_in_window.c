#include <stdio.h>
#include "./popcount_swar32.c"
// Dato un numero a 32 bit e una finestra di k bit consecutivi, trova la finestra che contiene il maggior numero di bit accesi.
// Applicazioni Pratiche: analisi di densità e burst detection.Mentre lo Spreading serve a mappare lo spazio e il Popcount puro misura l'entropia globale,
// la Sliding Window di Popcount serve a misurare la densità locale di informazione (o di errore) in un flusso continuo.
// -Rilevamento dei Burst di errore nelle telecomunicazioni (FEC): nei protocolli di rete e nelle trasmissioni RF, i disturbi raramente colpiscono un singolo bit
// isolato; si verificano invece i cosiddetti Burst Errors (interferenze che corrompono molti bit consecutivi).I sistemi di Forward Error Correction (FEC) usano questo
// algoritmo per scansionare il buffer di ricezione: se una finestra di k bit contiene una densità di bit impostati a 1 (che rappresentano i flag di errore dello schema
// di parità) superiore a una certa soglia, il sistema dichiara il frame non recuperabile e richiede la ritrasmissione (ARQ) prima ancora di tentare la decodifica completa.
// -Cifratura e Side-Channel Attacks (DPA/CPA): nelle analisi di sicurezza hardware, i crittoanalisti studiano il consumo energetico o l'emissione elettromagnetica di un
// chip che esegue algoritmi come l'AES. Il consumo è direttamente proporzionale al Hamming Weight (popcount) dei registri in transizione.Trovare la finestra temporale/bit
// con il massimo numero di bit accesi permette di isolare il momento esatto in cui la chiave segreta viene caricata nei registri transitori, permettendo attacchi di tipo
// Differential Power Analysis.
// -Compressione video e algoritmi di disparità (Stereo Matching): nelle pipeline di computer vision a bassissimo consumo, le immagini vengono convertite in feature binarie
// (es. Local Binary Patterns - LBP). Per calcolare la disparità tra l'occhio destro e l'occhio sinistro in un visore VR, l'hardware fa scorrere una finestra di bit per
// trovare la massima corrispondenza (massimo popcount dello XOR tra le due stringhe LBP). L'algoritmo di sopra ne è il nucleo accelerato.

// Implementazione Hardware (ASIC / FPGA):a differenza del software, dove l'algoritmo scorre sequenzialmente nel tempo lungo i 32 bit, l'hardware ci permette un bivio
// architetturale straordinario a seconda dei vincoli di design: sincrono (Seriale) o puramente combinatorio (Parallelo Spaziale).
// Opzione A: il mapping combinatorio parallelo (massimo throughput): se il parametro k è noto a tempo di compilazione (statico), l'hardware non esegue alcun ciclo.
// Genera fisicamente 32 - k + 1 piccoli alberi di popcount separati in parallelo sul silicio, ognuno collegato alla rispettiva finestra di fili.I risultati di tutti
// i popcount paralleli vengono convogliati in un albero di comparazione (un albero binario di comparatori e multiplexer) che sputa fuori il massimo in un singolo ciclo di clock.
// Vantaggio: latenza minima (1 ciclo di clock).Svantaggio: elevato consumo di area di silicio (moltiplicazione degli adder).
// Opzione B: il registro a scorrimento sincrono (ottimizzazione di area):s e vogliamo mappare esattamente la logica incrementale O(1) per risparmiare silicio, usiamo un'architettura
// sequenziale basata su un registro a scorrimento (Shift Register) e un accumulatore differenziale. Il dato a 32 bit entra in un registro. Due tap fisici (linee di rame) estraggono
// costantemente il bit che sta per abbandonare la finestra e quello che vi sta entrando.L'unità di aggiornamento: un piccolo sommatore/sottrattore a 6 bit aggiorna il contatore
// corrente ad ogni fronte di clock: count <= count + bit_In - bit_Out.Il registro di max: un comparatore branchless basato su multiplexer aggiorna il registro max se count > Max.

// Questo codice,per quanto estremamente efficiente,e' ancora perfettibile,perche' ad ogni iterazione del ciclo passo l'intera finestra estratta,per cui,per un ciclo che gira n volte,in funzione della lunghezza di k,
// eseguiro' n popcount completi.
int max_set_bits_in_window(unsigned int n, int k)
{
    if (k < 0)
        return 0;
    if (k >= 32)
        return popcount_swar32(n);
    int max = 0;
    unsigned int mask = (1U << k) - 1;
    for (int i = 0; i <= 32 - k; i++)
    {
        unsigned int window = (n >> i) & mask;
        int count = popcount_swar32(window);
        max = max ^ ((max ^ count) & -(count > max));
    }
    return max;
}

// In fondo non abbiamo bisogno di ricalcolare il popcount per ogni finestra,perche' avanzando nel ciclo di una sola unita',
// lo stato della window cambiera' solo alle estremita': un bit esce da destra(il precedente lsb della window) e uno entra da sinistra.
// Possiamo quindi calcolare il popcount una sola volta all'inizio e aggiornarlo in modo puramente incrementale guardando solo quei due bit,
// sempre in modo totalmente branchless come prima,ma passando da una complessita' temporale O(n) ad una O(1). Questa e' la versione SOTA.
// int max_set_bits_in_window(unsigned int n, int k)
// {
//     if (k <= 0)  return 0;
//     if (k >= 32) return popcount_swar32(n);
//     unsigned int mask = (1U << k) - 1;
//     int current_count = popcount_swar32(n & mask);
//     int max = current_count;

//     // Delta updates
//     for (int i = 1; i <= 32 - k; i++)
//     {
//         // Il bit che ESCE era in posizione (i - 1) nel numero originale
//         int bit_out = (n >> (i - 1)) & 1;

//         // Il bit che ENTRA è in posizione (i + k - 1) nel numero originale
//         int bit_in  = (n >> (i + k - 1)) & 1;

//         // Aggiorniamo il contatore per differenza
//         current_count = current_count - bit_out + bit_in;
//         max = max ^ ((max ^ current_count) & -(current_count > max));
//     }

//     return max;
// }

int main()
{
    printf("%d\n", max_set_bits_in_window(0xFFFF0000, 14));
}