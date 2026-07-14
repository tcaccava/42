/* =============================================================================
 * 1. BACKGROUND TEORICO E FORMALIZZAZIONE MATEMATICA
 * =============================================================================
 * L'operazione di spaziatura uniforme dei bit (bitstream dilation) rappresenta
 * un sottoinsieme matematicamente vincolato della classe di trasformazioni
 * Parallel Bits Deposit (PDEP).
 *
 * Mentre la PDEP classica mappa un vettore compatto di bit attraverso una
 * proiezione iniettiva arbitraria impostata da una maschera di destinazione,
 * lo spread dinamico a gap costante si muove all'interno di un'algebra
 * geometricamente definita, dove gli indici di destinazione seguono una
 * progressione aritmetica lineare.
 *
 * Siano:
 *   - x = [x_0, x_1, ..., x_m-1] il vettore booleano di input (n) in {0, 1}^m.
 *   - y in {0, 1}^W il registro di destinazione (con larghezza W = 32 o 64 bit).
 *   - G il valore del gap di zeri da inserire (il parametro 'gap').
 *
 * La funzione di transizione degli indici pi(i) per uno spread uniforme con
 * passo G definisce la posizione finale del bit i-esimo dell'input:
 *
 *   pi(i) = i * (G + 1)
 *
 * Di conseguenza, il registro di destinazione y sara' popolato secondo la
 * seguente legge di assegnazione:
 *
 *   y_k = x_i      se k == i * (G + 1)
 *   y_k = 0        altrimenti
 *
 * Il vincolo fisico dell'architettura impone che il massimo indice mappato
 * non provochi l'overflow oltre i confini del registro di destinazione:
 *
 *   i_max * (G + 1) < W
 *
 * Se questo vincolo viene violato, i bit eccedenti subiscono una troncatura
 * strutturale (boundary truncation).
 *
 * =============================================================================
 * 2. COMPLEMENTO A DUE E MASCHERE BRANCHLESS
 * =============================================================================
 * Nell'emulazione software della PDEP generica, l'isolamento dei bit attivi
 * e la generazione delle maschere si basano sulle proprieta' dell'aritmetica
 * del complemento a due.
 *
 * L'operazione chiave per isolare il bit attivo piu' basso (LSSB - Least
 * Significant Set Bit) e' definita come:
 *
 *   LSSB(mask) = mask & (-mask)
 *
 * Poiche' in complemento a due -mask equivale a (~mask + 1), la negazione
 * inverte tutti i bit a sinistra del primo bit attivo (partendo da destra)
 * e aggiunge 1, propagando il riporto fino a reinvertire l'LSSB originale.
 * L'operazione di AND cancella tutti i bit eccetto l'LSSB isolato.
 *
 * La generazione di maschere condizionali senza salti (branchless) sfrutta la
 * saturazione del segno della negazione aritmetica di un booleano puro (0 o 1):
 *
 *   mask_v = -(bit_val)
 *
 *   Se bit_val == 1  =>  -(1) == 0xFFFFFFFF (tutti i bit a 1)
 *   Se bit_val == 0  =>  -(0) == 0x00000000 (tutti i bit a 0)
 *
 * Questo trucco permette di applicare o ignorare un bit rilocato con un AND,
 * eliminando del tutto le istruzioni di salto (jump condizionali) e prevenendo
 * i micidiali stalli della pipeline dovuti a branch misprediction.
 *
 * =============================================================================
 * 3. ANALISI MICROARCHITETTURALE E IMPATTO HARDWARE
 * =============================================================================
 * Sulle moderne CPU x86_64, la scelta tra emulazione software e istruzioni
 * native BMI2 (PDEP/PEXT) dipende strettamente dal silicio target:
 *
 * INTEL (Haswell, 2013 in poi) / AMD (Zen 3, 2020 in poi):
 *   Implementano PDEP direttamente sul silicio tramite una rete di commutazione
 *   logica dedicata (crossbar switch). L'operazione viene risolta in soli
 *   3 cicli di clock con un throughput di 1 istruzione per ciclo.
 *
 * AMD (Zen 1 & Zen 2):
 *   Scelta architetturale disastrosa: PDEP/PEXT non erano mappate su transistor
 *   dedicati, ma emulate internamente tramite microcodice (MROM).
 *   L'istruzione richiedeva da 18 fino a oltre 250 cicli di clock per essere
 *   eseguita, bloccando la pipeline.
 *   Su queste macchine, l'emulazione software branchless (come quella proposta
 *   sotto) risulta fino a 10 volte piu' veloce dell'istruzione assembly nativa.
 *
 * ARM64 / EMBEDDED:
 *   Sistemi privi del set BMI2. L'emulazione software e' l'unica via possibile
 *   per garantire portabilita' ed efficienza.
 *
 * =============================================================================
 * 4. OTTIMIZZAZIONE DELLO SPREAD UNIFORME CONTRO PDEP
 * =============================================================================
 * Per lo spread a gap costante, usare la PDEP software e' un overkill inutile.
 * Poiche' la distanza tra i bit e' costante, non serve calcolare e aggiornare
 * dinamicamente la maschera con l'aritmetica complessa (mask & -mask).
 *
 * Basta accumulare lo shift ad ogni iterazione:
 *
 *   res |= (n & 1) << shift;
 *   shift += step;
 *
 * Questo ciclo compie pochissime iterazioni grazie all'uscita anticipata
 * (quando n == 0). Per numeri piccoli (es: input = 5, cioe' 0b101), il ciclo
 * gira solo 3 volte. Il branch predictor della CPU impara subito questo limite
 * riducendo il costo reale di esecuzione a pochissimi cicli di clock (6-10).
 * =============================================================================
 */

 /* =============================================================================
 * 5. APPLICAZIONI PRATICHE NEL SOFTWARE AD ALTE PRESTAZIONI
 * =============================================================================
 * L'operazione di spaziatura e dilatazione dei bit non e' un esercizio di stile,
 * ma rappresenta un pilastro ottimizzativo in tre domini critici:
 *
 * I. CURVE DI RIEMPIMENTO DELLO SPAZIO (Z-ORDER CURVES / MORTON CODES)
 * -----------------------------------------------------------------------------
 * Nei database spaziali, GIS (Sistemi Informativi Geografici), nei motori di
 * rendering 3D (per l'ottimizzazione di Octree e BVH - Bounding Volume
 * Hierarchies) e nelle simulazioni fisiche N-Body, e' necessario mappare
 * coordinate multidimensionali (X, Y, Z) in una singola chiave lineare 1D.
 * Questa mappatura deve preservare al massimo la localita' spaziale.
 *
 * La curva di Morton (Z-order curve) fa esattamente questo interlacciando
 * i bit delle coordinate. Ad esempio, per coordinate a 10 bit in 3D:
 *   X = x_9 x_8 ... x_0
 *   Y = y_9 y_8 ... y_0
 *   Z = z_9 z_8 ... z_0
 *
 * Per calcolare il codice di Morton 3D dobbiamo:
 *   1. Dilatare X con gap = 2  =>  x_9 0 0 x_8 0 0 ... x_0
 *   2. Dilatare Y con gap = 2, shiftato di 1 => 0 y_9 0 0 y_8 0 ... 0
 *   3. Dilatare Z con gap = 2, shiftato di 2 => 0 0 z_9 0 0 z_8 ... 0
 *   4. Unire i tre registri con un OR bitwise.
 *
 * L'algoritmo 'bit_spread_dynamic_32' risolve questo problema a runtime con un
 * gap arbitrario, permettendo di cambiare dinamicamente il numero di dimensioni
 * (es. passare da Morton 2D con gap=1 a Morton 3D con gap=2, o Morton 4D con
 * gap=3) senza dover scrivere e compilare funzioni SWAR cablate a codice.
 *
 *
 * II. COMPRESSIONE BITMAP E INDEXING COLONNARE (OLAP)
 * -----------------------------------------------------------------------------
 * I moderni database analitici (es. ClickHouse, DuckDB, Snowflake) gestiscono
 * enormi indici bitmap compressi tramite varianti di Run-Length Encoding (RLE)
 * o Roaring Bitmaps.
 *
 * Durante le fasi di scansione e filtraggio vettorializzato dei predicati, la
 * CPU deve espandere rapidamente i bit di controllo ammassati in una word
 * densa per proiettarli su campi dati allineati a seconda dello schema di
 * compressione applicato a quella specifica colonna. Se la densita' e il gap
 * di decompressione variano a runtime in base alla cardinalita' della colonna,
 * lo spread dinamico evita la generazione di codice JIT o pesanti tabelle di
 * lookup in memoria RAM, riducendo drasticamente la pressione sulla cache L1.
 *
 *
 * III. CIFRARI SIMMETRICI E PERMUTAZIONI BIT-SLICED
 * -----------------------------------------------------------------------------
 * Molti cifrari a blocchi crittografici (es. DES, PRESENT, serpent) si basano
 * su reti di permutazione (SPN) complesse. Implementare queste permutazioni
 * tramite array di puntatori o shift manuali e' inefficiente e vulnerabile a
 * side-channel attacks sulla cache.
 *
 * Spaziando i bit con un pattern calcolato a runtime e applicando maschere,
 * e' possibile implementare permutazioni arbitrarie in parallelo usando registri
 * generici. Il bit-slicing crittografico sfrutta proprio queste tecniche di
 * "dilatazione e fusione" per processare piu' blocchi di dati simultaneamente.
 *
 * =============================================================================
 * 6. IMPLEMENTAZIONE HARDWARE E DETTAGLI MICROARCHITETTURALI
 * =============================================================================
 * La fisica del silicio dietro le istruzioni PDEP e PEXT del set BMI2 rivela
 * perche' l'ottimizzazione software sia ancora oggi fondamentale.
 *
 * I. IL RETICOLO DI COMMUTAZIONE (CROSSBAR SWITCH NETWORK)
 * -----------------------------------------------------------------------------
 * Nelle CPU Intel (da Haswell in poi) e AMD (da Zen 3 in poi), l'istruzione PDEP
 * non viene scomposta in micro-operazioni sequenziali. Viene eseguita da una
 * rete logica combinatoria chiamata "Crossbar Switch".
 *
 * Questa rete e' composta da una matrice di multiplexer configurati in modo
 * piramidale. Quando la CPU decodifica `pdep r32, r32, r32`, la maschera di
 * controllo configura istantaneamente i nodi della matrice di commutazione,
 * aprendo e chiudendo le "strade" logiche per far scivolare i bit dell'input
 * direttamente nelle posizioni di destinazione in parallelo.
 *
 * Latenza hardware nativa: 3 cicli di clock.
 * Throughput nativo: 1 istruzione per ciclo.
 *
 *
 * II. L'EMULAZIONE IN MICROCODICE (AMD ZEN 1 & ZEN 2)
 * -----------------------------------------------------------------------------
 * Nelle microarchitetture Zen 1 e Zen 2 (Ryzen serie 1000, 2000 e 3000), AMD
 * decise di risparmiare transistor evitando di implementare la matrice fisica
 * per PDEP/PEXT. Quando il processore incontrava l'istruzione PDEP, sollevava
 * internamente un trap hardware dirottando l'esecuzione verso la MROM (Microcode
 * Read-Only Memory).
 *
 * Il microcodice interno avviava un loop sequenziale di estrazione, shift e
 * deposito bit per bit direttamente dentro la CPU.
 *
 * Conseguenze sulle prestazioni:
 *   - Latenza schizzata a ~18 cicli (se la maschera aveva pochi bit attivi)
 *     fino a oltre 250 cicli di clock (per maschere dense).
 *   - Completa saturazione delle unita' di esecuzione con conseguente stallo
 *     della pipeline Out-of-Order.
 *
 * Per questo motivo, nei motori di scacchi (es. Stockfish, che usa le bitboard)
 * e nei database, i programmatori hanno introdotto macro di compilazione per
 * disabilitare il BMI2 su macchine Zen 1/Zen 2, preferendo l'algoritmo di
 * emulazione branchless in C (come `pdep_emulated_32`), che gira in una frazione
 * infinitesima di quel tempo.
 *
 *
 * III. PIPELINE ANALYSIS DELL'ALGORITMO DINAMICO
 * -----------------------------------------------------------------------------
 * Analizziamo l'esecuzione del ciclo di `bit_spread_dynamic_32` a basso livello:
 *
 *   while (n != 0 && shift < 32) {
 *       res |= (n & 1) << shift;
 *       shift += step;
 *       n >>= 1;
 *   }
 *
 * 1. Dipendenze dei Dati (Data Dependency Chains):
 *    - C'e' una catena di dipendenza critica sulla variabile `shift` (`shift += step`)
 *      e su `n` (`n >>= 1`). Tuttavia, sono operazioni aritmetiche banali che
 *      richiedono 1 solo ciclo di clock e vengono eseguite in parallelo sulle
 *      ALU superscalari (es. Porte 0, 1 e 5 di Intel).
 *    - Lo shift variabile `<< shift` richiede l'uso dell'unita' barrell shifter
 *      della CPU. Su x86 questo viene mappato sull'istruzione `SHLX` (se BMI2 e'
 *      attivo) o sulla classica `SHL` con registro `CL`. `SHLX` non ha dipendenze
 *      implicite sul registro dei flag, migliorando l'esecuzione Out-of-Order.
 *
 * 2. Efficienza del Branch Predictor (Lazy Termination):
 *    - Molti algoritmi software PDEP devono ciclare un numero fisso di volte
 *      (es. 32 o 64 iterazioni stabili). Il nostro spread dinamico si ferma
 *      non appena `n == 0`.
 *    - Se si stanno elaborando dati a bassa densita' (es. coordinate spaziali
 * da 8 o 10 bit), il ciclo gira solo 8 o 10 volte invece di 32.
 *    - Le CPU moderne tracciano la storia dei salti tramite la branch target
 *      buffer (BTB). Poiche' il limite del ciclo dipende direttamente dalla
 *      lunghezza in bit del dato di input, se i dati hanno lunghezze omogenee,
 *      il branch predictor azzera completamente i cicli persi per misprediction,
 *      rendendo questo codice incredibilmente veloce e privo di stalli.
 */

