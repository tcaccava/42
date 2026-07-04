/*
 * =====================================================================================
 * BIT_INTERLEAVE_3WAY.c - MORTON CODE 3D (Z-ORDER CURVE)
 * =====================================================================================
 * 
 * BACKGROUND TEORICO:
 * L'interleaving dei bit a 3 vie serve a calcolare il "Morton Code" (o Z-order curve) 
 * in 3 dimensioni. È una funzione hash spaziale che mappa coordinate 3D (x, y, z) 
 * in un singolo numero scalare 1D, preservando la località spaziale (punti vicini 
 * nello spazio 3D avranno Morton code vicini).
 * 
 * LOGICA DI DILATAZIONE (IL METODO MAGIC BITS):
 * Invece di usare un lento ciclo for per spostare i bit uno ad uno, sfruttiamo 
 * il parallelismo bitwise. Prendiamo i nostri 8 bit compatti e li "frantumiamo" 
 * in posizioni specifiche, lasciando 2 zeri di spazio tra ogni bit utile.
 * 
 * PROGRESSIONE DELLA MASCHERA:
 * 1. Dividiamo in 2 blocchi da 4 bit. Shift 8. 
 *    Mask: 0x0000F00F (00000000 00000000 11110000 00001111)
 * 2. Dividiamo in 4 blocchi da 2 bit. Shift 4.
 *    Mask: 0x000C30C3 (00000000 00001100 00110000 11000011)
 * 3. Isliamo i singoli bit distanziati di due zeri. Shift 2.
 *    Mask: 0x00249249 (00000000 00100100 10010010 01001001)
 * 
 * abcdefgh
 * 00000000abcd00000000efgh
 * 0000ab0000cd0000ef0000gh
 * 00a00b00c00d00e00f00g00h 
 * 
 * APPLICAZIONI PRATICHE:
 * - Motori Grafici 3D e Raytracing: Strutture dati spaziali come gli Octree usano 
 *   questa tecnica per cercare collisioni velocemente.
 * - Database spaziali e voxel engines (es. Minecraft).
 * =====================================================================================
 */

#include <stdio.h>

/**
 * @brief Intreccia i bit di tre byte in un singolo intero a 32-bit.
 * L'ordine finale è a0, b0, c0, a1, b1, c1, ecc.
 */
unsigned int bit_interleave_3way(unsigned char a, unsigned char b, unsigned char c)
{
    unsigned int ex_a = a;
    unsigned int ex_b = b;
    unsigned int ex_c = c;

    // STEP 1: Shift di 8
    ex_a = (ex_a | (ex_a << 8)) & 0x0000F00F;
    ex_b = (ex_b | (ex_b << 8)) & 0x0000F00F;
    ex_c = (ex_c | (ex_c << 8)) & 0x0000F00F;

    // STEP 2: Shift di 4
    ex_a = (ex_a | (ex_a << 4)) & 0x000C30C3;
    ex_b = (ex_b | (ex_b << 4)) & 0x000C30C3;
    ex_c = (ex_c | (ex_c << 4)) & 0x000C30C3;

    // STEP 3: Shift di 2
    ex_a = (ex_a | (ex_a << 2)) & 0x00249249;
    ex_b = (ex_b | (ex_b << 2)) & 0x00249249;
    ex_c = (ex_c | (ex_c << 2)) & 0x00249249;

    /*
     * MERGE FINALE:
     * ex_a ha i bit alle posizioni 0, 3, 6, 9...
     * ex_b viene shiftato a sx di 1 -> posizioni 1, 4, 7, 10...
     * ex_c viene shiftato a sx di 2 -> posizioni 2, 5, 8, 11...
     * L'OR finale incastra tutto perfettamente senza collisioni.
     */
    return (ex_a | (ex_b << 1) | (ex_c << 2));
}