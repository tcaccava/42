#include <stdio.h>
#include "./popcount_swar32.c"


extern int popcount_swar32(unsigned int n);

/*====================================================================================================
 * BACKGROUND TEORICO GENERALE: BIT COMPACTION (COMPATTAZIONE DI CAMPI DI BIT)
 * ====================================================================================================
 * Il problema di estrarre e compattare bit sparsi (in questo caso i bit in posizione pari: 0, 2, 4... 30)
 * eliminando i "gap" vuoti per stiparli in modo contiguo nei bit meno significativi (LSB) prende il nome
 * di Bit Compaction o Parallel Bit Extract.
 * * Esistono due filosofie computazionali per risolvere questo problema a basso livello:
 * * 1) APPROCCIO ITERATIVO DINAMICO BASATO SU LSB (Versione Custom):
 * Sfrutta le proprietà algebriche del complemento a due per isolare i bit attivi uno alla volta.
 * La complessità temporale è O(K), dove K è il numero di bit effettivamente accesi (Popcount).
 * È un approccio asimmetrico: ultra-veloce se ci sono pochi bit accesi, più lento se il registro è denso.
 * * 2) APPROCCIO LOGARITMICO PARALLELO SWAR (Versione SWAR):
 * Sfrutta il paradigma "Divide et Impera" applicato direttamente all'interno del registro (SIMD Within A Register).
 * Invece di elaborare i bit individualmente, i gap geometrici tra i bit attivi vengono dimezzati ad ogni step.
 * La complessità temporale è rigorosamente fissa e deterministica: O(\log N), ovvero 4 passaggi per un registro a 32 bit.
 * È un approccio simmetrico: immune da variazioni di tempo basate sui dati in ingresso.
 * * ====================================================================================================
 * IMPLEMENTAZIONE HARDWARE
 * ====================================================================================================
 * - ARCHITETTURA x86 (Intel Haswell+, AMD Ryzen+):
 * Entrambe le versioni software vengono superate a livello hardware dall'estensione BMI2 (Bit Manipulation Instruction Set 2).
 * Gli ingegneri Intel hanno mappato questa esatta operazione direttamente nel silicio tramite l'istruzione nativa:
 * 'PEXT' (Parallel Bit Extract).
 * La CPU x86 esegue l'intera estrazione e compattazione in 1 singolo ciclo di clock utilizzando una maschera hardware.
 * Esempio Assembly: PEXT eax, edx, 0x55555555 (estrae i bit pari da edx e li compatta in eax).
 * * - ARCHITETTURA ARM (Apple Silicon M1/M2/M3, Cortex):
 * I processori ARM non possiedono un equivalente hardware di PEXT. Le operazioni di estrazione parallela non standard
 * devono essere emulate software. Di conseguenza, l'implementazione SWAR logaritmica (Versione 2) rappresenta lo
 * STATO DELL'ARTE ASSOLUTO su piattaforme ARM per l'estrazione di pattern fissi, poiché azzera i cicli di clock persi
 * in branch misprediction ed ottimizza l'uso delle pipeline di esecuzione out-of-order.
 * * ====================================================================================================
 * APPLICAZIONI PRATICHE NEL REAL-TIME COMPUTING
 * ====================================================================================================
 * 1) CURVE DI MORTON / Z-ORDER (Game Dev & Database Spaziali):
 * Per mappare coordinate bidimensionali (X, Y) in un indice lineare di memoria a 1D preservando la località spaziale,
 * i bit di X e Y vengono interlacciati (es. X0 Y0 X1 Y1 X2 Y2...). Quando il motore grafico o il database deve
 * effettuare il DECODING dell'indice per recuperare la coordinata X originale, deve estrarre e compattare esattamente
 * tutti i bit in posizione pari. Questa funzione è il core critico di quel processo.
 * * 2) MANIPOLAZIONE DI PIXEL FORMATS / BITMAPS:
 * Estrazione rapida di canali colore o dati stencil memorizzati in formati bit-packed alternati. Ad esempio,
 * nei flussi video YUYV (dove i dati di luminanza e crominanza sono alternati a livello di bit/byte), la compattazione
 * di bit parallela permette di isolare un intero canale senza dover allocare buffer temporanei o eseguire cicli for pesanti.
 */

/**
 * VERSIONE 1: APPROCCIO CUSTOM (Algoritmo Dinamico O(K))
 * Sviluppato tramite isolamento matematico dell'LSB e calcolo dell'indice via Popcount.
 */
