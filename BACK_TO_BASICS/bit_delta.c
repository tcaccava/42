#include <stdio.h>
#include "./popcount_swar32.c"
/*============================================================================
 * 1. BACKGROUND TEORICO: RILEVAMENTO DELLE TRANSIZIONI (EDGE DETECTION)
 * =============================================================================
 * Come si contano i gruppi contigui di bit accesi in modo efficiente?
 * Ciascun gruppo contiguo di 1 ha una "testa" (il bit più a destra del gruppo)
 * che è caratterizzata da una condizione logica unica:
 *   1. Il bit attuale deve essere 1.
 *   2. Il suo vicino di destra originale deve essere 0
 *
 * Per isolare questa testa con una porta AND (&), dobbiamo trasformare quel vicino
 * 0 in un 1, altrimenti l'AND lo spegnerebbe inevitabilmente. È qui che entra
 * in gioco l'inversione bitwise NOT (~) applicata allo shift.
 * Se noi prendiamo la maschera delle differenze X e la compariamo con se stessa
 * shiftata di una posizione (X << 1), possiamo isolare matematicamente solo
 * il "fronte d'inizio" (il bit più a sinistra) di ogni singolo gruppo.
 *
 * Formula magica SWAR in O(1):
 *   mask = X & ~(X << 1);
 *
 Tracciamento meccanico passo-passo nel silicio su un gruppo (es. 0b00111000):
 *
 * 1. IL DATO ORIGINALE (X):
 *    Posizioni bit:  7 6 5 4 3 2 1 0
 *    X             = 0 0 1 1 1 0 0 0   (Il gruppo occupa i bit 5, 4, 3)
 *
 * 2. LO SHIFT A SINISTRA (X << 1):
 *    Spostando tutto a sinistra, portiamo il vicino di destra di ogni bit nella
 *    sua stessa posizione attuale.
 *    X << 1        = 0 1 1 1 0 0 0 0   (Lo '0' in posizione 2 si sposta in pos 3)
 *
 * 3. IL NOT DELLA COPIA SHIFTATA (~(X << 1)):
 *    Invertiamo completamente la riga. Gli '0' diventano '1' e gli '1' diventano '0'.
 *    ~(X << 1)     = 1 0 0 0 1 1 1 1   (Lo '0' in pos 3 è diventato la trappola '1')
 *
 * 4. L'AND FINALE (X & ~(X << 1)):
 *    Facciamo l'AND verticale tra il dato originale (riga 1) e la trappola (riga 3).
 *    X             = 0 0 1 1 1 0 0 0
 *    ~(X << 1)     = 1 0 0 0 1 1 1 1
 *    ───────────────────────────────
 *    RISULTATO     = 0 0 0 0 1 0 0 0   <-- Sopravvive SOLO il bit in posizione 3!
 *
 * ANALISI DEGLI EFFETTI SUI BIT:
 *   - Bit interni (5 e 4): originariamente '1', ma lo shift e il NOT li hanno
 *     azzerati. L'AND fa 1 & 0 = 0. [SPENTI]
 *   - Bit esterni (7, 6, 2, 1, 0): originariamente '0'. L'AND fa 0 & X = 0. [SPENTI]
 *   - Testa del gruppo (3): originariamente '1'. Il vicino destro era '0', invertito
 *     in '1' da shift+NOT. L'AND fa 1 & 1 = 1. [ACCESO - ISOLATO CON SUCCESSO]
 *
 * A questo punto il problema si riduce semplicemente a contare quanti bit totali
 * sono rimasti accesi tramite una routine di Population Count (popcount).
 *
 * =============================================================================
 * 2. APPLICAZIONI PRATICHE
 * =============================================================================
 * I. COMPRESSIONE RUN-LENGTH ENCODING (RLE) E PACKING
 * -----------------------------------------------------------------------------
 * Contare i gruppi di bit diversi serve a stimare l'efficienza di compressione
 * di un vettore prima di applicare algoritmi come RLE o varianti bitwise. Meno
 * gruppi significano sequenze più lunghe di bit identici, segnalando che il
 * flusso dati è altamente comprimibile.
 *
 * II. PROTOCOLLI DI RETE E VALIDAZIONE SUBNET MASK
 * -----------------------------------------------------------------------------
 * Nello stack TCP/IP, una maschera di sottorete (Subnet Mask) valida deve essere
 * composta tassativamente da una serie contigua di 1 seguita da una serie contigua
 * di 0 (es. 0xFFFFFF00).bit_delta permette di validare istantaneamente una
 * maschera: se il delta tra la maschera e 0xFFFFFFFF produce più di un singolo
 * gruppo di bit alterati, la maschera è frammentata e quindi illegale.
 *
 * =============================================================================
 * 3. IMPLEMENTAZIONE HARDWARE
 * =============================================================================
 * L'operazione finale di popcount viene mappata direttamente sul silicio tramite
 * l'istruzione assembly nativa POPCNT (su x86) o CNT (su ARM/Apple Silicon).
 * Se compilato con flag `-mpopcnt`, la CPU risolve il conteggio dei gruppi in
 * 1 solo ciclo di clock hardware, rendendo l'intera funzione libera da cicli
 * e stalli di pipeline.
 * =============================================================================
 */

