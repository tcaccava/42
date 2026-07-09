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
 * COLLO DI BOTTIGLIA (Evoluzioni Future):
 * Nel ciclo centrale, la ricostruzione della maschera a 64-bit (key64) avviene tramite
 * un ciclo scalar loop interno (8 iterazioni con operazioni di shift e modulo '%'). L'operatore
 * di modulo % richiede tra i 10 e i 40 cicli di clock a seconda della CPU. 
 * Per raggiungere prestazioni estreme senza istruzioni proprietarie (AVX/SIMD), le strade sono:
 * A) Pre-packing della chiave: Espandere e pre-calcolare le maschere a 64-bit prima del ciclo.
 * B) Auto-Vectorization: Scrivere un ciclo lineare a singolo byte e compilare con "gcc -O3",
 * permettendo al compilatore di srotolare il ciclo (Loop Unrolling) e iniettare vettori hardware.
 * -------------------------------------------------------------------------------------
 */

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

void xor_mask_region(unsigned char *data,unsigned char *key, size_t keylen, size_t offset, size_t region_len)
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

        block ^= key64;             /* XOR parallelo simultaneo su 8 byte */
        *(uint64_t *)init = block;  /* Scrittura atomica a 64-bit in memoria */
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

int main(void)
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

