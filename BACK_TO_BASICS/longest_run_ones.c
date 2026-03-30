#include <stdio.h>
// ritorna la lunghezza della sequenza piu' lunga di bit accesi consecutivi,usata in compressione dati(LZ77,Deflate) e pattern matching
unsigned char longest_run_ones(unsigned char n)
{
    unsigned char count = 0;
    unsigned char max = 0;
    for (int i = 0; i <= 7; i++)
    {
        if (n >> i & 1)
        {
            count++;
            if(max < count)
                max = count;
        }
        else
            count = 0;
    }
    return max;
}
//oppure in maniera piu' efficiente usando il trick di Kernighan: itero su n finche' c'e' un bit acceso 
// e con n&= n << 1 accorcio le sequenze accese di un bit contemporaneamente,il che significa che non itero
//per forza 8 volte come sopra,ma solo tante volte quante e' lunga la sequenza piu' lunga, quindi banalmente su
//00000001 non ha bisogno di iterare 8 volte,ma 1
// unsigned char longest_run_ones(unsigned char n) {
//     unsigned char max = 0, count = 0;
//     while (n) {
//         n &= n << 1;  // spegne il bit più basso di ogni sequenza
//         count++;
//         if (count > max) max = count;
//     }
//     return max;
// }
int main()
{
    printf("%d\n", longest_run_ones(0b11101110));
    printf("%d\n", longest_run_ones(0b11111111));
    printf("%d\n", longest_run_ones(0b10101010));
    printf("%d\n", longest_run_ones(0b00011100));
    printf("%d\n", longest_run_ones(0b10110101));
}