#include <stdio.h>
#include "./popcount.c"
#include "./popcount_swar32.c"

/**
 * ============================================================================
 * CONTEGGIO BRANCHLESS DELLE TRANSIZIONI DI BIT (EDGE DETECTION) IN C
 * ============================================================================
 * * 1. BACKGROUND : LA DERIVATA DISCRETA BOOLEANA
 * Rappresentando un intero come un vettore di bit [b_n, ..., b_1, b_0], il cambio
 * di stato (fronte di salita o di discesa) tra due bit adiacenti è definito
 * algebricamente nello Spazio di Galois (GF(2)) come:
 * * t_i = b_(i+1) ^ b_i
 * * L'operazione `n ^ (n << 1)` calcola simultaneamente questa derivata discreta
 * su ogni coppia di bit contigui dell'intero registro. Il numero totale di
 * transizioni coincide con la norma L1 (distanza di Hamming dallo zero) del
 * vettore delle derivate, calcolabile tramite Popcount (Population Count).
 * * Questa tecnica è il duale asimmetrico del Codice di Gray (che usa lo shift a
 * destra). Spostando lo shift a sinistra, l'effetto di sbrodolamento del bit
 * fantasma si concentra sul bit meno significativo (LSB), consentendo una
 * correzione branchless immediata.
 * * 2. MECCANICA DELLA CORREZIONE SULL'LSB
 * Lo XOR tra il numero e la sua traslazione a sinistra di 1 fa sì che ogni bit
 * del risultato contenga il valore della differenza tra ogni bit di 'n' e quello
 * immediatamente alla sua destra:
 * - Se il risultato dello XOR è 1: i bit adiacenti erano DIFFERENTI (transizione).
 * - Se il risultato dello XOR è 0: i bit adiacenti erano UGUALI (stasi).
 * * Poiché lo shift a sinistra introduce uno '0' artificiale in posizione zero,
 * il bit 0 del risultato conterrà lo XOR tra il bit 0 originale di 'n' e questo
 * zero introdotto. Di conseguenza, se 'n' è dispari (bit 0 = 1), lo XOR registrerà
 * una transizione fittizia "1 ^ 0 = 1" sul confine destro, anche se non c'è
 * alcuna transizione reale prima del bit zero.
 * L'espressione `-(n & 1)` sottrae esattamente 1 se il numero è dispari,
 * ripristinando il rigore matematico del conteggio.
 * * 3. APPLICAZIONI PRATICHE DEL TOGGLE RATE
 * - Compressione Dati (Run-Length Encoding - RLE): Determina l'entropia di un
 * blocco prima di elaborarlo. Strutture con transizioni prossime allo zero
 * indicano sequenze altamente comprimibili; valori vicini al limite teorico
 * (31 per i 32-bit) indicano rumore bianco o pattern alternati ad alta frequenza.
 * - Telecomunicazioni e Sincronizzazione: Algoritmi di clock recovery per
 * segnali seriali asincroni (es. codifica Manchester, UART) per stimare il
 * Baud Rate analizzando il toggle rate dei fronti d'onda hardware.
 * - Test di Entropia Statistica (NIST FIPS 140-2): Analisi in tempo reale della
 * qualità dei Generatori di Numeri Casuali Hardware (TRNG).
 * * 4. IMPLEMENTAZIONE HARDWARE (ASIC / FPGA)
 * In logica cablata, questa funzione ha costo computazionale quasi nullo:
 * - Lo shift `n << 1` non richiede porte logiche o cicli di clock (costo zero),
 * trattandosi di un semplice re-routing dei fili di rame sul silicio.
 * - Lo stadio di transizione istanzia N porte XOR in parallelo con un ritardo
 * di propagazione fisso pari a 1 Gate Delay (1 FO4).
 * - Il Popcount viene risolto da un albero combinatorio di sommatori (Wallace Tree),
 * dove la correzione `-(n & 1)` viene iniettata direttamente come Carry-In
 * negato nell'ultimo stadio dell'addizionatore.
 ============================================================================
 */

int count_bit_transitions(unsigned char n)
{
    return count_bits(n ^ (n << 1)) - (n & 1);
}

int count_bits_transitions32(unsigned int n)
{
    return popcount_swar32(n ^ (n << 1)) - (n & 1);
}
int main()
{
    printf("%d\n", count_bit_transitions(0b01100011));    // 3
    printf("%d\n", count_bit_transitions(0b01101011));    // 5
    printf("%d\n", count_bit_transitions(0b01001011));    // 5
    printf("%d\n", count_bit_transitions(0b01010101));    // 7
    printf("%d\n", count_bit_transitions(0b10101010));    // 7
    printf("%d\n", count_bit_transitions(0b11100010));    // 3
    printf("%d\n", count_bit_transitions(0b01000011));    // 3
    printf("%d\n", count_bit_transitions(0b11111111));    // 0
    printf("%d\n", count_bits_transitions32(0xAAAAAAAA)); // 31
}
