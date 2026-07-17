/*
 * =============================================================================
 * 1. BACKGROUND TEORICO: CONTEGGIO ORIZZONTALE VS ADDIZIONATORI VERTICALI SWAR
 * =============================================================================
 * Il problema del voto a maggioranza bitwise (Bitwise Majority Vote) richiede
 * di calcolare per ogni colonna i-esima di un set di N vettori binari se il
 * numero di bit accesi (popcount verticale) sia strettamente superiore a N/2.
 *
 * APPROCCIO ORIZZONTALE CLASSICO (O(32 * N)):
 * È l'approccio algoritmico più intuitivo. Si isola la colonna i-esima mediante
 * uno shift dinamico e una maschera, si cicla su tutti gli N elementi accumulando
 * il bit estratto in un contatore, e si verifica il quorum. Questo metodo soffre
 * di due colli di bottiglia microarchitetturali:
 *   1. Lo shift dinamico (<< i o >> i) introduce latenze di dipendenza.
 *   2. La CPU lavora su una sola colonna di bit alla volta, lasciando i restanti
 *      31 bit della word completamente inutilizzati durante il calcolo.
 *
 * APPROCCIO VERTICALE SWAR (O(N * log(N))):
 * Per ottimizzare l'operazione al livello del silicio, si ribalta la geometria
 * del calcolo utilizzando i "Contatori Verticali" (Vertical Counters o Sommatori
 * di Knuth). Invece di contare i bit orizzontalmente, usiamo i registri della
 * CPU come accumulatori paralleli di bit (ogni bit del registro 'C_0' rappresenta
 * il bit meno significativo del conteggio per quella specifica colonna).
 *
 * Sfruttando le equazioni logiche di un Full Adder (Addizionatore Completo):
 *   - Somma (S) = A ^ B ^ Rin
 *   - Riporto (Rout) = (A & B) | (Rin & (A ^ B))
 * È possibile far avanzare il conteggio di tutte le 32 colonne SIMULTANEAMENTE
 * con pochissime istruzioni bitwise (AND, XOR, OR), eliminando completamente
 * gli shift dinamici e processando i dati alla massima larghezza di banda del bus.
 *
 * =============================================================================
 * 2. APPLICAZIONI PRATICHE NEL SOFTWARE AD ALTE PRESTAZIONI
 * =============================================================================
 * I. ERROR CORRECTING CODES (ECC) E MAJORITY LOGIC DECODING (MLD)
 * -----------------------------------------------------------------------------
 * Nelle telecomunicazioni e nei sistemi di storage consumer/aerospaziale soggetti
 * a radiazioni o forti disturbi energetici (Single Event Upsets), i dati vengono
 * trasmessi con ridondanza. La decodifica a logica di maggioranza (MLD) permette
 * di ricostruire il messaggio originale corrotto confrontando N copie della
 * parola ricevuta. Questo algoritmo bitwise ripara istantaneamente gli errori
 * di bit-flip su 32 canali paralleli.
 *
 * II. TRIPLE MODULAR REDUNDANCY (TMR) E SISTEMI CRITICI ASIL-D
 * -----------------------------------------------------------------------------
 * Nei sistemi embedded automotive (Safety-Critical, es. Drive-by-Wire) o nei
 * controllori di volo, tre o più microcontrollori eseguono lo stesso calcolo in
 * parallelo. Un hardware o software di "voto" raccoglie gli output; se uno dei
 * nodi fallisce o calcola un dato errato a causa di un glitch, il majority vote
 * lo esclude senza interrompere l'operazione in tempo reale.
 *
 * III. ELABORAZIONE DI DATABASE COLONNARI E BITMAP AGGREGATIONS
 * -----------------------------------------------------------------------------
 * Nei motori analitici, operazioni di aggregazione di filtri booleani complessi
 * o calcoli di dominanza (es. algoritmi di classificazione o clustering basati
 * su feature binarie) sfruttano il popcount verticale per determinare la firma
 * di maggioranza di un set di record aggregati.
 *
 * =============================================================================
 * 3. ANALISI MICROARCHITETTURALE E IMPATTO HARDWARE
 * =============================================================================
 * I. IL VANTAGGIO DELLE ISTRUZIONI VETTORIALI (SIMD / AVX-512)
 * -----------------------------------------------------------------------------
 * Sebbene l'implementazione SWAR su registri generici a 32 bit sia altamente
 * efficiente e portabile, le CPU x86 moderne dotate di AVX-512 offrono estensioni
 * specifiche come `_mm512_popcnt_epi32` o istruzioni di compressione e riduzione.
 * Su ARM64 (Apple Silicon, ARM Neon), le istruzioni di popcount vettoriale (`CNT`)
 * permettono di calcolare la densità dei bit su interi vettori di registri a 128 bit
 * riducendo la necessità di implementare sommatori logici complessi in software.
 *
 * II. ANALISI DELLA PIPELINE SUL LOOP CORRETTO
 * -----------------------------------------------------------------------------
 * Nella versione corretta del loop tradizionale (`majority_vote_n`), l'uso
 * di un ciclo interno pulito `for (int j = 0; j < n; j++)` permette alla CPU di:
 *   1. Sfruttare l'Unrolling del ciclo (eseguito automaticamente da GCC -O3).
 *   2. Eseguire accessi sequenziali alla memoria (`arr[j]`), massimizzando
 *      l'efficienza del prefetcher hardware della cache L1.
 *   3. Evitare stalli da branch misprediction, poiché il limite dei cicli (N)
 *      è una costante fissa durante l'intera esecuzione della funzione.
 * =============================================================================
 */

