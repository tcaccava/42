/*
 * =============================================================================
 * 1. BACKGROUND TEORICO: COMPRENSIONE LOGARITMICA SWAR
 * =============================================================================
 * L'operazione richiesta è un deinterleave uniforme (estrazione e compattazione 
 * a destra di bit alternati), formalmente speculare al deposito (PDEP). 
 * Equivale alla primitiva classica PEXT (Parallel Bits Extract).
 * 
 * Un loop sequenziale estrarrebbe un bit alla volta spostandolo nella posizione
 * corretta, richiedendo un tempo lineare O(N) rispetto alla dimensione della 
 * word (32 iterazioni). L'approccio SWAR (Simd Within A Register) abbatte 
 * questa complessità a tempo logaritmico O(log N), ovvero esattamente:
 *   log2(32) = 5 passi teorici (ridotti a 4 in esecuzione dopo il mascheramento).
 * 
 * Principio della contrazione geometrica:
 * Siano i bit di interesse già isolati nelle posizioni pari dalle maschere:
 *   [b30, 0, b28, 0, b26, 0, b24, 0, b22, 0, b20, 0, b18, 0, b16, 0...]
 * 
 * Ad ogni passo l'algoritmo esegue una mossa di tipo "shift-and-merge":
 *   1. n >> k sposta i blocchi alti sopra i buchi lasciati dagli zeri.
 *   2. L'operazione di OR (n | (n >> k)) fonde i bit originali con quelli shiftati.
 *   3. Una maschera ad hoc pulisce i duplicati spuri generati dall'OR, 
 *      consolidando i bit utili in gruppi sempre più compatti (coppie, quartetti, 
 *      byte, fino alla word finale da 16 bit contigui a destra).
 * 
 * =============================================================================
 * 2. APPLICAZIONI PRATICHE NEL SOFTWARE AD ALTE PRESTAZIONI
 * =============================================================================
 * I. DECODIFICA DEI CODICI MORTON (Z-ORDER CURVES)
 * -----------------------------------------------------------------------------
 * Se la dilatazione dei bit serve a generare le chiavi di Morton per indici 
 * spaziali (es. Octree, BVH nei raytracer, database GIS), il deinterleave è 
 * l'operazione fondamentale per la DECODIFICA.
 * Dato un codice di Morton 2D a 32 bit, per riottenere le coordinate cartesiane 
 * originali (X e Y a 16 bit) devi estrarre i bit dispari per Y e i bit pari per X.
 * Questo algoritmo, con start=0 e start=1, fa esattamente questo alla velocità 
 * massima consentita dal software.
 * 
 * II. TRASFORMAZIONI DI IMMAGINI E FORMATI VIDEO (PLANAR VS INTERLEAVED)
 * -----------------------------------------------------------------------------
 * Nel processing di segnali video o grafici (es. formati YUV422 o canali audio 
 * stereo interleaved L/R), i dati arrivano alternati nel flusso di byte. 
 * Separare i canali sinistro e destro, o deinterlacciare i piani di crominanza, 
 * richiede la scomposizione dei flussi. Lo SWAR permette di processare queste 
 * stringhe di bit direttamente dentro i registri generali senza toccare la RAM.
 * 
 * III. CRITTOGRAFIA (DES, RETI DI PERMUTAZIONE)
 * -----------------------------------------------------------------------------
 * Algoritmi di cifratura classici e moderni che applicano permutazioni fisse 
 * ed estrazioni bit-slicing sfruttano questa esatta compressione logaritmica 
 * per evitare timing-attacks derivanti dall'uso di tabelle di lookup (S-Box o 
 * P-Box caricate in memoria che causano cache-miss misurabili).
 * 
 * =============================================================================
 * 3. ANALISI MICROARCHITETTURALE E IMPLEMENTAZIONE HW
 * =============================================================================
 * I. L'ISTRUZIONE NATIVA NELLE CPU ACCELERATE: BMI2 PEXT
 * -----------------------------------------------------------------------------
 * Sulle architetture moderne Intel (Haswell+) e AMD (Zen 3+), questo intero 
 * file viene reso obsoleto dall'istruzione assembly nativa PEXT.
 * Il compilatore GCC, se viene passato il flag `-mbmi2`, ottimizza la funzione 
 * sostituendola con una singola istruzione hardware che usa una rete a farfalla 
 * (Inverse Butterfly Network) sul silicio, risolvendo l'estrazione in 3 cicli 
 * di clock fissi.
 * 
 * II. DISASTRO HARDWARE AMD ZEN 1 E ZEN 2
 * -----------------------------------------------------------------------------
 * Sulle prime due generazioni AMD Ryzen (Zen 1/2), l'istruzione hardware PEXT 
 * veniva eseguita in microcodice (MROM), impiegando fino a ~250 cicli di clock 
 * e paralizzando l'unita' di esecuzione. Su quei processori, questa specifica 
 * implementazione SWAR manuale risulta drammaticamente più veloce dell'istruzione 
 * nativa della CPU.
 * 
 * III. PORTABILITÀ SU ARCHITETTURE RISC (ARM64 / APPLE SILICON)
 * -----------------------------------------------------------------------------
 * I processori ARM64 (inclusi i chip Apple M1/M2/M3/M4 o i server Graviton) non 
 * hanno un'istruzione equivalente a PEXT nel loro ISA standard. Di conseguenza, 
 * l'implementazione SWAR qui sotto rappresenta lo stato dell'arte dell'efficienza 
 * per la manipolazione di bit paralleli su tali piattaforme.
 * =============================================================================
 */