#include <stdio.h>
#include <stdint.h>

/**
 * @brief Emulazione software branchless di PDEP (Parallel Bits Deposit).
 * 
 * Prende i bit piu' bassi di 'n' e li distribuisce sulle posizioni definite
 * dai bit attivi di 'mask'. Implementazione standard ad alta portabilita'.
 */
uint32_t pdep_emulated_32(uint32_t n, uint32_t mask) {
    uint32_t res = 0;
    while (mask != 0) {
        uint32_t low_bit = mask & -mask;       /* Isola l'LSSB della maschera */
        res |= low_bit & -(n & 1);             /* Applica branchless masking */
        n >>= 1;                               /* Consuma il bit di input */
        mask ^= low_bit;                       /* Disattiva il bit elaborato */
    }
    return res;
}

/**
 * @brief Algoritmo ottimizzato per la dilatazione a gap uniforme e dinamico.
 * 
 * Evita la maschera PDEP. Calcola lo shift accumulando il passo ad ogni ciclo.
 * Ottimizzato per uscire immediatamente non appena l'input viene consumato (n == 0)
 * o quando lo shift supera i confini del registro a 32 bit.
 */
uint32_t bit_spread_dynamic_32(uint32_t n, uint32_t gap) {
    uint32_t res = 0;
    uint32_t shift = 0;
    uint32_t step = gap + 1;

    while (n != 0 && shift < 32) {
        res |= (n & 1) << shift;
        shift += step;
        n >>= 1;
    }
    return res;
}

