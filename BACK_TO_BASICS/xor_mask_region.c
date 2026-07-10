#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

/**
 * =====================================================================================
 * @file        selective_crypto_swar.c
 * @brief       Motore di Selective Encryption basato su ottimizzazione manuale SWAR
 * (SIMD Within A Register) per pipeline a 64-bit.
 * =====================================================================================
 *
 * -------------------------------------------------------------------------------------
 * 1. BACKGROUND TEORICO: ALLINEAMENTO DELLA MEMORIA E ARCHITETTURA SWAR
 * -------------------------------------------------------------------------------------
 * Il core di questo algoritmo si basa sul principio dell'allineamento dei dati in RAM.
 * Nelle moderne CPU a 64-bit, il bus dati verso la cache L1 è ottimizzato per muovere
 * blocchi nativi di 8 byte alla volta.
 *
 * Se si tenta di eseguire una lettura a 64-bit (uint64_t) da un indirizzo non allineato
 * (es. un indirizzo che termina in ...003 anziché ...000):
 * - Sulle vecchie architetture RISC (MIPS, SPARC, vecchi ARM): La CPU solleva un'eccezione
 * hardware (Alignment Fault) e killa il processo.
 * - Sulle moderne architetture (x86_64, ARM64): La CPU gestisce l'accesso non allineato
 * in hardware, ma è costretta a eseguire DUE letture distinte sulla cache L1 per poi
 * fondere i bit, dimezzando di fatto il throughput di memoria.
 *
 * Questa implementazione risolve il problema alla radice con una strategia a 3 fasi:
 * [Prologo Scalare]   --> Allinea il puntatore ai successivi 8 byte (byte-by-byte).
 * [Corpo SWAR]        --> Esegue XOR vettoriali paralleli su blocchi nativi a 64-bit.
 * [Epilogo Scalare]   --> Elabora i residui finali inferiori a 8 byte.
 *
 * -------------------------------------------------------------------------------------
 * 2. APPLICAZIONI PRATICHE: SELECTIVE ENCRYPTION NEI MEDIA STREAM (CENC / DRM)
 * -------------------------------------------------------------------------------------
 * Nei container multimediali moderni (come Matroska .mkv o MP4), i flussi video 4K/8K ad
 * alto framerate muovono gigabyte di dati al secondo. Se un hardware a basse prestazioni
 * (Smart TV, router, SoC embedded) dovesse decifrare l'intero flusso raw fotogramma per
 * fotogramma tramite algoritmi pesanti (es. AES-GCM completo), il carico della CPU
 * schizzerebbe al 100%, causando stuttering.
 *
 * Nei container multimediali moderni (MP4, Matroska), i dati non sono un ammasso informe.
 * Il flusso è diviso in:
 * -Metadata & NAL Units (Network Abstraction Layer): intestazioni che dicono al codec come
 *  decodificare il frame.
 * -I-Frames (Keyframes): fotogrammi completi che contengono l'intera immagine di partenza.
 * -P/B-Frames: semplici vettori di movimento (delta) che dicono quali pixel si sono spostati
 *  rispetto al frame precedente.
 *
 * Il "Selective Encryption" risolve questo collo di bottiglia lasciando i dati grezzi dei
 * pixel (il payload pesante) completamente in chiaro. Tramite i parametri di offset e
 * region_len, l'algoritmo isola e cifra unicamente le parti vitali del flusso:
 * - I metadati del container e gli header dei pacchetti di rete.
 * - Le NAL Units (Network Abstraction Layer) critiche.
 * - Gli I-Frames (Keyframes), senza i quali i codec (H.264, H.265, AV1) non possono
 * assolutamente ricostruire i successivi P-Frames e B-Frames (vettori di movimento).
 *
 * Il risultato è che il video diventa totalmente illegibile per i client non autorizzati,
 * ma il client legittimo deve decifrare solo l'1% o il 2% del flusso totale, preservando
 * banda, cicli di clock e autonomia termica/elettrica del dispositivo.
 *
 * -------------------------------------------------------------------------------------
 * 3. IMPLEMENTAZIONE HARDWARE E PIPELINE DELLA CPU
 * -------------------------------------------------------------------------------------
 * Durante l'esecuzione del ciclo SWAR, il processore opera tramite tecniche di:
 * - Out-of-Order Execution: La CPU riorganizza i micro-op interni per nascondere le
 * latenze di caricamento dalla memoria.
 * - Branch Prediction: Poiché il ciclo centrale avanza rigidamente a colpi di +8 byte,
 * il predittore di salto della CPU azzecca la transizione con una precisione vicina
 * al 100%, evitando lo svuotamento della pipeline (pipeline flush).
 *
 *

// APPUNTI PERSONALI:
// Applica XOR con una chiave ciclica a una sottoregione di data che inizia a offset
// e ha lunghezza region_len. Il resto di data rimane intatto. Usa SWAR per la parte centrale allineata a 8.
// È la base del selective encryption usato nei protocolli di streaming e nei container multimediali come Matroska dove solo parte del payload
// viene cifrata per performance. In container multimediali moderni come Matroska(.mkv) o MP4, un flusso video
// 4k o 8k ad alti framerate muove gigabytes di dati al secondo. Se un dispositivo poco performante come
// una smart tv o un router dovessero decodificare l'intro flusso byteraw fotogramma per fotogramma
// tramite algoritmi pesanti,il video andrebbe a scatti e la cpu al 100% di carico. Il selective encryption
// risolve questo problema: lascia i dati grezzi dei pixel(il payload pesante del frame) completamente in chiaro
// ,individua tramite offset e regionlen solo le parti vitali del flusso(i metadati del container,gli headers,cioe'
// le intestazioni dei pacchetti di rete o i fotogrammi chiave(I-Frames),senza i quali il codec non e' assolutamente
// in grado di ricorstruire le immagini). Cifrando solo una piccola porzione del file, il video rimane totalmente
// illegibile e protetto per chi non ha la chiave,ma il client autorizzato deve decifrare solo una frazione infinitesima
// dei dati,risparmiando preziose risorse hw e autonomia.

/**
 * @brief Applica lo XOR ciclico a una specifica sottoregione di memoria usando ottimizzazione SWAR.
 * @param data       Puntatore al buffer di dati totale.
 * @param key        Puntatore alla chiave crittografica simmetrica.
 * @param keylen     Lunghezza della chiave in byte.
 * @param offset     Punto di inizio della cifratura selettiva all'interno del buffer.
 * @param region_len Lunghezza esatta della finestra da cifrare.
 */

