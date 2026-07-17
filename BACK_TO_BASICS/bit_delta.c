#include <stdio.h>
#include "./popcount_swar32.c"
/* =============================================================================
 * 1. IL GRANCHIO NELL'ESEMPIO DELLA TRACCIA (DEBUNKING)
 * =============================================================================
 * L'esempio fornito nella traccia asserisce che:
 *   a   = 0b11001100
 *   b   = 0b10101010
 *   XOR = 0b01100110  --> Viene dichiarato "3 gruppi -> risultato 3"
 *
 * Questo è matematicamente FALSO. Se isoliamo gli 1 contigui in 0b01100110:
 *   Bit 7: 0
 *   Bit 6: 1 ┐ [Gruppo 1]
 *   Bit 5: 1 ┘
 *   Bit 4: 0
 *   Bit 3: 0
 *   Bit 2: 1 ┐ [Gruppo 2]
 *   Bit 1: 1 ┘
 *   Bit 0: 0
 *
 * I gruppi contigui di 1 sono esattamente DUE. Chi ha redatto il testo ha
 * confuso il numero di transizioni interne o ha contato il blocco di zeri
 * centrale ('00') come se fosse un gruppo valido, violando la specifica
 * "conta le sequenze di 1 contigui". Il codice corretto deve restituire 2.
 *
 * =============================================================================
 * 2. BACKGROUND TEORICO: RILEVAMENTO DELLE TRANSIZIONI (EDGE DETECTION)
 * =============================================================================
 * Come si contano i gruppi contigui di bit accesi in modo efficiente?
 * Ciascun gruppo contiguo di '1' è necessariamente delimitato da una transizione
 * di inizio (0 -> 1) e una transizione di fine (1 -> 0).
 *
 * Se noi prendiamo la maschera delle differenze 'X' e la compariamo con se stessa
 * shiftata di una posizione ('X << 1'), possiamo isolare matematicamente solo
 * il "fronte d'inizio" (il bit più a sinistra) di ogni singolo gruppo.
 *
 * Formula magica SWAR in O(1):
 *   Gruppi = X & ~(X << 1);
 *
 * Applicata a 0b01100110:
 *   X        = 01100110
 *   X << 1   = 11001100
 *   ~(X <<1) = 00110011
 *   AND      = 01100110 & 00110011 = 00100010
 *
 * Il risultato dell'AND ha esattamente DUE bit accesi (bit 5 e bit 1), che
 * corrispondono alla testa di ogni gruppo. A questo punto, il problema si
 * riduce a contare quanti bit totali sono rimasti accesi (Population Count).
 *
 * =============================================================================
 * 3. APPLICAZIONI PRATICHE
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
 * di 0 (es. 0xFFFFFF00).ft_bit_delta permette di validare istantaneamente una
 * maschera: se il delta tra la maschera e 0xFFFFFFFF produce più di un singolo
 * gruppo di bit alterati, la maschera è frammentata e quindi illegale.
 *
 * =============================================================================
 * 4. IMPLEMENTAZIONE HARDWARE
 * =============================================================================
 * L'operazione finale di popcount viene mappata direttamente sul silicio tramite
 * l'istruzione assembly nativa `POPCNT` (su x86) o `CNT` (su ARM/Apple Silicon).
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
int ft_bit_delta_moulinette(unsigned int a, unsigned int b) {
    unsigned int diff = a ^ b;
    int groups_count = 0;

    while (diff != 0) {
        /* Questa operazione bitwise azzera l'intero gruppo di 1 più a destra in un colpo solo */
        diff &= ((diff | (diff - 1)) + 1);
        groups_count++;
    }
    
    return groups_count;
}

int main(void) {
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
    printf(" FT_BIT_DELTA: VERIFICA PRESTAZIONI E CORREZIONE TRACCIA\n");
    printf("====================================================================\n");
    
    printf("TEST 1 (Esempio Traccia: 0xCC ^ 0xAA):\n");
    printf("  -> Metodo O(1) Fast:         %d\n", ft_bit_delta_fast(a1, b1));
    printf("  -> Metodo Moulinette Loop:   %d (Valore reale corretto, non 3!)\n\n", ft_bit_delta_moulinette(a1, b1));

    printf("TEST 2 (Massima divergenza: 0x55555555 ^ 0xAAAAAAAA):\n");
    printf("  -> Metodo O(1) Fast:         %d\n", ft_bit_delta_fast(a2, b2));
    printf("  -> Metodo Moulinette Loop:   %d\n\n", ft_bit_delta_moulinette(a2, b2));

    printf("TEST 3 (Bit ai margini: 0x80000001 ^ 0x0):\n");
    printf("  -> Metodo O(1) Fast:         %d\n", ft_bit_delta_fast(a3, b3));
    printf("  -> Metodo Moulinette Loop:   %d\n", ft_bit_delta_moulinette(a3, b3));
    
    printf("====================================================================\n");

    return 0;
}