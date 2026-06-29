/*
 * =====================================================================================
 * BYTE_PARITY_VECTOR - SWAR PARALLEL PARITY (SIMD WITHIN A REGISTER)
 * =====================================================================================
 *
 * BACKGROUND TEORICO:
 * L'obiettivo è calcolare la parità (XOR di tutti i bit) di ogni byte (8 bit) 
 * in parallelo. La parità di un byte è 1 se il numero di bit a 1 è dispari, 0 altrimenti.
 * * SWAR (SIMD Within A Register): 
 * Il registro a 32 bit viene trattato come un vettore di 4 elementi da 8 bit. 
 * Per evitare che i dati di un byte "inquinino" quello adiacente, si usa il 
 * "fold logico": si dimezza lo spazio di lavoro a ogni passaggio, applicando una 
 * maschera per isolare i bit di parità parziali.
 * * TECNICA DI FOLDING:
 * 1. XOR con shift di 4: riduce 8 bit a 4 bit (nibble). Maschera 0x0F0F0F0F.
 * 2. XOR con shift di 2: riduce 4 bit a 2 bit. Maschera 0x03030303.
 * 3. XOR con shift di 1: riduce 2 bit a 1 bit (risultato finale). Maschera 0x01010101.
 * * APPLICAZIONI PRATICHE:
 * - Hamming Codes: Calcolo del checksum di parità in codici correttori d'errore (ECC).
 * - Sistemi di memoria RAM: Verifica dell'integrità dei dati su bus a 32 o 64 bit.
 * - Compressione dati: Algoritmi di streaming che richiedono validazione veloce.
 *
 * IMPLEMENTAZIONE HARDWARE:
 * In ambito FPGA, questa operazione viene implementata mediante una rete di 
 * alberi XOR (XOR-tree). Il segnale attraversa porte logiche in parallelo, 
 * riducendo la latenza a soli pochi cicli di gate logici.
 * =====================================================================================
 */

#include <stdio.h>

unsigned int byte_parity_vector(unsigned int n)
{
    // Fold da 8 a 4 bit
    n ^= (n >> 4);
    n &= 0x0F0F0F0F;
    
    // Fold da 4 a 2 bit
    n ^= (n >> 2);
    n &= 0x03030303;
    
    // Fold da 2 a 1 bit
    n ^= (n >> 1);
    n &= 0x01010101;
    
    return n; // return Un registro dove ogni byte contiene 1 (parità dispari) o 0 (parità pari) 
 
}

int main(void)
{
    // Test: 0x80010307
    // Byte 0 (0x07): 00000111 -> 3 bit a 1 -> Parità 1
    // Byte 1 (0x03): 00000011 -> 2 bit a 1 -> Parità 0
    // Byte 2 (0x01): 00000001 -> 1 bit a 1 -> Parità 1
    // Byte 3 (0x80): 10000000 -> 1 bit a 1 -> Parità 1
    // Risultato atteso: 0x01010001
    
    unsigned int input = 0x80010307;
    unsigned int output = byte_parity_vector(input);
    
    printf("Input:  0x%08X\n", input);
    printf("Output: 0x%08X\n", output);
    
    return 0;
}