#include <stdio.h>

/**
 * Complessità: O(32 * N). Approccio orizzontale sequenziale.
 */
unsigned int majority_vote_n(unsigned int *arr, int n)
{
    int quorum = (n / 2) + 1;
    unsigned int res = 0;

    /* Ciclo esterno stabile sulle 32 posizioni dei bit */
    for (int i = 0; i <= 31; i++)
    {
        int counter = 0;

        /* Ciclo interno pulito sugli N elementi dell'array */
        for (int j = 0; j < n; j++)
            counter += (arr[j] >> i) & 1;

        /* Se viene raggiunto il quorum, accendi il bit i-esimo nel risultato */
        res |= ((unsigned int)(counter >= quorum) << i);
    }
    return res;
}

/**
 * @brief Approccio Avanzato SWAR tramite Sommatori Verticali (Vertical Counters).
 * Complessità: O(N * log(N)) rispetto alle operazioni sui bit.
 *
 * Questa funzione conta i bit in verticale sfruttando registri come accumulatori
 * di bit. Supporta fino a un massimo di 7 elementi (N <= 7) usando solo 3 registri
 * di conteggio (v0, v1, v2) che fungono da contatore a 3 bit in base 2 per ogni colonna.
 *
 * Struttura dei contatori:
 *   v0: bit 0 del conteggio (peso 1)
 *   v1: bit 1 del conteggio (peso 2)
 *   v2: bit 2 del conteggio (peso 4)
 */
unsigned int majority_vote_swar_7(unsigned int *arr, int n)
{
    unsigned int v0 = 0; /* Peso 1 */
    unsigned int v1 = 0; /* Peso 2 */
    unsigned int v2 = 0; /* Peso 4 */

    /* Accumulazione verticale parallela via Full-Adder logici */
    for (int i = 0; i < n; i++)
    {
        unsigned int data = arr[i];

        /* Aggiornamento del bit di peso 4 (v2) tramite il riporto di v1 e data */
        v2 ^= (v1 & (v0 & data));

        /* Aggiornamento del bit di peso 2 (v1) tramite il riporto di v0 e data */
        v1 ^= (v0 & data);

        /* Aggiornamento del bit di peso 1 (v0) */
        v0 ^= data;
    }

    int quorum = (n / 2) + 1;
    unsigned int res = 0;

    /*
     * Ricostruzione finale del quorum senza loop di calcolo:
     * Estraiamo il valore finale del contatore per ogni bit combinando le maschere
     * v0, v1, v2 in base al valore binario del quorum richiesto.
     *
     * Esempio per N = 4 elementi -> Quorum = 3 (0b011 in binario).
     * Vogliamo scoprire dove il valore (v2 v1 v0) è >= 3.
     * Condizione matematica: v2 == 1 (valore >= 4) OPPURE (v1 == 1 E v0 == 1) (valore == 3).
     */
    if (quorum == 3)
        res = v2 | (v1 & v0);
    /* Esempio per N = 3 elementi -> Quorum = 2 (0b010 in binario). */
    else if (quorum == 2)
        res = v2 | v1;
    /* Fallback generico orizzontale se N non è nei limiti precalcolati dello SWAR veloce */
    else
        return majority_vote_n(arr, n);

    return res;
}

int main(void)
{
    /*
     * Data Set di Test (4 elementi):
     * arr[0] = 0xFFFFFFFF -> Tutti 1
     * arr[1] = 0x11111111 -> 1 solo nei nibble bassi (0, 4, 8, 12, 16, 20, 24, 28)
     * arr[2] = 0x10101010 -> 1 alternati
     * arr[3] = 0x00000000 -> Tutti 0
     *
     * Analisi Quorum (N = 4, Quorum = 3):
     * - Posizioni con tre 1: Bit 4, 12, 20, 28 (Presenti in arr[0], arr[1], arr[2]) -> Devono essere 1.
     * - Restanti posizioni con bit accesi (es: Bit 0 ha 1 in arr[0], arr[1] ma 0 in arr[2] -> totale 2) -> Sotto quorum, devono essere 0.
     *
     * Risultato atteso: 0x10101010
     */
    unsigned int arr[] = {0xFFFFFFFF, 0x11111111, 0x10101010, 0x00000000};
    int n = 4;

    unsigned int res_fixed = majority_vote_n(arr, n);
    unsigned int res_swar = majority_vote_swar_7(arr, n);

    printf("====================================================================\n");
    printf(" BITWISE VERTICAL MAJORITY VOTE REPORT\n");
    printf("====================================================================\n");
    printf("Numero elementi (N):      %d\n", n);
    printf("Quorum richiesto (>N/2):  %d\n\n", (n / 2) + 1);

    printf("[CALCOLO 1] Loop O(32*N) Corretto:   0x%08X\n", res_fixed);
    printf("[CALCOLO 2] SWAR Vertical Adder:     0x%08X\n\n", res_swar);

    return 0;
}