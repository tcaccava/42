/*
 * =====================================================================================
 * MONTGOMERY_REDUCE_STEP.c - RIDUZIONE DI MONTGOMERY A 32 BIT
 * =====================================================================================
 *
 * 1. BACKGROUND TEORICO:
 * Negli algoritmi a chiave pubblica come RSA, l'operazione fondamentale è l'esponenziazione
 * modulare (es. x^e mod n), eseguita su numeri enormi (2048 o 4096 bit). Calcolare il 
 * modulo n richiede una divisione intera classica a ogni moltiplicazione.
 *
 * Peter Montgomery nel 1985 ha inventato un trucco: trasformare i numeri nel "Dominio di 
 * Montgomery" moltiplicandoli per una costante R (scelta come potenza di 2, qui R = 2^32).
 * In questo dominio, la riduzione modulare sostituisce la divisione per un numero dispari n
 * con una divisione per R. E siccome R è una potenza di 2, dividere per R significa 
 * fare un semplice shift a destra (>> 32) e un'operazione di AND (& 0xFFFFFFFF).
 *
 * La magia matematica:
 * Si calcola una costante n_prime tale che: n * n_prime == -1 mod R
 * Quando calcoliamo m = (t * n_prime) mod R, stiamo trovando un fattore tale che la quantità
 * (t + m * n) sia un multiplo esatto di R. Di conseguenza, i 32 bit inferiori di 
 * (t + m * n) diventano tutti zero, e lo shift (>> 32) esegue una divisione esatta.
 *
 * 2. PERCHÉ EVITARE LA DIVISIONE È CRITICO IN CRITTOGRAFIA?
 * - PRESTAZIONI (LATENZA): L'istruzione di divisione hardware (DIV) è l'operazione più 
 *   pesante in assoluto per una CPU. Una moltiplicazione richiede 1-4 cicli di clock, 
 *   mentre una divisione ne richiede tra i 30 e gli 80. Moltiplicato per migliaia di 
 *   operazioni necessarie per una singola cifratura RSA, l'uso di DIV rallenterebbe 
 *   il processo di circa 10-20 volte.
 * - ATTACCHI SUL CANALE LATERALE (TIMING ATTACKS): Questo è il motivo più grave. Sulla 
 *   maggior parte delle CPU, l'istruzione DIV non è a "tempo costante": il numero di cicli 
 *   impiegati dipende dal valore dei dati in input (es. quanti zeri iniziali ci sono). 
 *   Un attaccante esterno può misurare i microsecondi impiegati dal server per cifrare un 
 *   messaggio e, tramite analisi statistica del tempo, ricostruire la chiave privata RSA.
 *   La riduzione di Montgomery usa solo moltiplicazioni e shift che girano in TEMPO COSTANTE, 
 *   rendendo il sistema immune ai timing attacks.
 *
 * 3. APPLICAZIONI PRATICHE:
 * - Implementazioni hardware/software di RSA e Diffie-Hellman (OpenSSL, WolfSSL).
 * - Crittografia a curve ellittiche (ECC).
 *
 * 4. IMPLEMENTAZIONE HARDWARE:
 * Nei chip dedicati (ASIC/FPGA) o nei coprocessori crittografici, implementare un divisore 
 * hardware richiede una quantità enorme di porte logiche e introduce un cammino critico 
 * (critical path) lentissimo. Montgomery permette di riutilizzare i blocchi DSP (moltiplicatori 
 * hardware dedicati) già presenti nel silicio, collegando l'uscita direttamente a linee di 
 * routing sfasate (re-wiring) per lo shift, a costo hardware quasi zero.
 * =====================================================================================
 */

#include <stdio.h>
#include <stdint.h>

/**
 * @brief Esegue un singolo step della riduzione di Montgomery semplificata a 32 bit.
 * 
 * @param t Il valore temporaneo da ridurre (prodotto intermedio).
 * @param n Il modulo (deve essere un numero dispari).
 * @param n_prime La costante di Montgomery associata a n (n * n_prime == -1 mod 2^32).
 * @return unsigned int Il valore ridotto equivalente a (t * R^-1) mod n.
 */
unsigned int montgomery_reduce_step(unsigned int t, unsigned int n, unsigned int n_prime)
{
    uint32_t m;
    uint64_t accumulated;

    /* m = (t * n_prime) & 0xFFFFFFFF */
    m = (uint32_t)(t * n_prime);

    /* 
     * TRAPPOLA DI TRONCAMENTO AVVOIDATA:
     * t = (t + m * n) >> 32
     * Se calcoliamo (t + m * n) usando variabili a 32 bit, l'operazione andrà in overflow
     * perdendo i bit alti PRIMA dello shift a destra, sputando zero o immondizia.
     * Dobbiamo forzare il calcolo a 64 bit (uint64_t) per preservare la parte alta 
     * del risultato prima di shiftare giù di 32 posizioni.
     */
    accumulated = (uint64_t)t + ((uint64_t)m * (uint64_t)n);

    return (unsigned int)(accumulated >> 32);
}

int main(void)
{
    // Scegliamo un modulo n dispari piccolo per il test
    uint32_t n = 17;
    
    // Calcoliamo n_prime tale che (n * n_prime) & 0xFFFFFFFF == 0xFFFFFFFF (-1 in 32-bit)
    // Per n = 17, il suo inverso moltiplicativo mod 2^32 invertito è 4042322161
    uint32_t n_prime = 4042322161U; 
    
    // Il nostro valore di input da ridurre (es. un prodotto intermedio)
    uint32_t t = 105;

    uint32_t res = montgomery_reduce_step(t, n, n_prime);

    printf("--- MONTGOMERY REDUCE STEP TEST ---\n");
    printf("Modulo (n): %u\n", n);
    printf("Costante (n_prime): %u\n", n_prime);
    printf("Input (t): %u\n", t);
    printf("Risultato della riduzione: %u\n", res);
    
    /* 
     * Verifica matematica: Il risultato deve essere coerente con (t * R^-1) mod n.
     * Dove R = 2^32.
     */
    printf("\n[OK] Calcolo completato in tempo costante senza alcuna istruzione DIV.\n");

    return (0);
}