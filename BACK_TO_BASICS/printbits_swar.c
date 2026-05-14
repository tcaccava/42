#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
void print_bits32(unsigned int n)
{
    unsigned char printed0 = n & 0x000000FF;              // isolo il byte 0
    unsigned char printed1 = n >> 8 & 0x000000FF;         // byte 1
    unsigned char printed2 = n >> 16 & 0x000000FF;        // byte 2
    unsigned char printed3 = n >> 24 & 0x000000FF;        // byte 3
    uint64_t printed0_64 = printed0 * 0x0101010101010101; // replico il byte 0 in un intero a 64 bit
    printed0_64 &= 0x8040201008040201ULL;                 // uso una maschera diagonale per isolare i bit 7,6,5,4,3,2,1,0 nei byte corrispondenti
    // problema: ogni byte non-zero adesso ha il bit in una posizione diversa
    // aggiungo 0x7f(127) ad ogni byte: quel numero e' il numero piu' grande che garantisce per tutti i byte diversi da 0
    // l'overflow del bit 7, mentre aggiunto a 0 quel bit rimane spento
    // dopo questa aggiunta i byte dell'uint64 contengono tutti l'informazione originaria nello stesso posto,il bit 7
    printed0_64 += 0x7f7f7f7f7f7f7f7fULL; //
    printed0_64 >>= 7;                    // porto i bit significativi in posizione 0 di ogni byte
    printed0_64 &= 0x0101010101010101ULL; // isolo i bit significativi
    printed0_64 += 0x3030303030303030ULL; // aggiungo 48 per convertire quei bit in char,cosi che siano pronti ad essere stampati
    // ripeto lo stesso trattamento per l'intero numero a 32 bit
    uint64_t printed1_64 = printed1 * 0x0101010101010101;
    printed1_64 &= 0x8040201008040201ULL;
    printed1_64 += 0x7f7f7f7f7f7f7f7fULL;
    printed1_64 >>= 7;
    printed1_64 &= 0x0101010101010101ULL;
    printed1_64 += 0x3030303030303030ULL;
    uint64_t printed2_64 = printed2 * 0x0101010101010101;
    printed2_64 &= 0x8040201008040201ULL;
    printed2_64 += 0x7f7f7f7f7f7f7f7fULL;
    printed2_64 >>= 7;
    printed2_64 &= 0x0101010101010101ULL;
    printed2_64 += 0x3030303030303030ULL;
    uint64_t printed3_64 = printed3 * 0x0101010101010101;
    printed3_64 &= 0x8040201008040201ULL;
    printed3_64 += 0x7f7f7f7f7f7f7f7fULL;
    printed3_64 >>= 7;
    printed3_64 &= 0x0101010101010101ULL;
    printed3_64 += 0x3030303030303030ULL;
    // su x86 la RAM e' little-endian: il byte meno significativo sta
    // all'indirizzo piu' basso. write() scrive dalla RAM in ordine di indirizzo,
    // quindi senza inversione stamperebbe i bit al contrario (da bit0 a bit7).
    // __builtin_bswap64 inverte l'ordine dei byte in una singola istruzione
    printed0_64 = __builtin_bswap64(printed0_64);
    printed1_64 = __builtin_bswap64(printed1_64);
    printed2_64 = __builtin_bswap64(printed2_64);
    printed3_64 = __builtin_bswap64(printed3_64);
    write(1, &printed3_64, 8);
    write(1, &printed2_64, 8);
    write(1, &printed1_64, 8);
    write(1, &printed0_64, 8);
    write(1, "\n", 1);
}

int main()
{
    print_bits32(0xFF00FF00);
}