/**
 * @brief APPROCCIO 1: O(1) Absolute Zen (Il top delle prestazioni).
 * Sfrutta il trucco del mascheramento dei fronti e l'istruzione hardware popcount.
 */
int bit_delta(unsigned int a, unsigned int b)
{
    unsigned int diff = a ^ b;
    /* Isola il bit di testa di ogni gruppo contiguo */
    unsigned mask = diff & ~(diff << 1);
    /* Conta i bit rimasti*/
    return popcount_swar32(mask);
}

/**
 * @brief APPROCCIO 2: O(Gruppi) - Totalmente conforme alle restrizioni dei quiz 42.
 * Non usa funzioni di libreria o built-in. Sfrutta l'algebra di brian kernighan
 * modificata per cancellare un INTERO GRUPPO di 1 alla volta anziché un singolo bit.
 *
 * Formula di cancellazione del blocco: x & ((x | (x - 1)) + 1)
 */
int bit_delta_moulinette(unsigned int a, unsigned int b)
{
    unsigned int diff = a ^ b;
    int groups_count = 0;

    while (diff != 0)
    {
        /* Questa operazione bitwise azzera l'intero gruppo di 1 più a destra in un colpo solo */
        diff &= ((diff | (diff - 1)) + 1);
        groups_count++;
    }

    return groups_count;
}

int main(void)
{
    /*
     * Il caso dell'esempio corretto:
     * a = 0b11001100 (0xCC)
     * b = 0b10101010 (0xAA)
     * Risultato reale atteso: 2 gruppi
     */
    unsigned int a1 = 0xCC;
    unsigned int b1 = 0xAA;

    /*
     * Caso 2: Maschere completamente alternate (32 gruppi isolati)
     * a = 0x55555555 (0b01010101...)
     * b = 0xAAAAAAAA (0b10101010...)
     * Differenza = 0xFFFFFFFF (Tutti i bit diversi -> 1 unico grande gruppo da 32 bit)
     * Risultato atteso: 1
     */
    unsigned int a2 = 0x55555555;
    unsigned int b2 = 0xAAAAAAAA;

    /*
     * Caso 3: Gruppi multipli sparsi su 32 bit
     * diff = 0x80000001 (Un bit acceso all'estremo sinistro, uno all'estremo destro)
     * Risultato atteso: 2 gruppi
     */
    unsigned int a3 = 0x80000001;
    unsigned int b3 = 0x00000000;

    printf("====================================================================\n");
    printf(" BIT_DELTA: VERIFICA PRESTAZIONI E CORREZIONE TRACCIA\n");
    printf("====================================================================\n");

    printf("TEST 1 (Esempio Traccia: 0xCC ^ 0xAA):\n");
    printf("  -> Metodo O(1) Fast:         %d\n", bit_delta(a1, b1));
    printf("  -> Metodo Moulinette Loop:   %d (Valore reale corretto, non 3!)\n\n", bit_delta_moulinette(a1, b1));

    printf("TEST 2 (Massima divergenza: 0x55555555 ^ 0xAAAAAAAA):\n");
    printf("  -> Metodo O(1) Fast:         %d\n", bit_delta(a2, b2));
    printf("  -> Metodo Moulinette Loop:   %d\n\n", bit_delta_moulinette(a2, b2));

    printf("TEST 3 (Bit ai margini: 0x80000001 ^ 0x0):\n");
    printf("  -> Metodo O(1) Fast:         %d\n", bit_delta(a3, b3));
    printf("  -> Metodo Moulinette Loop:   %d\n", bit_delta_moulinette(a3, b3));

    printf("====================================================================\n");

    return 0;
}