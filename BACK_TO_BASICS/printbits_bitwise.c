#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
//--PRINT BITS---------------------------------------------------------------------------

int main()
{
    unsigned char nove_bit = 56;
    for (int i = 7; i >= 0; i--)
    {
        unsigned char bit = ((nove_bit >> i) & 1) + 48;
        write(1, &bit, 1);
    }
    printf("\n");
    // PRINTBITS - versione parallela senza loop
    // idea: invece di estrarre un bit alla volta, prepariamo tutti e 8 i bit
    // contemporaneamente dentro un uint64_t (8 byte = 64 bit)
    // ogni byte del uint64_t conterra' esattamente '0' o '1' ASCII
    // alla fine stampiamo tutto con una write sola
    // =========================================================================

    // --- PASSO 1: copia nove in tutti e 8 i byte ----------------------------
    // 0x0101010101010101 in binario e' 00000001 ripetuto 8 volte
    // moltiplicare per questo numero e' come sommare nove shiftato di
    // 0, 8, 16, 24, 32, 40, 48, 56 posizioni → ogni byte diventa nove
    //
    // risultato (ogni riga e' un byte):
    // 00001001
    // 00001001
    // 00001001
    // 00001001
    // 00001001
    // 00001001
    // 00001001
    // 00001001
    uint64_t t = nove_bit * 0x0101010101010101ULL;

    // --- PASSO 2: maschera diagonale ----------------------------------------
    // 0x8040201008040201 e' una matrice diagonale di bit:
    // byte7: 10000000  ← seleziona solo il bit 7
    // byte6: 01000000  ← seleziona solo il bit 6
    // byte5: 00100000  ← seleziona solo il bit 5
    // byte4: 00010000  ← seleziona solo il bit 4
    // byte3: 00001000  ← seleziona solo il bit 3
    // byte2: 00000100  ← seleziona solo il bit 2
    // byte1: 00000010  ← seleziona solo il bit 1
    // byte0: 00000001  ← seleziona solo il bit 0
    //
    // dopo il &, ogni byte contiene UN SOLO BIT di nove, ma in posizioni
    // diverse: il byte7 ha il suo bit in pos 7, il byte3 in pos 3, ecc.
    // ogni byte vale 0x00 (bit era 0) oppure il suo "peso diagonale"
    // (0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01)
    t &= 0x8040201008040201ULL;

    // --- PASSO 3: normalizza tutti i byte a 0 o 1 ---------------------------
    // problema: ogni byte non-zero ha il bit in una posizione diversa.
    // dobbiamo portarli tutti in posizione 0.
    //
    // il trucco: aggiungere 0x7f (= 01111111) a ogni byte
    // - se il byte e' 0x00: 0x00 + 0x7f = 0x7f = 01111111 → bit7 rimane 0
    // - se il byte e' non-zero (anche solo 0x01): 0x01 + 0x7f = 0x80 = 10000000 → bit7 diventa 1
    // - se il byte e' 0x80: 0x80 + 0x7f = 0xff = 11111111 → bit7 rimane 1
    //
    // 0x7f e' la soglia esatta: e' il numero piu' grande che puoi aggiungere
    // a un byte non-zero garantendo SEMPRE overflow nel bit7,
    // mentre aggiunto a zero non produce overflow.
    // e' come un barattolo che contiene 127 biglie su 128: anche aggiungerne
    // una sola lo fa traboccare. se e' vuoto, non trabocca.
    //
    // dopo += 0x7f..., tutti i byte hanno l'informazione nello stesso posto: il bit7
    // bit7 = 1 significa "il bit originale di nove era 1"
    // bit7 = 0 significa "il bit originale di nove era 0"
    t += 0x7f7f7f7f7f7f7f7fULL;

    // ora shiftiamo il bit7 di ogni byte in posizione 0
    // >>= 7 sposta tutto il numero a 64 bit di 7 posizioni a destra
    // il bit7 di ogni byte scende in posizione 0 dello stesso byte
    // (ci puo' essere un po' di spazzatura negli altri bit del byte,
    //  per questo nel passo successivo usiamo un & per pulire)
    t >>= 7;

    // pulizia: teniamo solo il bit0 di ogni byte, azzeriamo tutto il resto
    // 0x0101... = 00000001 ripetuto 8 volte → maschera che tiene solo il bit0
    // dopo questo ogni byte vale esattamente 0x00 oppure 0x01
    t &= 0x0101010101010101ULL;

    // --- PASSO 4: converti in ASCII -----------------------------------------
    // 0x30 = 48 = '0' in ASCII
    // aggiungendo 0x30 a ogni byte: 0x00 → '0',  0x01 → '1'
    // 0x3030303030303030 e' semplicemente 0x30 ripetuto in tutti e 8 i byte
    t += 0x3030303030303030ULL;

    // --- PASSO 5: inverti i byte per l'endianness ---------------------------
    // su x86 la RAM e' little-endian: il byte meno significativo sta
    // all'indirizzo piu' basso. write() scrive dalla RAM in ordine di indirizzo,
    // quindi senza inversione stamperebbe i bit al contrario (da bit0 a bit7).
    // __builtin_bswap64 inverte l'ordine dei byte in una singola istruzione
    // CPU (istruzione assembly "bswap"), senza loop, senza perdere efficienza.
    t = __builtin_bswap64(t);

    // stampiamo tutti e 8 i byte in una volta sola
    write(1, &t, 8);
    write(1, "\n", 1);

    return 0;
}