#include <stdio.h>

/**
 * @brief Estrae i bit alternati e li compatta a destra tramite SWAR logaritmico.
 * @param n Il registro a 32 bit da decostruire.
 * @param start Se 0 estrae i bit pari (0,2,4...), se 1 estrae i bit dispari (1,3,5...).
 * @return Un intero con i 16 bit estratti e compattati nei primi 16 bit di destra.
 */
unsigned int extract_alternating(unsigned int n, int start) {
    /* 
     * PASSO 1: Allineamento dinamico.
     * Se start=1, shifto a destra di 1. I bit dispari diventano pari.
     * Da questo punto in poi lavoriamo SEMPRE e solo sulle posizioni pari.
     */
    n >>= start;

    /* 
     * PASSO 2: Mascheramento iniziale statico.
     * CORREZIONE: 0x55555555 (0b01010101...) seleziona TUTTI i bit pari.*/
    unsigned int mask = 0x55555555;
    n &= mask;

    /* 
     * PASSO 3: Compressione SWAR logaritmica.
     * Unisce i bit distanti a coppie, quartetti, byte e mezze-word.
     */
    n = (n | (n >> 1)) & 0x33333333;  /* Fonde coppie isolate: bit da posizioni 0,2 -> 0,1 */
    n = (n | (n >> 2)) & 0x0F0F0F0F;  /* Fonde quartetti: sposta e compatta i nibble */
    n = (n | (n >> 4)) & 0x00FF00FF;  /* Fonde i byte alternati */
    n = (n | (n >> 8)) & 0x0000FFFF;  /* Compressione finale nei 16 bit d'ordine basso */

    return n;
}

int main(void) {
    /* 
     * TEST CASE 1: n = 0b10110100 (0xB4), start = 0
     * Bit posizioni pari (0, 2, 4, 6):
     *   Pos 6 = 0
     *   Pos 4 = 1
     *   Pos 2 = 1
     *   Pos 0 = 0
     * Risultato atteso: 0b0110 (6 in decimale, 0x6)
     */
    unsigned int test1 = 0x000000B4; 
    unsigned int res1 = extract_alternating(test1, 0);

    /* 
     * TEST CASE 2: Maschera piena (0xFFFFFFFF)
     * Con start = 0, deve estrarre tutti i 16 bit pari -> 0xFFFF
     * Con start = 1, deve estrarre tutti i 16 bit dispari -> 0xFFFF
     */
    unsigned int res2_pari    = extract_alternating(0xFFFFFFFF, 0);
    unsigned int res2_dispari = extract_alternating(0xFFFFFFFF, 1);

    printf("====================================================================\n");
    printf(" SWAR DEINTERLEAVE VERIFICATION REPORT\n");
    printf("====================================================================\n");
    printf("Test 1 [Input: 0x%08X, Start: 0] -> Atteso: 0x6    | Ottenuto: 0x%X\n", test1, res1);
    printf("Test 2 [Input: 0xFFFFFFFF, Start: 0] -> Atteso: 0xFFFF | Ottenuto: 0x%X\n", res2_pari);
    printf("Test 3 [Input: 0xFFFFFFFF, Start: 1] -> Atteso: 0xFFFF | Ottenuto: 0x%X\n", res2_dispari);
    printf("====================================================================\n");

    return 0;
}