/*
 * =====================================================================================
 * PARALLEL_PREFIX_AND - PROPAGAZIONE DELLO ZERO (Duale dell'OR)
 * =====================================================================================
 *
 * BACKGROUND TEORICO:
 * Il problema è propagare il "segnale di zero" verso destra. In logica booleana,
 * lo zero è l'elemento assorbente dell'operazione AND. Tuttavia, in C, lo shift
 * logico (>>) introduce zeri da sinistra, che sono distruttivi per l'AND.
 * Per aggirare questo limite hardware, applichiamo le Leggi di De Morgan:
 * NOT(A AND B) = NOT(A) OR NOT(B)
 * Trasformiamo il registro tramite NOT, propaghiamo gli 1 (che sono neutri per l'OR),
 * e ri-invertiamo tutto.
 *
 * TECNICA (Recursive Doubling):
 * L'algoritmo scala in O(log N). Ogni riga di shift raddoppia la portata del 
 * segnale: 1, 2, 4, 8, 16 bit. È il metodo più veloce ed efficiente (branchless).
 *
 * APPLICAZIONI:
 * - Generazione di maschere dinamiche per bit-field.
 * - Sistemi di gestione interrupt/reset a cascata in hardware.
 * - Algoritmi di compressione dati e bit-stuffing.
 *
 * IMPLEMENTAZIONE HARDWARE:
 * In un ASIC, questa logica viene sintetizzata come una rete combinatoria
 * a albero, che propaga il segnale in un singolo ciclo di clock (latenza minima).
 * =====================================================================================
 */

#include <stdio.h>

unsigned int parallel_prefix_and(unsigned int n)
{
    // Complemento: trasformo gli 0 in 1 (dominio OR)
    n = ~n; 
    
    // Propagazione parallela (Recursive Doubling)
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n |= (n >> 8);
    n |= (n >> 16);
    
    // Complemento finale: ritorno nel dominio AND
    return ~n;
}

int main(void)
{
    /* Test: 0xFF00FFF0
     * Il primo zero a sinistra (posizione 23) deve propagarsi 
     * fino a piallare tutti i bit successivi.
     */
    unsigned int input = 0xFF00FFF0;
    unsigned int output = parallel_prefix_and(input);
    
    printf("Input:  0x%08X\n", input);
    printf("Output: 0x%08X\n", output);
    
    return 0;
}