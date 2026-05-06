#include <stdio.h>
#include "./utf8_decode.c"
// Dato un puntatore a una stringa UTF-8, decodifica il carattere corrente, avanza il puntatore al carattere successivo, e ritorna il codepoint.
// È il pattern usato da tutti i parser UTF-8.
unsigned int next_utf8_char(unsigned char **s)
{
    unsigned int codepoint;
    *s += utf8_decode(*s, &codepoint);
    return codepoint;
}

// int main()
// {
//     unsigned char *str = "A€🔥";
//     unsigned int cp;

//     while (*str)
//     {
//         cp = next_utf8_char(&str); // Passiamo l'indirizzo del puntatore!
//         printf("Codepoint: U+%X, Prossimo byte inizia a: %p\n", cp, str);
//     }
// }