/*
 * =====================================================================================
 * BINARY_TO_BCD_FAST.c - ALGORITMO DOUBLE DABBLE
 * =====================================================================================
 * 
 * 1. IL PROBLEMA:
 * In molti microcontrollori a 8-bit (o in logica pura FPGA), le operazioni di divisione 
 * (/) e modulo (%) sono estremamente onerose. Richiedono decine o centinaia di cicli 
 * di clock, consumando energia e bloccando la pipeline della CPU.
 * 
 * 2. LA SOLUZIONE (DOUBLE DABBLE):
 * L'algoritmo Double Dabble (o Shift-and-Add-3) permette di eseguire la conversione 
 * in un numero fisso di passi (O(8) per un byte), utilizzando solo operazioni logiche 
 * bitwise (SHIFT) e addizioni elementari.
 * 
 * 3. PERCHÉ IL +3? (LA TEORIA DEL CORRETTORE):
 * Quando shiftiamo a sinistra, stiamo moltiplicando per 2. In binario, il riporto 
 * avviene a 16 (0x10). In BCD, il riporto deve avvenire a 10.
 * La differenza tra 16 e 10 è 6.
 * Sommare 3 *prima* dello shift equivale a sommare 6 *dopo* lo shift (3 * 2 = 6).
 * Questo "trucco" matematico corregge automaticamente il valore del nibble, 
 * forzando il riporto nel nibble superiore esattamente quando il valore 
 * decimale supera 9.
 * =====================================================================================
 */

#include <stdio.h>

/**
 * @brief Converte un numero binario (0-99) in BCD usando l'algoritmo Double Dabble.
 * @param n Il numero binario da convertire.
 * @return Il valore in formato BCD (es. 23 -> 0x23).
 */
unsigned char binary_to_bcd_fast(unsigned char n)
{
    unsigned char bcd = 0;
    
    // Per un byte (8 bit), eseguiamo 8 iterazioni
    for (int i = 0; i < 8; i++) {
        
        // CORREZIONE DELLE UNITÀ:
        // Se il nibble basso (unità) è >= 5, aggiungiamo 3 prima dello shift.
        // Questo forza il riporto verso il nibble alto alla prossima iterazione.
        if ((bcd & 0x0F) >= 5)
            bcd += 0x03;
        
        // CORREZIONE DELLE DECINE:
        // Se il nibble alto (decine) è >= 5, aggiungiamo 0x30 (ovvero 3 nel nibble alto).
        // Questo gestisce il superamento della soglia decimale per le decine.
        if ((bcd & 0xF0) >= 0x50)
            bcd += 0x30;
        
        // SHIFT E CARICAMENTO:
        // 1. Shifta tutto il registro BCD a sinistra di 1.
        // 2. Preleva il bit più significativo (MSB) dal numero originale 'n'.
        // 3. Inserisci quel bit nella posizione meno significativa di 'bcd'.
        bcd = (bcd << 1) | ((n >> 7) & 1);
        
        // Shifta anche 'n' per processare il prossimo bit
        n <<= 1;
    }
    
    return bcd;
}

int main(void)
{
    unsigned char val = 99;
    unsigned char bcd = binary_to_bcd_fast(val);
    
    // Output atteso: 0x99
    printf("Input: %d | BCD esadecimale: 0x%02X\n", val, bcd);
    
    return 0;
}