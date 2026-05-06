#include <stdio.h>
#include "./utf8_charlen.c"
// restituisce il numero di caratteri utf-8 in una stringa,non il numero di bytes.

int utf8_strlen(unsigned char *s)
{
    int len = 0;
    while (*s)
    {
        len++;
        s += (utf8_charlen(*s));
    }
    return len;
}
// oppure bitwise pura,sfruttando il fatto che in utf-8 tutti i byte di continuazione iniziano con 10
// int ft_utf8_strlen(unsigned char *s)
// {
//     int len = 0;
//     while (*s)
//     {
//         // Contiamo il byte solo se NON inizia con 10 (0x80 in bitwise)
//         // In UTF-8, i byte che iniziano con 10 sono i pezzi "di mezzo" di un carattere
//         if ((*s & 0xC0) != 0x80)
//             len++;
//         s++;
//     }
//     return len;
// }
int main() {
    char *norm_string = "Hello";
    char *utf8_string = "Héllo";// questa ha sempre lunghezza 5 ma occupa 6 bytes
    printf("%d %d\n", utf8_strlen(norm_string),utf8_strlen(utf8_string));
}