void xor_mask_region(unsigned char *data, unsigned char *key, size_t keylen, size_t offset, size_t region_len)
{
    unsigned char *init = data + offset;
    unsigned char *end = init + region_len;
    size_t kindex = 0;

    /* ---------------------------------------------------------------------------------
     * FASE 1: PROLOGO SCALARE (ALLINEAMENTO HARDWARE)
     * Verifichiamo se l'indirizzo 'init' è divisibile per 8 controllando i 3 bit meno
     * significativi del puntatore. Avanziamo byte-by-byte fino a raggiungere il confine
     * dei 64-bit, fermandoci prematuramente se la regione finisce prima dell'allineamento.
     * --------------------------------------------------------------------------------- */
    while (init < end && ((uintptr_t)init & 7))
    {
        *init = *init ^ key[kindex++ % keylen];
        init++;
    }

    /* ---------------------------------------------------------------------------------
     * FASE 2: CORPO SWAR (PARALLELISMO A LIVELLO DI REGISTRO CO-ALLINEATO)
     * Ora 'init' è matematicamente allineato a 8 byte. Processiamo blocchi da 64-bit.
     * Usiamo la condizione sicura (init + 8 <= end) anziché (init <= end - 8) per
     * sventare disastrosi fenomeni di pointer underflow in caso di indirizzamento basso.
     * --------------------------------------------------------------------------------- */
    while (init + 8 <= end)
    {
        uint64_t block = *(uint64_t *)init;
        uint64_t key64 = 0;
        int len = 0;

        /* Riempiamo il registro a 64-bit iniettando gli 8 byte successivi del keystream.
         * Forziamo il cast preventivo a (uint64_t) per garantire la portabilità dello
         * shift a sinistra anche su architetture LLP64 (es. Windows a 64-bit) dove
         * il tipo 'long' standard è limitato a soli 32 bit. */
        while (len < 8)
        {
            uint64_t byte = (uint64_t)(key[kindex % keylen]);
            byte <<= len++ * 8; /* Sposta geometricamente il byte nella sua posizione (0, 8, 16...56 bit) */
            key64 |= byte;
            kindex++;
        }

        block ^= key64;            /* XOR parallelo simultaneo su 8 byte */
        *(uint64_t *)init = block; /* Scrittura atomica a 64-bit in memoria */
        init += 8;
    }

    /* ---------------------------------------------------------------------------------
     * FASE 3: EPILOGO SCALARE (GESTIONE RESIDUI)
     * Se la 'region_len' originale non era un multiplo perfetto di 8 byte, consumiamo
     * gli ultimi byte rimasti (da 1 a 7) con un'elaborazione classica a livello di byte.
     * --------------------------------------------------------------------------------- */
    size_t rem = end - init;
    if (rem)
    {
        while (rem--)
        {
            *(unsigned char *)init++ ^= key[kindex++ % keylen];
        }
    }
}

