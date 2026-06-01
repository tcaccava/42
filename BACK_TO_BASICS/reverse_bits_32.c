#include <stdio.h>
// Algoritmo SWAR (SIMD Within A Register) per l'inversione speculare di un intero a 32 bit.
//
// BACKGROUND TEORICO:
// Invece di scambiare un bit alla volta con un ciclo for da 32 iterazioni (approccio O(N) ingenuo),
// l'algoritmo applica il paradigma "Divide et Impera" in parallelo (O(log N)).
// Il registro viene trattato come un vettore SIMD virtuale. Ad ogni riga, la granularità raddoppia:
// prima si invertono i singoli bit adiacenti, poi le coppie, poi i quartetti (nibbles), i byte e infine le metà a 16 bit.
// Esegue esattamente 15 operazioni logiche fisse, totalmente branchless e instruction-independent nelle sotto-espressioni.
//
// IMPLEMENTAZIONE HARDWARE:
// - Architettura ARM: Non serve questo codice. ARM ha l'istruzione nativa 'RBIT' (Reverse Bits) scolpita nel silicio che fa tutto in 1 ciclo di clock.
// - Architettura x86 (Intel/AMD): Non esiste un'istruzione nativa per il bit-reversal totale (esiste solo 'BSWAP' che inverte l'ordine dei byte, non dei singoli bit).
//   Di conseguenza, questo specifico algoritmo SWAR è lo STATO DELL'ARTE ASSOLUTO su x86 ed è esattamente ciò che i compilatori moderni (GCC/Clang)
//   generano sotto il cofano quando ottimizzano pattern di inversione bit.
//
// APPLICAZIONI PRATICHE:
// - FFT (Fast Fourier Transform): Algoritmo di Cooley-Tukey (radix-2). Per riordinare i dati di input prima del calcolo delle frequenze (MP3, JPEG, elaborazione radar),
//   gli indici dell'array devono subire il "bit-reversal ordering" (es. l'elemento 001 va al posto di 100). Questo codice spara la FFT al massimo della velocità di cache.
// - Elaborazione Immagini e Grafica: Manipolazione di bitmap binarie o specchiatura di texture a basso livello senza toccare la VRAM.
unsigned int reverse_bits32(unsigned int n)
{
    n = ((n & 0xAAAAAAAA) >> 1) | ((n & 0x55555555) << 1);   // Scambia bit adiacenti (gruppi di 1)
    n = ((n & 0xCCCCCCCC) >> 2) | ((n & 0x33333333) << 2);   // Scambia coppie di bit (gruppi di 2)
    n = ((n & 0xF0F0F0F0) >> 4) | ((n & 0x00FF00FF) << 4);   // Scambia quartetti (gruppi di 4)
    n = ((n & 0xF0F0F0F0) >> 4) | ((n & 0x0F0F0F0F) << 4);   // Versione corretta della maschera dei nibbles
    n = ((n & 0xFF00FF00) >> 8) | ((n & 0x00FF00FF) << 8);   // Scambia i bytes adiacenti (gruppi di 8)
    n = ((n & 0xFFFF0000) >> 16) | ((n & 0x0000FFFF) << 16); // Scambia le due metà a 16 bit (halfword)
    return n;
}