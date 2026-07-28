/*******************************************************************************
 * TEORIA, APPLICAZIONI E IMPLEMENTAZIONI HARDWARE DELLA SOTTRAZIONE CON SATURAZIONE
 *******************************************************************************
 * 
 * 1. BACKGROUND TEORICO
 * 
 * La saturazione aritmetica (Saturating Arithmetic) e' un tipo di aritmetica 
 * in cui le operazioni matematiche sono limitate all'interno di un intervallo 
 * fisso (in questo caso tra 0 e UINT_MAX).
 * 
 * A differenza dell'aritmetica modulare standard (Unsigned Wrap-Around), dove 
 * un underflow come (0 - 1) produce il valore massimo 0xFFFFFFFF (4294967295), 
 * la sottrazione saturata blocca (satura) il risultato al limite inferiore (0) 
 * non appena il sottraendo supera il minuendo.
 * 
 * Analisi dell'implementazione branchless:
 * - Espressione relazionale: (a >= b) valuta a 1 (vero) oppure 0 (falso).
 * - Generazione della maschera bitwise: -(a >= b)
 *   * Se a >= b (1): -1 in complemento a due genera una maschera di tutti bit a 1 
 *     (0xFFFFFFFF per uint32).
 *   * Se a < b (0): -0 genera una maschera di tutti bit a 0 (0x00000000).
 * - Sottrazione e mascheramento: (a - b) & maschera
 *   * Se a >= b: (a - b) & 0xFFFFFFFF restituisce la differenza esatta (a - b).
 *   * Se a < b: anche se (a - b) va in wrap-around unsigned, l'operazione & 0x0 
 *     azzera istantaneamente il risultato restituendo 0 senza generare branch.
 * - Garanzia di C Standard: L'underflow su tipi unsigned int e' definito 
 *   esplicitamente dallo standard C come aritmetica modulo 2^N, rendendo l'espressione 
 *   sicura e priva di Undefined Behavior.
 * 
 * -----------------------------------------------------------------------------
 * 2. APPLICAZIONI PRATICHE
 * -----------------------------------------------------------------------------
 * - Elaborazione Digitale delle Immagini (Pixel Blending): I canali colore (RGB) 
 *   vengono memorizzati come valori senza segno (8-bit o 16-bit). Nel calcolo di 
 *   ombre, dissolvenze o sottrazione di sfondi, sottrare luminescenza senza 
 *   saturazione causerebbe glitch visivi estremi (un pixel leggermente piu scuro 
 *   diventerebbe improvvisamente bianco brillante per via del wrap-around).
 * - Elaborazione Audio e DSP (Digital Signal Processing): Evita il fenomeno del 
 *   "Clipping Wrap-Around". Quando si abbassa il volume o si applicano filtri 
 *   audio, la saturazione garantisce che un segnale sotto il minimo venga 
 *   stroncato a silenzio (0) invece di generare un rumore impulsivo massimo 
 *   (distorsione digitale da underflow).
 * - Motori Fisici e Game Engine: Calcolo di decremento vita (HP), scudi, 
 *   timer o velocita di attrito. Impedisce che un danno superiore agli HP attuali 
 *   conferisca vita infinita al giocatore per via dell'underflow.
 * - Algoritmi di Rete (Token Bucket Rate Limiting): Gestione dei contatori di 
 *   bandwidth e dei pacchetti inviati, dove i token non possono scendere sotto zero.
 * 
 * -----------------------------------------------------------------------------
 * 3. IMPLEMENTAZIONI HARDWARE
 * -----------------------------------------------------------------------------
 * - Istruzioni SIMD dedicate (x86 e ARM): 
 *   * Su architetture x86/x64, le estensioni SSE/AVX mettono a disposizione 
 *     istruzioni hardware dedicate come PSUBUSB (Packed Subtract Unsigned 
 *     Bytes with Saturation) o PSUBUSW (Unsigned Words).
 *   * Su architettura ARM (NEON / ARM64), l'istruzione hardware equivalente e' 
 *     UQSUB (Unsigned Saturating Subtract).
 *   Tali istruzioni eseguono la sottrazione saturata in parallelo su vettori 
 *   di dati in un singolo ciclo di clock.
 * - Evitamento delle Misprediction nelle Pipeline (Branchless Advantage): 
 *   Nei processori superscalari con pipeline profonde, una struttura if (a < b) 
 *   puo causare stalli di pipeline (Pipeline Flush) drammatici in caso di 
 *   errata predizione del salto (Branch Misprediction). Trasformare la logica 
 *   in operazione aritmetico-bitwise elimina i salti condizionali (Branchless Code), 
 *   mantenendo il flusso di istruzioni costante ed eseguibile in pochi cicli ALU.
 * - Circuiti Combinatori ALU (Borrow Flag): A livello di silicio, l'ALU esegue 
 *   la Sottrazione Aritmetica e usa direttamente il flag di Carry/Borrow prodotto 
 *   dal sommatore finale per pilotare un banco di Multiplexer (MUX) che seleziona 
 *   tra il risultato della sottrazione e lo zero.
 *******************************************************************************/

#include <stdio.h>

// Ritorna a - b se a >= b, altrimenti 0.
// Implementazione branchless basata su mascheramento tramite il segno dell'operatore di confronto.
unsigned int saturating_sub32(unsigned int a, unsigned int b) 
{
    return (a - b) & -(a >= b);
}

int main() 
{
    // Test 1: Caso standard (a >= b)
    unsigned int a1 = 255;
    unsigned int b1 = 1;
    printf("saturating_sub32(%u, %u) = %u\n", a1, b1, saturating_sub32(a1, b1)); // Output: 254

    // Test 2: Caso saturato (a < b)
    unsigned int a2 = 255;
    unsigned int b2 = 256;
    printf("saturating_sub32(%u, %u) = %u\n", a2, b2, saturating_sub32(a2, b2)); // Output: 0

    // Test 3: Limiti estremi (0 - valore)
    unsigned int a3 = 0;
    unsigned int b3 = 42;
    printf("saturating_sub32(%u, %u) = %u\n", a3, b3, saturating_sub32(a3, b3)); // Output: 0

    return 0;
}