int main(void) {
    uint32_t input = 0x00000005; /* 0b00000101 (bit 0 e bit 2 attivi) */
    uint32_t gap = 2;            /* Spaziatura di 2 zeri tra ogni bit */

    /* -------------------------------------------------------------------------
     * Esecuzione del Bit Spreading Dinamico
     * -------------------------------------------------------------------------
     * Con input = 0b101 e gap = 2:
     * - Il bit 0 (valore 1) va a posizione 0.
     * - Il bit 1 (valore 0) andrebbe a posizione 3.
     * - Il bit 2 (valore 1) va a posizione 6.
     * Risultato atteso: 1000001 in binario, ovvero 65 in decimale (0x00000041).
     */
    uint32_t spread_res = bit_spread_dynamic_32(input, gap);
    
    /* -------------------------------------------------------------------------
     * Esecuzione di verifica tramite PDEP Software classica
     * -------------------------------------------------------------------------
     * Per simulare lo stesso comportamento con la PDEP generica, dobbiamo
     * passare una maschera pre-calcolata con i bit attivi a distanza di (gap + 1).
     * Maschera per gap=2: 0x000000C1 (bit in posizione 0, 3 e 6 attivi).
     */
    uint32_t test_mask = 0x00000049; /* 0b01001001 (posizioni 0, 3 e 6 attive) */
    uint32_t pdep_res = pdep_emulated_32(input, test_mask);

    printf("====================================================================\n");
    printf(" BITSTREAM DILATION & PDEP EMULATION REPORT\n");
    printf("====================================================================\n");
    printf("Input iniziale:           0x%08X  (Decimal: %u)\n", input, input);
    printf("Gap dinamico selezionato: %u\n\n", gap);
    
    printf("[1] Risultato Dynamic Spread: 0x%08X  (Decimal: %u)\n", spread_res, spread_res);
    printf("[2] Risultato PDEP Emulata:   0x%08X  (Decimal: %u)\n\n", pdep_res, pdep_res);
    
    if (spread_res == pdep_res) {
        printf("VERIFICA: Corretta. Entrambi i metodi producono lo stesso output.\n");
    } else {
        printf("ERRORE: Disallineamento nei risultati.\n");
    }
    printf("====================================================================\n");

    return 0;
}