/* COLLO DI BOTTIGLIA:
 * Nel ciclo centrale, la ricostruzione della maschera a 64-bit (key64) avviene tramite
 * un ciclo scalar loop interno (8 iterazioni con operazioni di shift e modulo '%'). L'operatore
 * di modulo % richiede tra i 10 e i 40 cicli di clock a seconda della CPU.
 * Per raggiungere prestazioni estreme senza istruzioni proprietarie (AVX/SIMD), le strade sono:
 * A) Pre-packing della chiave: Espandere e pre-calcolare le maschere a 64-bit prima del ciclo.
 * B) Auto-Vectorization: Scrivere un ciclo lineare a singolo byte e compilare con "gcc -O3",
 * permettendo al compilatore di srotolare il ciclo (Loop Unrolling) e iniettare vettori hardware.
 * -------------------------------------------------------------------------------------

/* =====================================================================================
 * DETTAGLIO LOGICO-MATEMATICO: CALCOLO M.C.M. E PRE-PACKING DELLA CHIAVE (SWAR V2)
 * =====================================================================================
 * * 1. IL PROBLEMA DEL MODULO (%) NEL LOOP CENTRALE
 * Nella versione standard, ogni iterazione SWAR elabora 8 byte, ma per ricostruire la
 * maschera a 64-bit (key64) coordinata con la chiave ciclica si usa l'operatore %.
 * Il modulo (%) a livello hardware compie una divisione intera, l'operazione più lenta
 * in assoluto per la pipeline della CPU (fino a 40 cicli di clock). Moltiplicato per
 * 8 byte a ogni blocco, questo annulla i vantaggi prestazionali dello SWAR.
 *
 * 2. LA SOLUZIONE MATEMATICA: MINIMO COMUNE MULTIPLO (LCM)
 * Per eliminare il modulo dal loop dei dati, pre-calcoliamo tutte le possibili varianti
 * della maschera a 64-bit prima di iniziare. La combinazione tra la lunghezza della chiave
 * (keylen) e la dimensione della word di macchina (8 byte) si ripete periodicamente.
 * Il punto esatto in cui il flusso della chiave e i blocchi da 8 byte tornano a essere
 * perfettamente sincronizzati è il Minimo Comune Multiplo:
 *
 * m.c.m.(keylen, 8) = (keylen * 8) / M.C.D.(keylen, 8)
 *
 * - Calcolo del M.C.D.: Implementato tramite l'algoritmo di Euclide classico (iterativo,
 * senza chiamate a funzioni intrinseche per garantire portabilità totale).
 * - Calcolo di 'cycle': Rappresenta il numero totale di BYTE necessari affinché il pattern
 * della chiave torni a coincidere con l'inizio di un blocco da 64-bit.
 * - Calcolo di 'num_blocks': Rappresenta quanti BLOCCHI da 8 byte compongono questo ciclo
 * (cycle / 8). Questa è la dimensione esatta dell'array di maschere da pre-generare.
 *
 * 3. ALLOCAZIONE PRESTAZIONALE SULLO STACK (VLA)
 * Anziché invocare malloc (chiamata di sistema pesante sulla Heap che frammenta la memoria),
 * usiamo un Variable-Length Array (VLA): uint64_t key_blocks[num_blocks];.
 * L'array viene allocato istantaneamente sullo Stack della CPU e rimosso automaticamente
 * al termine della funzione, azzerando l'overhead di allocazione.
 *
 * 4. IL FIX CRITICO DI SINCRONIZZAZIONE (PROLOGO-AWARE)
 * La chiave non viene assemblata partendo arbitrariamente dall'indice 0. Se il Prologo
 * Scalare ha consumato dei byte per allineare l'indirizzo di memoria, l'indice della
 * chiave è avanzato fino a un valore corrente kindex.
 * Il ciclo di pre-packing cattura esattamente questo stato iniziale ('temp_kindex = kindex')
 * e modella le maschere a 64-bit "su misura" per la specifica posizione in cui si trova
 * il puntatore in quel momento, garantendo che il keystream non sia desincronizzato.
 *
 * 5. MECCANICA DI BIT-PACKING
 * Per ognuno dei 'num_blocks' necessari:
 * - Si estraggono linearmente gli 8 byte successivi della chiave (gestendo la circolarità).
 * - Ogni byte viene promosso a 64-bit con un cast esplicito `(uint64_t)`.
 * - Il byte viene iniettato nella sua corretta posizione geometrica dentro il registro
 * tramite shift a sinistra: `<< (i * 8)` (quindi a 0, 8, 16, ... 56 bit di distanza).
 * - I byte vengono fusi tramite OR logico (`|=`) per formare la maschera finale a 64-bit.
 *
 * 6. RISULTATO NELLA PIPELINE
 * Nel loop centrale dei dati, l'operazione si riduce a: `*(uint64_t *)init ^= key_blocks[block_index];`
 * Il modulo sparisce. La CPU carica la maschera pre-calcolata dallo Stack, esegue lo XOR
 * atomico su 8 byte in un solo colpo e avanza. Zero calcoli, massima saturazione del bus.
 * =====================================================================================
 */

