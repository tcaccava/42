#include <stdio.h>

/**
 * =======================================================================================
 * BACKGROUND TEORICO, ARCHITETTURALE E APPLICAZIONI DELLA ROTAZIONE PARALLELA DEI NIBBLE
 * =======================================================================================
 * * 1. ALGEBRA DEL CIRCULAR SHIFT E ISOLAMENTO DEI DOMINI
 * A differenza degli shift logici (<<, >>) che iniettano zeri e distruggono l'informazione
 * facendola "cadere" fuori dal registro, la rotazione (Circular Shift) è un'operazione che
 * conserva rigorosamente l'entropia del dato. È una permutazione biunivoca perfetta.
 * * Formalmente, dato un registro di ampiezza N e un passo di rotazione k, la mappatura geometrica
 * del bit in posizione i nella nuova posizione f(i) è definita come:
 * f(i) = (i + k) mod N
 * * Nel caso specifico della funzione sottostante, il registro a 32 bit viene partizionato in
 * 8 sotto-domini indipendenti e paralleli di ampiezza fissa N = 4 (i singoli nibble).
 * Ciascun nibble si comporta come un'isola topologica chiusa: il bit che viene spinto fuori
 * dal confine superiore (MSB del nibble) deve rientrare istantaneamente dal confine inferiore
 * (LSB dello stesso nibble), senza alcuna contaminazione o "sanguinamento" (bleeding) nei
 * nibble adiacenti.
 * * 2. IL PARADIGMA COMPUTAZIONALE SWAR (SIMD WITHIN A REGISTER)
 * Questa implementazione sposa la filosofia SWAR. Non potendo o non volendo allocare vettori
 * hardware espliciti (come le estensioni SIMD Intel AVX o ARM Neon), si sfrutta la ALU standard
 * a 32 bit come se fosse un processore vettoriale.
 * Generando maschere dinamiche espanse tramite la costante magica moltiplicativa 0x11111111U,
 * si applica l'algoritmo di rotazione contemporaneamente su 8 flussi di dati indipendenti a 4 bit.
 * * 3. EMULAZIONE DELLE LACUNE STRUTTURALI DELLE ISA (ISA GAP)
 * I moderni set di istruzioni (Intel x86, ARM, RISC-V) dispongono di istruzioni native codificate
 * nel silicio per la rotazione di interi registri (es. ROL/ROR in x86). Tuttavia, nessuna ISA
 * commerciale possiede un'istruzione in grado di frammentare un registro ed eseguire rotazioni
 * bitwise su granularity sub-byte (4 bit). Questa funzione colma il vuoto istruzionale (ISA Gap)
 * garantendo massime prestazioni senza la decomposizione del registro in memoria.
 * * 4. APPLICAZIONI PRATICHE NELL'INFORMATICA PRESTAZIONALE
 * A) Conversione di Endianness, Formati e Pixel Packing (BSWAP):
 * Mentre le comunicazioni di rete e molte architetture RISC lavorano in Big-Endian, i sistemi
 * x86 sono nativamente Little-Endian. La manipolazione parallela di byte e nibble permette il
 * riordinamento dei flussi di dati in tempo reale. Un'applicazione critica risiede nel
 * Pixel-Shuffling (es. conversione istantanea dei canali di colore da formato RGBA a ARGB o BGRA
 * prima del caricamento nel frame buffer della GPU).
 * * B) Crittografia Simmetrica, Funzioni di Hash e Cifrari ARX:
 * I moderni algoritmi di cifratura ad altissima velocità (come ChaCha20 o Salsa20, pilastri del
 * protocollo TLS/HTTPS) si basano su primitive ARX (Add-Rotate-XOR). L'addizione introduce non-linearità,
 * mentre lo XOR e la rotazione distruggono la correlazione lineare tra l'input e l'output
 * garantendo la "diffusione" (principio di Shannon). Cambiare un singolo bit in input deve
 * propagarsi e stravolgere statisticamente il 50% dei bit in output; la rotazione dei nibble
 * accelera radicalmente questo processo di miscelazione logica.
 * * C) Compressione e Bit-Packing nell'Inferenza AI Quantizzata (LLM Edge Computing):
 * Nei motori di esecuzione locali per modelli linguistici (LLM quantizzati in INT4), i pesi sinaptici
 * vengono compressi a 4 bit per dimezzare l'impatto sulla larghezza di banda della memoria.
 * Ogni singolo byte contiene quindi due pesi distinti. Per processare le attivazioni matematiche
 * senza eseguire uno scompattamento (unpacking) in registri float (operazione che distruggerebbe
 * le performance), la CPU elabora il primo nibble e, tramite rotazioni mirate, espone il secondo
 * peso in posizione utile per l'operazione successiva mantenendo il dato compresso nel registro.
 * * 5. DETERMINISMO HARDWARE E SICUREZZA CRITTOGRAFICA (CONSTANT-TIME)
 * A livello fisico di CPU, l'utente può richiedere rotazioni dinamiche non note a tempo di compilazione.
 * L'hardware gestisce questo scenario tramite il "Barrel Shifter", una cascata geometrica di multiplexer
 * disposti in stadi logici equivalenti a log2(Word_Size) - nel caso di 32 bit, 5 stadi sequenziali di transistor.
 * * Un punto cruciale di questa implementazione software è che l'algoritmo è totalmente BRANCHLESS:
 * non esistono salti condizionali (`if`, `else`, `switch` o cicli). Di conseguenza:
 * - Previene il Branch Misprediction, mantenendo intatta la pipeline di esecuzione della CPU.
 * - È matematicamente "Constant-Time": il tempo di esecuzione e il consumo energetico sul silicio
 * sono perfettamente costanti e indipendenti dai valori di 'n' o 'k'. Questo neutralizza alla radice
 * i Side-Channel Timing Attacks, impedendo a un attaccante esterno di ricostruire il dato sensibile
 * analizzando i tempi di risposta del processore.
 */
unsigned int rotate_bits_in_nibbles(unsigned int n, int k)
{
    k &= 3;                                              // riconduciamo k al resto di 4
    unsigned char mask_keep = (1U << (4 - k)) - 1;       // la maschera che useremo per i bit che rimangono nel registro
    unsigned char mask_wrap = (1U << k) - 1;             // la maschera che useremo per i bit che escono a sinistra
    unsigned int mask_keep_32 = mask_keep * 0x11111111U; // estendiamo le maschere in un intero a 32 bit
    unsigned int mask_wrap_32 = mask_wrap * 0x11111111U;
    unsigned int half1 = (n & mask_keep_32) << k;       // isoliamo i keep bit nel numero originario e li shiftiamo di k
    unsigned int half2 = (n >> (4 - k)) & mask_wrap_32; // shiftiamo il numero originario di 4 - k e poi isoliamo i bit shiftati
    return half1 | half2;                               // ritorniamo l'OR delle due operazioni di masking
}