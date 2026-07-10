/**
 * =====================================================================================
 * @file        byte_reversal_32.c
 * @brief       Inversione dell'ordine dei byte (Endianness Swap) a 32-bit.
 * =====================================================================================
 */

#include <stdio.h>

/**
 * -------------------------------------------------------------------------------------
 * 1. BACKGROUND TEORICO: LA GEOMETRIA DEI BYTE
 * -------------------------------------------------------------------------------------
 * L'algoritmo esegue una manipolazione posizionale dei byte (gruppi di 8 bit) all'interno
 * di un registro a 32 bit, invertendone l'ordine geometrico speculare. 
 * A differenza di un "reverse_bits" classico (che inverte l'ordine di ogni singolo bit),
 * questo operatore preserva l'architettura interna del byte originario.
 * 
 * Dato un intero composto da 4 byte [A][B][C][D], l'espressione esegue 4 micro-operazioni:
 * - (n << 24): Sposta il byte meno significativo [D] in cima -> [D][0][0][0]
 * - (n >> 24): Sposta il byte piu' significativo [A] in fondo -> [0][0][0][A]
 * - ((n & 0x00FF0000) >> 8): Isola [B], lo sposta sul terzo byte  -> [0][0][B][0]
 * - ((n & 0x0000FF00) << 8): Isola [C], lo sposta sul secondo byte -> [0][C][0][0]
 * 
 * L'unione tramite l'operatore OR logico (|) unisce i frammenti generando [D][C][B][A].
 * La complessita' computazionale astratta e' O(1) in tempo e in spazio.
 * 
 * -------------------------------------------------------------------------------------
 * 2. APPLICAZIONI PRATICHE: ENDIANNESS CONVERSION
 * -------------------------------------------------------------------------------------
 * Questa operazione e' fondamentale per gestire i conflitti di Endianness:
 * - Network Byte Order vs Host Byte Order: I protocolli di rete (TCP/IP, UDP, header IP)
 *   utilizzano la codifica Big-Endian (il byte piu' significativo viene trasmesso per primo).
 *   Le architetture consumer piu' diffuse (x86_64, ARM in modalità di default) usano il
 *   formato Little-Endian. Questo algoritmo e' il motore dietro le funzioni di sistema
 *   come ntohl() e htonl().
 * - Parsing di File Binary: Formati di file storici o cross-platform (es. immagini BMP, 
 *   audio WAV, eseguibili ELF, archivi ZIP) dichiarano nei loro header campi a 32 bit
 *   con un'endianness specifica. Per leggerli correttamente su macchine diverse, lo swap
 *   dei byte e' obbligatorio.
 * 
 * -------------------------------------------------------------------------------------
 * 3. IMPLEMENTAZIONE HARDWARE E OTTIMIZZAZIONI DEL COMPILATORE
 * -------------------------------------------------------------------------------------
 * Sebbene il codice sorgente mostri 4 shift, 2 AND e 3 OR (per un totale di 9 operazioni
 * logico-aritmetiche), l'esecuzione reale sulle CPU moderne e' infinitamente piu' veloce.
 * 
 * - Architettura x86_64: Esiste un'istruzione hardware dedicata chiamata BSWAP,che puo'
 *   essere richiamata in codice conla builtin GCC/Clang  __builtin_bswap32. 
 *   In alternativa i compilatori moderni (GCC/Clang) riconoscono questo pattern(ad esempio usando
 *   con gcc le ottimizzazioni -o2 e -o3) esatto di codice e,usando la Idiom Recognition(cioe'
 *   l'ottimizzazione attraverso la ricerca dell' "intento" semantico del flusso di codice) tramite 
 *   la tecnica di "Pattern Matching" identificano che il codice scritto "equivale" ad una __builtinswap32 e
 *   eliminano tutti gli shift e gli AND, compilando la funzione in una singola riga assembly: bswap eax.
 * - Architettura ARM: Utilizza l'istruzione nativa REV (Reverse bytes).
 * 
 * L'operazione richiede quindi 1 solo ciclo di clock hardware e zero accessi alla memoria,
 * girando direttamente dentro i registri della CPU.
 */

/**
 * @brief Inverte l'ordine dei byte di un intero a 32 bit senza alterare i bit interni.
 * @param n Il valore a 32 bit in ingresso.
 * @return unsigned int Il valore con i byte invertiti.
 */
unsigned int bit_reverse_byte_order(unsigned int n) 
{
    return ((n << 24) | (n >> 24) | ((n & 0x00FF0000) >> 8) | ((n & 0x0000FF00) << 8));
}

int main(void) 
{
    unsigned int input = 0xAABBCCDD;
    unsigned int output = bit_reverse_byte_order(input);

    printf("--- VERIFICA SWAP BYTE (bswap32) ---\n\n");
    printf("Input iniziale:  0x%08X\n", input);
    printf("Output invertito: 0x%08X\n", output);

    /*
     * TRACCIAMENTO DI VERIFICA:
     * Input:  [AA] [BB] [CC] [DD]
     * Passo 1 (<< 24): DD 00 00 00
     * Passo 2 (>> 24): 00 00 00 AA
     * Passo 3 (Mid R): 00 00 BB 00
     * Passo 4 (Mid L): 00 CC 00 00
     * ----------------------------
     * OR Finale:       DD CC BB AA -> 0xDDCCBBAA
     */

    if (output == 0xDDCCBBAA) 
    {
        printf("\nVerifica superata: I byte sono stati invertiti correttamente.\n");
    } 
    else 
    {
        printf("\nErrore nel calcolo.\n");
    }

    return 0;
}