/**
 * @brief Applica lo XOR ciclico con PRE-PACKING delle maschere a 64-bit sulla Stack.
 * Risolve i colli di bottiglia del modulo '%' senza ricorrere a malloc.

void xor_mask_region(unsigned char *data, size_t n, unsigned char *key, size_t keylen, size_t offset, size_t region_len)
{
    /* DEFENSIVE PROGRAMMING: Validazione confini anti-overflow
    if (offset >= n || region_len == 0 || region_len > n - offset)
        return;

    unsigned char *init = data + offset;
    unsigned char *end = init + region_len;
    size_t kindex = 0;

    /* ---------------------------------------------------------------------------------
     * FASE 1: PROLOGO SCALARE (ALLINEAMENTO HARDWARE)
     * ---------------------------------------------------------------------------------
    while (init < end && ((uintptr_t)init & 7))
    {
        *init = *init ^ key[kindex];
        kindex = (kindex + 1 == keylen) ? 0 : kindex + 1; /* Branchless-style toggle
        init++;
    }

    /* ---------------------------------------------------------------------------------
     * FASE 2: CALCOLO DEL MASSIMO COMUNE DIVISORE (Euclide Standard - No __builtin)
     * Determiniamo quanti blocchi da 64-bit servono prima che la combinazione
     * Chiave-Word si ripeta in modo identico.
     * ---------------------------------------------------------------------------------
    size_t a = keylen, b = 8;
    while (b != 0)
    {
        size_t t = b;
        b = a % b;
        a = t;
    }
    size_t gcd = a;
    size_t cycle = (keylen * 8) / gcd;
    size_t num_blocks = cycle / 8;

    /* Allocazione sicura sulla STACK (Variable-Length Array).
     * Niente chiamate di sistema o frammentazione della Heap.
    uint64_t key_blocks[num_blocks];

    /* Generiamo il pre-packing partendo ESATTAMENTE dallo stato corrente
     * di 'kindex' ereditato dal prologo, garantendo la sincronizzazione del keystream.
    size_t temp_kindex = kindex;
    for (size_t b = 0; b < num_blocks; b++)
    {
        uint64_t k = 0;
        for (int i = 0; i < 8; i++)
        {
            k |= (uint64_t)key[temp_kindex] << (i * 8);
            temp_kindex = (temp_kindex + 1 == keylen) ? 0 : temp_kindex + 1;
        }
        key_blocks[b] = k;
    }

    /* ---------------------------------------------------------------------------------
     * FASE 3: CORPO SWAR OTTIMIZZATO (SENZA CICLI INTERNI NÉ MODULI)
     * ---------------------------------------------------------------------------------
    size_t block_index = 0;
    while (init + 8 <= end)
    {
        *(uint64_t *)init ^= key_blocks[block_index];
        init += 8;

        /* Avanzamento circolare dell'indice dei blocchi pre-caricati
        block_index = (block_index + 1 == num_blocks) ? 0 : block_index + 1;

        /* FIX BUG 2: Teniamo traccia dell'avanzamento reale del keystream
         * per non rompere la coordinazione con la fase di epilogo successiva.
        kindex = (kindex + 8) % keylen;
    }

    /* ---------------------------------------------------------------------------------
     * FASE 4: EPILOGO SCALARE (GESTIONE RESIDUI FINALI)
     * ---------------------------------------------------------------------------------
    size_t rem = end - init;
    if (rem)
    {
        while (rem--)
        {
            *(unsigned char *)init++ ^= key[kindex];
            kindex = (kindex + 1 == keylen) ? 0 : kindex + 1;
        }
    }
}
*/

