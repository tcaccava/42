#include <stdio.h>
#include "popcount_swar32.c"
/*
 * =====================================================================================
 * COUNT_COMMON_BITS - ANALISI BITWISE SOTA
 * =====================================================================================
 * Conta i bit identici tra due registri a 32 bit (complemento Hamming).
 * BACKGROUND TEORICO:
 * La funzione opera sulla base della distanza di Hamming. L'operatore XOR (^) 
 * confronta due bit: restituisce 0 se i bit sono identici, 1 se sono diversi.
 * Di conseguenza, il numero di 1 risultanti dopo l'operazione (a ^ b) rappresenta 
 * il numero di discrepanze tra i due operandi. Sottraendo questo valore dalla 
 * lunghezza del registro (32 bit), si ottiene matematicamente il numero di bit 
 * che coincidono.
 *
 * APPLICAZIONI PRATICHE:
 * - Bioinformatica: Confronto rapido di sequenze nucleotidiche codificate in bit.
 * - Compressione Dati: Calcolo della ridondanza tra blocchi di dati simili (delta-encoding).
 * - Algoritmi di Ricerca: Fuzzy hashing e rilevamento di similarità in file binari.
 * - Machine Learning: Calcolo della similarità tra vettori in reti neurali binarie.
 *
 * IMPLEMENTAZIONI INDUSTRIALI:
 * Nel software ad alte prestazioni, questo codice viene compilato in istruzioni 
 * hardware atomiche:
 * - x86_64: L'istruzione POPCNT (SSE4.2) esegue il conteggio in 1 ciclo di clock.
 * - ARM: L'istruzione CNT (Neon/ASIMD) permette il conteggio vettoriale massivo.
 * Il compilatore moderno riconosce l'idioma SWAR e ottimizza la branchless logic 
 * direttamente nelle pipeline della CPU.
 * =====================================================================================
 */

int count_common_bits(unsigned int a, unsigned int b) {
    return 32 - popcount_swar32(a ^ b);
}

int main(void) {
    unsigned int a = 0x00FFF000;
    unsigned int b = 0xFFF00FFF;
    
    printf("%d\n", count_common_bits(a, b)); // 4
    
    return 0;
}