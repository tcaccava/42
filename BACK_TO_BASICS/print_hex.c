#include <unistd.h>
// stampa un dump in formato hex,replicando in sostanza quanto fa il comando xxd da tarminale
void ft_print_hex(unsigned char *data, size_t n)
{
    unsigned char hex[] = "0123456789abcdef";
    while (n--)
    {
        char tmp[3];
        tmp[0] = hex[*data >> 4];   // isolo il nibble alto
        tmp[1] = hex[*data & 0x0F]; // isolo il nibble basso
        tmp[2] = 32;                // spazio tra coppie di nibbles
        write(1, tmp, 3);           // usando un array di char riduco il numero di syscall quando chiamo write
        data++;
    }
    write(1, "\n", 1);
}
// versione ultra ottimizzata con un unica chiamata di sistema di stampare tutto
//  void ft_print_hex(unsigned char *data, size_t n)
//  {
//      unsigned char hex[] = "0123456789abcdef";
//      // Ogni byte occupa 3 caratteri (due hex + spazio), più il newline finale
//      char out[n * 3 + 1];
//      size_t i = 0;
//      size_t j = 0;

//     while (i < n)
//     {
//         out[j++] = hex[data[i] >> 4];   // Nibble alto
//         out[j++] = hex[data[i] & 0x0F]; // Nibble basso
//         out[j++] = ' ';                 // Spazio
//         i++;
//     }
//     out[j] = '\n'; // newline alla fine del buffer

//     // Un'unica chiamata di sistema per stampare tutto
//     write(1, out, j + 1);
// }

// un'alternativa all'uso della lookup table e'usare logica binaria pura: per entrambi i nibble,se superano 9 aggiungo 87 per
// arrivare al carattere a,altrimenti 48,per arrivare a '0'
//  char c = nibble + (nibble > 9 ? 87 : 48)
int main()
{
    unsigned char data[] = "abc";
    ft_print_hex(data, 3);
}