/* =====================================================================================
 * ANALISI DI UN CASO REALE: TRACCIAMENTO FLUSSO PRE-PACKING CON CHIAVE DA 5 BYTE
 * SCENARIO: keylen = 5, kindex (post-prologo) = 3
 * =====================================================================================
 *
 * -------------------------------------------------------------------------------------
 * FASE 1: GEOMETRIA DEI REGISTRI E CALCOLO DELLE RIPETIZIONI (LCM)
 * -------------------------------------------------------------------------------------
 * Dati iniziali:
 * - Chiave astratta:  [K0, K1, K2, K3, K4]  (Lunghezza = 5 byte)
 * - Stato iniziale:   kindex = 3  --> Il Prologo Scalare ha già consumato K0, K1 e K2
 * per allineare l'indirizzo alla memoria hardware.
 * Il primo blocco SWAR deve iniziare da K3.
 *
 * Calcolo matematico del periodo tramite Algoritmo di Euclide:
 * M.C.D.(5, 8) = 1  (5 è un numero primo)
 * cycle        = (5 * 8) / 1 = 40 byte  --> Finestra di dati prima della ripetizione.
 * num_blocks   = 40 / 8 = 5 blocchi     --> Maschere a 64-bit da generare sulla Stack.
 *
 * Viene istanziato l'array nello Stack frame: uint64_t key_blocks[5];
 *
 * -------------------------------------------------------------------------------------
 * FASE 2: RICOSTRUZIONE DELLE MASCHERE NELLO STACK (BIT-PACKING)
 * -------------------------------------------------------------------------------------
 * L'algoritmo inizializza un 'temp_kindex = kindex = 3' e assembla i 5 blocchi da 64-bit
 * iniettando i byte dal meno significativo (LSB, byte 0) al più significativo (MSB, byte 7)
 * tramite shift geometrici progressivi (<< 0, << 8, << 16... << 56) e OR logici (|=).
 *
 * [key_blocks[0]] -> Inizia da temp_kindex = 3
 * i=0 -> key[3] (K3) << 0       |  i=4 -> key[2] (K2) << 32
 * i=1 -> key[4] (K4) << 8       |  i=5 -> key[3] (K3) << 40
 * i=2 -> key[0] (K0) << 16 * |  i=6 -> key[4] (K4) << 48
 * i=3 -> key[1] (K1) << 24      |  i=7 -> key[0] (K0) << 56 *
 * (*) Nota: temp_kindex fa rollover (azzera su keylen) e ricomincia da K0.
 * Layout finale Registro 0 (MSB -> LSB): [K0][K4][K3][K2][K1][K0][K4][K3]
 * Stato temp_kindex al termine del blocco = 1
 *
 * [key_blocks[1]] -> Inizia da temp_kindex = 1
 * Sequenza lineare degli indici catturati nel ciclo: 1, 2, 3, 4, 0, 1, 2, 3
 * Layout finale Registro 1 (MSB -> LSB): [K3][K2][K1][K0][K4][K3][K2][K1]
 * Stato temp_kindex al termine del blocco = 4
 *
 * [key_blocks[2]] -> Inizia da temp_kindex = 4
 * Sequenza lineare degli indici catturati nel ciclo: 4, 0, 1, 2, 3, 4, 0, 1
 * Layout finale Registro 2 (MSB -> LSB): [K1][K0][K4][K3][K2][K1][K0][K4]
 * Stato temp_kindex al termine del blocco = 2
 *
 * [key_blocks[3]] -> Inizia da temp_kindex = 2
 * Sequenza lineare degli indici catturati nel ciclo: 2, 3, 4, 0, 1, 2, 3, 4
 * Layout finale Registro 3 (MSB -> LSB): [K4][K3][K2][K1][K0][K4][K3][K2]
 * Stato temp_kindex al termine del blocco = 0
 *
 * [key_blocks[4]] -> Inizia da temp_kindex = 0
 * Sequenza lineare degli indici catturati nel ciclo: 0, 1, 2, 3, 4, 0, 1, 2
 * Layout finale Registro 4 (MSB -> LSB): [K2][K1][K0][K4][K3][K2][K1][K0]
 * Stato temp_kindex al termine del blocco = 3 (Il cerchio si chiude perfettamente).
 *
 * -------------------------------------------------------------------------------------
 * FASE 3: SATURAZIONE DELLA PIPELINE NEL LOOP DATI (EXECUTION)
 * -------------------------------------------------------------------------------------
 * Durante l'elaborazione del payload pesante, la CPU esegue un ciclo privo di ramificazioni
 * complesse o calcoli aritmetici degradanti:
 *
 * Iterazione 0: *(uint64_t *)init ^= key_blocks[0]; (Cifra 8 byte con K3..K0) -> block_index = 1
 * Iterazione 1: *(uint64_t *)init ^= key_blocks[1]; (Cifra successivi 8 byte) -> block_index = 2
 * Iterazione 2: *(uint64_t *)init ^= key_blocks[2]; (Cifra successivi 8 byte) -> block_index = 3
 * Iterazione 3: *(uint64_t *)init ^= key_blocks[3]; (Cifra successivi 8 byte) -> block_index = 4
 * Iterazione 4: *(uint64_t *)init ^= key_blocks[4]; (Cifra successivi 8 byte) -> block_index = 0
 *
 * Se il buffer continua, l'operatore ternario branchless resetta 'block_index' a 0
 * e ricomincia a ciclare sull'array nello Stack a costo zero.
 *
 * Ad ogni iterazione, l'indice reale della chiave 'kindex' viene tenuto in sincro tramite:
 * kindex = (kindex + 8) % keylen;
 *
 * Se ad esempio il loop centrale elabora esattamente un ciclo completo di 5 blocchi (40 byte):
 * kindex_finale = (3 + 40) % 5 = 43 % 5 = 3
 *
 * Il kindex per l'Epilogo Scalare è preservato a 3. Se avanzano da 1 a 7 byte residui,
 * l'epilogo sa esattamente che deve ripartire a cifrare byte-by-byte da key[3].
 * La continuità crittografica è garantita, la divisione hardware è azzerata.
 * -------------------------------------------------------------------------------------
 */
