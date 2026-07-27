#include <stdio.h>
/*******************************************************************************
 * TEORIA, APPLICAZIONI E HARDWARE DELLA BIT-PLANE EXTRACTION
 *******************************************************************************
 * 
 * 1. BACKGROUND TEORICO
 * 
 * La "Bit-Plane Extraction" (Estrazione dei Piani di Bit) è una tecnica 
 * fondamentale nell'elaborazione dei segnali digitali. Qualsiasi blocco di dati 
 * a 8 bit (come un'immagine in scala di grigi) può essere scomposto in 8 
 * array separati (i "piani"). Il piano 7 contiene i bit più significativi (MSB) 
 * di ogni byte, che portano la maggior parte delle informazioni visive/strutturali. 
 * Il piano 0 contiene i bit meno significativi (LSB), che spesso assomigliano a rumore.
 * 
 * Nel codice, l'estrazione e l'impacchettamento avvengono tramite due coordinate:
 * - L'indice del byte di destinazione: calcolato con (i >> 3), che equivale 
 *   a i / 8. Avanza di 1 ogni 8 bit elaborati.
 * - Lo shift all'interno del byte di destinazione: calcolato con (7 - (i & 7)).
 *   Garantisce che il primo bit estratto (i=0) finisca nella posizione 7 (MSB),
 *   il secondo (i=1) nella posizione 6, e così via fino alla posizione 0 (LSB).
 * 
 * -----------------------------------------------------------------------------
 * 2. APPLICAZIONI PRATICHE
 * -----------------------------------------------------------------------------
 * - Compressione Immagini e Fax (CCITT Group 4): I fax trasmettono immagini 
 *   in bianco e nero (1 bit per pixel). Estrarre i bit planari permette di 
 *   applicare algoritmi di Run-Length Encoding (RLE) in modo estremamente 
 *   efficiente, dato che i piani superiori (MSB) tendono ad avere lunghe 
 *   sequenze di bit identici (es. lunghi tratti bianchi o neri continui).
 * - Steganografia e Watermarking: I messaggi segreti o le firme digitali 
 *   vengono spesso "nascosti" alterando esclusivamente il piano 0 (LSB) di 
 *   un'immagine o di un file audio. A livello percettivo umano il rumore introdotto 
 *   è invisibile/inudibile, ma l'estrazione di quel piano rivela i dati occulti.
 * - Computer Vision e Medical Imaging (DICOM): La scomposizione in piani 
 *   aiuta a isolare i contorni netti (high-order bit planes) dal rumore di 
 *   fondo acquisito dai sensori delle risonanze magnetiche o dei raggi X,
 *   facilitando gli algoritmi di edge-detection.
 * 
 * -----------------------------------------------------------------------------
 * 3. IMPLEMENTAZIONI HARDWARE
 * -----------------------------------------------------------------------------
 * - Istruzioni x86 SIMD / BMI2: Le moderne CPU Intel e AMD offrono l'istruzione
 *   hardware PEXT (Parallel Bits Extract) all'interno dell'estensione BMI2.
 *   Questa istruzione fa esattamente ciò che fa questo algoritmo, ma a livello di 
 *   silicio in un singolo ciclo di clock, usando una maschera per raccogliere e 
 *   comprimere bit sparsi nei registri a 64 bit.
 * - Architetture GPU e Texture Mapping: Nelle pipeline grafiche (OpenGL/Vulkan),
 *   i formati di texture compressi memorizzano i dati dei canali (RGBA)
 *   utilizzando blocchi di bit impacchettati. I core delle GPU (Shader) hanno 
 *   unità ALU dedicate al "bit-slicing" per spacchettare e ricampionare questi
 *   piani di bit in tempo reale a larghezze di banda nell'ordine dei Terabyte/s.
 * - Shift Registers in FPGA/ASIC: Su hardware custom, questa operazione si 
 *   implementa brutalmente tramite dei registri a scorrimento (Shift Register)
 *   parallelo-seriale (PISO) connessi a banchi di multiplexer che instradano i 
 *   singoli bit sulle linee d'uscita a zero latenza logica.
 * 
 * -----------------------------------------------------------------------------
 * 4. POSTILLA: IL TRUCCO DEL NOT BIT A BIT
 * -----------------------------------------------------------------------------
 * Nel codice è stata utilizzata l'espressione: (7 - (i & 7)).
 * Questa formula è chiara, leggibile e standard.
 * Tuttavia, in base alle regole del complemento a due per i numeri binari, 
 * sottrarre un valore da 7 (che in binario è 111) equivale esattamente 
 * a invertire (flippare) i suoi bit più bassi. 
 * Quindi, l'operazione aritmetica (7 - (i & 7)) è computazionalmente e 
 * logicamente identica all'operazione bitwise ((~i) & 7). 
 * Entrambe mappano l'indice [0, 1, 2, 3, 4, 5, 6, 7] sulla posizione
 * di shift [7, 6, 5, 4, 3, 2, 1, 0].
 *******************************************************************************/

#include <stdio.h>

// Dato un array di n byte, estrae il bit numero plane (0-7) da ogni byte e lo impacchetta in out come bitmap.
// Esempio: array {0xAA, 0x55} = {10101010, 01010101}, plane=7 estrae i bit 7 = {1, 0} → out[0] = 0b10000000.
void bit_plane_extract(unsigned char *bytes, int n, unsigned int plane, unsigned char *out)
{
    // Sicurezza: mascheriamo plane per essere certi che sia tra 0 e 7
    plane &= 7; 
    
    // Ciclo sui byte in input
    for (int i = 0; i < n; i++)
    {
        // Isola il bit desiderato portandolo in posizione 0
        unsigned char bit = (bytes[i] >> plane) & 1;
        
        // Impacchetta il bit nell'array di output
        // (i >> 3) avanza il byte di destinazione ogni 8 iterazioni
        // (7 - (i & 7)) posiziona il bit decrescendo da 7 a 0
        out[i >> 3] |= (bit << (7 - (i & 7)));
    }
}

int main() 
{
    // Array di test
    unsigned char array[] = {0XAA, 0x55, 0x80, 0x80, 0x08, 0x80, 0x80, 0x08, 0XAA, 0x55, 0x80, 0x80, 0x08, 0x80, 0x80, 0x08, 0xAA};
    
    // Allocazione stack e inizializzazione esplicita a zero (Cruciale per operatori bitwise |=)
    unsigned char out[(sizeof(array) + 7) >> 3] = {0};
    
    // Estrazione del bit più significativo (plane 7)
    bit_plane_extract(array, sizeof(array), 7, out);
    
    // Stampa i risultati attesi: 182 182 128
    printf("%u %u %u\n", out[0], out[1], out[2]); 
    
    return 0;
}
