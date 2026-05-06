#include <stdio.h>
#include "./utf8_next_char.c"
#include "./utf8_encode.c"
// converte una stringa utf-8 in maiuscolo. Valido non sono per i caratteri ascii mono byte,
// ma anche per i caratteri multibyte del range latino esteso(U+00E0 - U+00FE),cioe' quella parte
// dello standard Unicode che contiene caratteri necessari per linguee europee occidentali
// che non siano parte dell'alfabeto inglese standard,come le lettere accentate minuscole à, é, ò, ñ, ü, ç. Unicode e' stato progettato
// in modo intelligente: la distanza di 32 tra minuscole e maiuscole e' mantenuta anche per i caratteri latini
// accentati
void utf8_to_upper(unsigned char *s)
{
    while (*s)
    {
        unsigned char *current = s;// puntatore che punta alla stringa,necesario perche nextt_utf8_char sposta s
        int codepoint = next_utf8_char(&s);// ritorna il codepoint del carattere attuale e sposta s al prossimo carattere
        if (codepoint >= 0x61 && codepoint <= 0x7A)// caratteri ASCII
        {
            codepoint &= ~32;// spengo il bit 5
            utf8_encode(codepoint, current);// encodo il codepoint cosi ottenuto in bytes su current,e quindi su s
        }
        else if (codepoint >= 0xE0 && codepoint <= 0xFE && codepoint != 0xF7)// caratteri latini estesi,con l'eccezione del simbolo della divisione
        {
            codepoint -= 32;
            utf8_encode(codepoint, current);
        }
    }
}

int main() {
    unsigned char string_ascii[] = "hello";
    unsigned char string_extended[] = "hélló";
    utf8_to_upper(string_ascii);
    utf8_to_upper(string_extended);
    printf("%s\n", string_ascii);// HELLO
    printf("%s\n", string_extended);// HÉLLÓ 
}