/*
 * =====================================================================================
 * BINARY_TO_BCD.c
 * =====================================================================================
 * 
 * TEORIA E FONDAMENTI:
 * Questa funzione esegue l'operazione inversa: converte un valore binario puro 
 * in formato BCD. Poiché un singolo byte può contenere al massimo 99 in BCD 
 * (0x99), è necessario un controllo di overflow.
 * 
 * LOGICA MATEMATICA:
 * 1. (n / 10): Ottiene la cifra delle decine.
 * 2. << 4: Sposta la cifra delle decine nel nibble alto (bits 7-4).
 * 3. (n % 10): Ottiene la cifra delle unità (il resto della divisione per 10).
 * 4. | (OR): Combina le decine (spostate) e le unità nel byte finale.
 * 
 * NOTE IMPLEMENTATIVE:
 * L'utilizzo di divisione (/) e modulo (%) è standard in C. Tuttavia, in contesti
 * hardware embedded (senza un divisore hardware dedicato), queste operazioni sono 
 * costose in termini di cicli CPU. In tali casi, si preferiscono implementazioni 
 * basate su shift e sottrazioni (come l'algoritmo Double Dabble).
 * =====================================================================================
 */

unsigned char binary_to_bcd(unsigned char n) 
{
    // Verifica di overflow: il formato BCD a singolo byte supporta solo fino a 99
    if (n > 99)
        return 255;
    
    // Converte e combina decine e unità
    return (n / 10 << 4) | (n % 10);
}