unsigned int compress_even_bits_custom(unsigned int n)
{
    unsigned int mask = 0x55555555;
    unsigned int even_bits = n & mask; // Isola i bit pari azzerando i dispari.
    unsigned int res = 0;

    // Il ciclo esegue tante iterazioni quanti sono i bit pari effettivamente accesi (K).
    while (even_bits)
    {
        // 1. ISOLAMENTO LSB: Sfrutta il complemento a due.
        // Negare even_bits inverte tutti i bit e aggiunge 1. Questo fa sì che tutti i bit a sinistra
        // dell'LSB originale siano invertiti, mentre l'LSB e gli zeri alla sua destra rimangono identici.
        // L'AND bitwise (&) cancella tutto il resto del registro, lasciando unicamente l'LSB attivo.
        unsigned int lsb = even_bits & (-even_bits); 

        // 2. CONTEGGIO TRAILING ZEROS VIA POPCOUNT:
        // Sottraendo 1 a 'lsb' (che ha un solo bit a 1, il bit a 1 si azzera
        // e tutti i bit alla sua destra (gli zeri di padding) si accendono a 1.
        // Passando questo valore a popcount_swar32, otteniamo il numero esatto di zeri che precedevano l'LSB,
        // che corrisponde matematicamente all'indice hardware posizionale (0-31) del bit nel registro.
        int lsb_pos = popcount_swar32(lsb - 1); 

        // 3. MAPPATURA COMPRESSA DIMEZZATA:
        // Poiché stiamo campionando solo posizioni pari (0, 2, 4, 6...), dividendo l'indice per 2 (lsb_pos / 2
        // o lsb_pos >> 1) eliminiamo lo spazio vuoto, mappando i bit in posizioni contigue nel registro di output (res).
        res |= (1 << (lsb_pos >> 1)); 

        // 4. AZZERAMENTO BIT ELABORATO:
        // Lo XOR (^) tra il vettore globale e l'unico bit isolato inverte (spegne) l'LSB appena processato,
        // facendo avanzare lo stato del registro verso la condizione di terminazione del ciclo (even_bits == 0).
        even_bits ^= lsb; 
    }
    return res;
}

/**
 * VERSIONE 2: APPROCCIO SWAR LOGARITMICO (Algoritmo Deterministico O(log N))
 * Sviluppato tramite collasso geometrico progressivo dei gap binari.
 */
unsigned int compress_even_bits_swar(unsigned int n)
{
    // STEP 1: ISOLAMENTO RADICALE
    // Elimina i bit dispari lasciando intatti solo i bit pari.
    n = n & 0x55555555; 

    // STEP 2: COLLASSO COPPIE ADIACENTI (Eliminazione gap di 1 bit)
    // Sposta i bit in posizione 2k nella posizione k all'interno di ogni blocco di 2 bit.
    // Lo shift a destra di 1 allinea i bit pari con le posizioni dispari liberate. L'OR logico combina i flussi.
    // La maschera 0x33333333 (00110011...) isola le coppie dense appena create.

    n = (n | (n >> 1)) & 0x33333333; 

    // STEP 3: COLLASSO QUARTETTI / NIBBLES (Eliminazione gap di 2 bit)
    // Sposta le coppie dense per unire i blocchi isolati. Lo shift di 2 scavalca il gap di 2 bit vuoti.
    // La maschera 0x0F0F0F0F (00001111...) isola i quartetti densi risultanti.
    n = (n | (n >> 2)) & 0x0F0F0F0F; 

    // STEP 4: COLLASSO BYTE (Eliminazione gap di 4 bit)
    // Sposta i quartetti per formare byte compatti staccati. Lo shift di 4 salta il gap di 4 zeri.
    // La maschera 0x00FF00FF isola gli 8 bit densi all'interno di ogni mezza word (16 bit).
    n = (n | (n >> 4)) & 0x00FF00FF; 

    // STEP 5: COMPATTAZIONE FINALE (Eliminazione gap di 8 bit)
    // Ultimo shift critico: sposta il byte alto di 8 posizioni per farlo collidere con il byte basso.
    // La maschera 0x0000FFFF pulisce la metà superiore del registro a 32 bit.

    n = (n | (n >> 8)) & 0x0000FFFF; 

    return n;
}

int main()
{
    unsigned int test1 = 0x55555555; // Caso limite massimo: tutti i 16 bit pari accesi
    unsigned int test2 = 0x55005500; // Caso sparso: bit accesi solo nelle sezioni superiori dei byte
    unsigned int test3 = 0x00000005; // Caso minimale: solo bit 0 e bit 2 accesi (risultato atteso: 0x3)

    printf("=== VERIFICA STRUTTURALE DEGLI ALGORITMI ===\n\n");

    printf("Test 1 [0x55555555]:\n");
    printf("  -> Custom: %#X\n", compress_even_bits_custom(test1));
    printf("  -> SWAR:   %#X\n\n", compress_even_bits_swar(test1));

    printf("Test 2 [0x55005500]:\n");
    printf("  -> Custom: %#X\n", compress_even_bits_custom(test2));
    printf("  -> SWAR:   %#X\n\n", compress_even_bits_swar(test2));

    printf("Test 3 [0x00000005]:\n");
    printf("  -> Custom: %#X\n", compress_even_bits_custom(test3));
    printf("  -> SWAR:   %#X\n", compress_even_bits_swar(test3));

    return 0;
}