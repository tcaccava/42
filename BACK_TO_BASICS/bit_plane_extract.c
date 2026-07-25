#include <stdio.h>

// Dato un array di n byte, estrae il bit numero plane (0-7) da ogni byte e lo impacchetta in out come bitmap.
// Esempio: array {0xAA, 0x55} = {10101010, 01010101}, plane=7 estrae i bit 7 = {1, 0} → out[0] = 0b10000000.
// Usato nella compressione di immagini bitmap (BMP raw, fax Group 4) e nell'analisi delle texture nelle GPU.
void bit_plane_extract(unsigned char *bytes, int n, int plane, unsigned char *out)
{
    plane &= 7;
    for (int i = 0,o = 0, j = 7; i < n; i++,o++,j--)
    {
        unsigned char bit = (bytes[i] & (1U << plane));
        out[i >> 3] |= bit << ((i + j) & 7);
    }
}

int main() {

}