#include <stdio.h>
#include "./utf8_charlen.c"
// Legge il primo byte dal buffer bytes,lo decodifica in codepoint e ritorna il numero di bytes decodificati
// cosi da poterla inserire in un ciclo while in cui aggiorna la posizione del puntatore bytes per leggere l'intero buffer
int utf8_decode(unsigned char *bytes, unsigned int *codepoint)
{
    int charlen = utf8_charlen(*bytes);
    *codepoint = 0;
    if (charlen == 1)
    {
        *codepoint = (unsigned char)*bytes;
        return 1;
    }
    if (charlen == 2)
    {
        *codepoint |= (bytes[0] & 0b00011111);
        *codepoint <<= 6;
        *codepoint |= (bytes[1] & 0b00111111);
        return 2;
    }
    if (charlen == 3)
    {
        *codepoint |= (bytes[0] & 0b00001111);
        *codepoint <<= 6;
        *codepoint |= (bytes[1] & 0b00111111);
        *codepoint <<= 6;
        *codepoint |= (bytes[2] & 0b00111111);
        return 3;
    }
    if (charlen == 4)
    {
        *codepoint |= (bytes[0] & 0b00000111);
        *codepoint <<= 6;
        *codepoint |= (bytes[1] & 0b00111111);
        *codepoint <<= 6;
        *codepoint |= (bytes[2] & 0b00111111);
        *codepoint <<= 6;
        *codepoint |= (bytes[3] & 0b00111111);
        return 4;
    }
    return 0;
}

// int main()
// {
//     // 1. Usiamo una stringa (array di byte) per contenere i simboli multi-byte
//     unsigned char *str = "Σ€🔥";
//     unsigned int codepoints[3];
//     unsigned char *ptr = str; // Puntatore mobile per scorrere la stringa

//     for (int i = 0; i < 3; i++)
//     {
//         // 2. Decodifichiamo un carattere alla volta
//         // Passiamo ptr (dove siamo arrivati) e l'indirizzo della cella i-esima
//         int len = utf8_decode(ptr, &codepoints[i]);

//         // 3. Spostiamo il puntatore avanti di 'len' byte
//         ptr += len;
//     }

//     // 4. Ora stampiamo i risultati
//     for (int i = 0; i < 3; i++)
//     {
//         printf("U+%X ", codepoints[i]); // U+3A3 U+20AC U+1F525,che sono proprio i codepoint unicode della lettera singma,dell'euro e della emoji del fuoco
//     }
//     printf("\n");
//     return 0;
// }