void print_hex(const char *label, unsigned char *data, size_t len)
{
    printf("%s:\n", label);
    for (size_t i = 0; i < len; i++)
    {
        printf("%02X ", data[i]);
        if ((i + 1) % 8 == 0)
            printf(" "); // Spazio ogni 8 byte per leggibilità
        if ((i + 1) % 16 == 0)
            printf("\n");
    }
    printf("\n");
}

int main()
{
    /* Buffer di 32 byte azzerati. Lo XOR con zero esporrà direttamente il keystream
     * generato dall'algoritmo, permettendo la verifica visiva della periodicità. */
    unsigned char data[32] = {0};
    size_t data_len = 32;

    /* Chiave periodica asimmetrica di 3 byte (Disallineata rispetto alla word di macchina) */
    unsigned char key[] = {0xAA, 0xBB, 0xCC};
    size_t key_len = 3;

    /* Parametri di stress-test per la cifratura selettiva:
     * L'offset a 3 costringe il prologo a girare per 5 byte per raggiungere l'allineamento a 8.
     * La lunghezza a 23 copre prologo, blocchi SWAR intermedi ed epilogo residuo. */
    size_t offset = 3;
    size_t region_len = 23;

    printf("--- TEST SELECTIVE ENCRYPTION (SWAR) ---\n\n");
    print_hex("Dati originali (In chiaro)", data, data_len);

    /* Esecuzione dell'algoritmo ottimizzato */
    xor_mask_region(data, key, key_len, offset, region_len);

    print_hex("Dati modificati (Dopo XOR)", data, data_len);

    return 0;
}
