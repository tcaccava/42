#include <stdio.h>

/*******************************************************************************
 * BRANCHLESS CLAMPING & NIBBLE PACKING - TEORIA, PRATICA ED HARDWARE
 *******************************************************************************
 * 
 * 1. BACKGROUND TEORICO
 * 
 * A. Il trucco del Branchless Min: n = n ^ ((n ^ 8) & -(n > 8))
 * Questo pattern implementa matematicamente la funzione min(n, 8) senza salti.
 * Si basa sulla generazione di una Bitmask condizionale usando il C standard:
 * - In C, gli operatori relazionali (come >) ritornano rigidamente 1 o 0.
 * - La negazione aritmetica in complemento a due (-x) mappa:
 *      0 -> 0x00000000 (Tutti 0)
 *      1 -> 0xFFFFFFFF (Tutti 1)
 * - La logica dell'XOR:
 *      Se n > 8 (Mask = 0xFFFFFFFF): n ^ (n ^ 8) = 8. (La n si elide).
 *      Se n <= 8 (Mask = 0x00000000): n ^ 0 = n.
 * Il risultato è una selezione in O(1) puro, che garantisce l'assenza di stalli 
 * nella pipeline della CPU dovuti a branch misprediction.
 * 
 * B. Data Packing (Nibbles)
 * Un "Nibble" equivale a 4 bit (mezzo byte, capace di contenere valori da 0 a 15).
 * Impacchettare nibbles in un word da 32 bit significa comprimere 8 unità di 
 * informazione in un singolo registro della CPU. L'operazione bitwise (OR + SHIFT) 
 * unisce le frazioni di dato sfruttando il parallelismo spaziale dei registri.
 * 
 * -----------------------------------------------------------------------------
 * 2. APPLICAZIONI PRATICHE
 * -----------------------------------------------------------------------------
 * - Voxel Engines (es. Minecraft, motori raycasting a blocchi):
 *   I dati dei blocchi spesso richiedono pochi bit (es. 4 bit per l'ID del blocco, 
 *   4 bit per il livello di illuminazione). Vengono impacchettati a coppie dentro 
 *   un singolo byte per dimezzare il consumo di RAM e raddoppiare l'efficienza 
 *   della cache della CPU.
 * 
 * - Network Protocols (TCP/IP):
 *   Gli header di rete sono progettati a nibbles. Nell'header IPv4, il campo 
 *   "Version" (4 bit) e il campo "IHL - Internet Header Length" (4 bit) sono 
 *   compressi nel primo byte del pacchetto.
 * 
 * - Crittografia (S-Boxes):
 *   Algoritmi come il DES (Data Encryption Standard) mappano blocchi da 6 bit a 
 *   nibbles da 4 bit. Manipolare multipli nibbles simultaneamente in un registro 
 *   è essenziale per implementazioni veloci.
 * 
 * -----------------------------------------------------------------------------
 * 3. HARDWARE ED ASSEMBLY (TRANSLATION)
 * -----------------------------------------------------------------------------
 * A. Come si traduce il branchless min nell'hardware moderno?
 * I compilatori moderni (GCC/Clang -O2/-O3) riconoscono questo pattern bitwise 
 * magico e lo traducono direttamente nell'istruzione hardware dedicata, saltando 
 * del tutto le operazioni logiche!
 * - Su x86_64, genera una CMOV (Conditional Move):
 *      cmp eax, 8          ; Compara n con 8
 *      mov edx, 8          ; Prepara 8 in un registro
 *      cmova eax, edx      ; Se eax è "Above" (maggiore), sposta 8 in eax
 * 
 * - Su ARM64 (Apple Silicon, Raspberry), genera una CSEL (Conditional Select):
 *      cmp w0, 8           ; Compara
 *      csel w0, w0, w1, ls ; Se "Less or Same", tieni w0, altrimenti prendi w1 (8)
 * 
 * B. SIMD e Vettorizzazione:
 * Se dovessi fare questa operazione su 16 interi contemporaneamente, i branch 
 * classici (if) fallirebbero miseramente perché i processori SIMD (AVX/NEON) 
 * non possono saltare diversamente per ogni elemento. Il pattern bitwise 
 * con maschera che hai usato è letteralmente l'unico modo per fare selezione 
 * condizionale dentro i registri vettoriali (VBLEND/VAND/VOR).
 *******************************************************************************/

#include <stdio.h>
#include <stdint.h>

// ============================================================================
// FUNZIONE UTENTE 
// Impacchetta fino a 8 nibbles (valori 0-15) in un singolo uint32_t.
// ============================================================================
uint32_t pack_nibbles(unsigned char *arr, int n)
{
    uint32_t res = 0;
    
    // Branchless clamp: n = min(n, 8)
    // Garantisce che il loop non ecceda mai gli 8 elementi (32 bit totali).
    n = n ^ ((n ^ 8) & -(n > 8));
    
    unsigned int shift = 0;
    for (int i = 0; i < n; i++)
    {
        // (arr[i] & 0x0F) garantisce che valori sporchi (>15) nell'array 
        // non vadano a sovrascrivere i bit adiacenti superiori.
        uint32_t val = (uint32_t)(arr[i] & 0x0F) << shift;
        res |= val;
        shift += 4;
    }
    
    return res;
}

// ============================================================================
// MAIN DI TEST
// ============================================================================
int main(void)
{
    // Test 1: Comportamento standard
    unsigned char arr1[] = {0xA, 0x5, 0x3, 0x7};
    printf("Test 1 (4 elementi): %#X (Atteso: 0X735A)\n", 
           pack_nibbles(arr1, sizeof(arr1)));

    // Test 2: Clamp branchless all'opera (Array di 10 elementi, ne prende 8)
    unsigned char arr2[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA};
    printf("Test 2 (>8 elementi, clamp branchless): %#X (Atteso: 0X87654321)\n", 
           pack_nibbles(arr2, sizeof(arr2)));

    // Test 3: Pulizia dati (valore 0xFF viene clampato al suo nibble 0x0F)
    unsigned char arr3[] = {0xFF, 0x00, 0xAA};
    printf("Test 3 (Valori sporchi): %#X (Atteso: 0X0A0F)\n", 
           pack_nibbles(arr3, sizeof(arr3)));

    return 0;
}