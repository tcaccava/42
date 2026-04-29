#include <stdio.h>
#include <stdint.h>
// Dato un buffer di byte, conta quanti bit sono accesi in un range di posizioni bit specifico.
// Utile in bitmap index dei database — rappresenti milioni di record come bit e fai query contando i bit accesi in range.
int bit_count_range(unsigned char *data, size_t n, int from, int to)
{
    // Controlli di sicurezza
    if (!data || n == 0 || from > to || from < 0)
        return 0;
    if (to >= (int)(n * 8))
        to = (n * 8) - 1; // Non andare oltre la fine del buffer

    int count = 0;
    for (int i = from; i <= to; i++)
    {
        int byte_idx = i / 8;
        int bit_idx = i % 8;
        if ((data[byte_idx] >> bit_idx) & 1)
            count++;
    }
    return count;
}
// oppure versione SWAR 64 pura
// int ft_bit_count_range(unsigned char *data, size_t n, int from, int to)
// {
//     int count = 0;
//     int cur = from;// bit di partenza

//     // --- TESTA ---
//     // Vado avanti un bit alla volta finché non sei su un byte allineato a 8 E sono all'inizio del byte(cur % 8 == 0)
//     while (cur <= to && ((uintptr_t)(data + (cur / 8)) & 7 || cur % 8 != 0))// il ciclo while continua ad avanzare bit per bit finche ENTRAMBE le condizioni non diventano false
//     {
//         if ((data[cur / 8] >> (cur % 8)) & 1)
//             count++;
//         cur++;
//     }

//     // --- CORPO ---
//     // Prendo 8 byte (64 bit) alla volta e uso il popcount
//     uint64_t *ptr = (uint64_t *)(data + (cur / 8));
//     while (cur + 63 <= to)
//     {
//         count += popcount_swar64(*ptr); // Somma 64 bit in un colpo
//         ptr++;        // Sposta il puntatore di 8 byte
//         cur += 64;    // Salta avanti di 64 bit nel nastro
//     }

//     // --- CODA ---
//     // Finisci gli ultimi bit rimasti
//     while (cur <= to)
//     {
//         if ((data[cur / 8] >> (cur % 8)) & 1)
//             count++;
//         cur++;
//     }
//     return count;
// }

int main()
{
    unsigned char data[] = {0b10011100, 0b01011110, 0b10000110};
    printf("%d %d\n", bit_count_range(data, 3, 3, 17), bit_count_range(data, 3, 5, 21));
}
