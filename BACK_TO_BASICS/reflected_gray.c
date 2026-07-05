#include <stdio.h>
#include <stdint.h>

/*
 * =====================================================================================
 * REFLECTED_GRAY.c - ENCODER OTTICO INDUSTRIALE BINDATO
 * =====================================================================================
 *
 * LOGICA DI PROTEZIONE DALLA METASTABILITÀ ESTERNA:
 * Se calcoliamo il codice Gray classico (n ^ (n >> 1)) direttamente su un intero a
 * 32-bit, l'eventuale presenza di bit accesi oltre la finestra stabilita dalla
 * variabile 'bits' andrebbe a inquinare il bit più significativo (MSB) della nostra
 * maschera a causa dello shift a destra.
 *
 * RISOLUZIONE:
 * Applichiamo una maschera preventiva (bitmask) per isolare la finestra del "disco
 * ottico" reale. Gestiamo esplicitamente l'eccezione logica dei 32-bit per evitare
 * l'Undefined Behavior (UB) dello shift hardware a 32 su registri a 32-bit.
 * =====================================================================================
 */

/**
 * @brief Genera il codice Gray riflesso confinato esattamente dentro la finestra dei bit richiesti.
 *
 * @param n Il valore numerico da convertire.
 * @param bits Il numero di bit attivi della traccia dell'encoder (da 1 a 32).
 * @return unsigned int Il codice Gray pulito, senza interferenze da bit fantasma superiori.
 */
unsigned int reflected_gray(uint32_t n, unsigned int bits)
{
    /*
     * GESTIONE UNDEFINED BEHAVIOR:
     * In C, fare (1 << 32) su un intero a 32 bit causa un comportamento imprevedibile
     * a livello CPU (molte architetture ignorano lo shift o fanno un wrapping a 0).
     * Se chiedono 32 bit, la maschera deve avere tutti i bit accesi (0xFFFFFFFF).
     */

    unsigned int mask = (bits >= 32) ? 0xFFFFFFFF : (1U << bits) - 1U;

    // Pialliamo via qualsiasi bit sporco oltre la finestra dell'encoder
    n &= mask;
    return n ^ (n >> 1);
}

int main(void)
{
    // Esempio critico: n ha il 5° bit acceso (16), ma vogliamo il calcolo solo su 4 bit.
    // Binario di n: 0001 1111 (ovvero 31)
    // Se pulito a 4 bit diventa: 1111 (ovvero 15)
    uint32_t n = 31;
    unsigned int bits = 4;
    unsigned int result;

    result = reflected_gray(n, bits);

    printf("Input originale (decimale): %u\n", n);
    printf("Finestra bit richiesta: %u\n", bits);
    printf("Risultato Gray Riflesso (decimale): %u\n", result);

    // Il Gray code di 15 (1111) è 10 (1010).
    // Se il codice funziona, deve stampare 10, ignorando il bit 5 che avrebbe prodotto 21.
    return (0);
}
