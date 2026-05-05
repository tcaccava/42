#include <stdio.h>
// Unicode e' un enorme tabella universale dove ogni simbolo mai creato dall'uomo(lettere latine,cirilliche
// ,geroglifici,emoji e simboli matematici) ha un numero identificativo unico e fisso,detto Codepoint.
// Si tratta di un codice identificativo unico,cioe' un indice numerico,per convenzione scritto col prefisso U+ seguito
// da un numero esadecimale. Per esempio la lettera A e' U+0041,che e' 65 in esadecimale,mentre l'emoji della cacca
// e' U+1F4A9,cioe' 128169 in decimale.
// Il sistema Unicode e' limitato ad un range che va da U+0000 a U+10FFFF,il che significa che il valore massimo del
// codepoint e' 1114111. Per organizzare questa giungla di simboli Unicode e' diviso in 17 Planes,distinti in
// Basic Multilingual Plane(BMP), da U+0000 a U+FFFF,che comprende la maggioranza degli alfabeti e simboli piu' comuni e usati,
// e i Supplementary Planes, da U+10000 a U+10FFFF,che contiene simboli rari,emoji e linguaggi estinti o storici.
// Bisogna a questo punto introdurre una distinzione fondamentale tra Codepoint Unicode,che identifica univocamente il carattere, e
// l'encoding(UTF-8,UTF-16,UTF-32),che dice invece come quel carattere viene scritto in memoria.
// Quindi,rifacendoci alla emoji di cui sopra, in UTF-32 sarebbe semplicemente 0x0001F4A9,in UTF-8 quel numero e' troppo grande per un byte,quindi,visto
// che ogni cifra esadecimale e' 4 bit,quelle 5 cifre esadecimali corrispondono a 17 bit di informazione:
// 1 corrisponde a 0001
// F a 1111
// 4 a 0100
// A a 1010
// 9 a 1001
// in totale sono 17 bit. UTF-8 puo' ospitare solo 11 bit di dati nella forma a 2 bytes,e solo 16 in quella a 3 bytes,quindi abbiamo bisogno
// del contenitore a 4 bytes,che puo' ospitare fino a 21 bit di dati.
// Quindi dobbiamo prendere i 17 bit di 0x1F4A9 e "infilarli" negli spazi della struttura di UTF-8:
// gli ultimi 6 bit(101001) vanno nel quarto bytes,con header 10 di continuazione,quindi 10101001
// i 6 bit successivi vanno nel byte terzo,quindi 10010010,poi gli altri 6 in 10011111,quindi nel primo byte i 3 bytes avanzati,cioe` 000,
// in aggiunta all'header 11110,quindi 1111000. Risultato finale F0 9F 92 A9.

// Converto il Codepoint Unicode nella codifica Utf-8 con sole operazioni bitwise,e ritorno il numero di bytes scritti
int utf8_encode(unsigned int codepoint, unsigned char *out)
{
    if (codepoint <= 0x7F) // se codepoint e' inferiore a 127,puo' essere contenuto in un solo byte utf-8
    {
        out[0] = (unsigned char)codepoint; // casto semplicemente codepoint ad unsigned char,che e' un contenitore a 8 bit
        return 1;                          // scritto 1 solo byte
    }
    else if (codepoint > 0x7f && codepoint <= 0x7FF) // codepoint compreso tra 128 e 2^11 - 1,quindi puo' essere contenuto in due byte con codifica utf8,che possono contenere esattamente 11 bit di informazione
    {
        out[1] = 0b10000000 | (codepoint & 0b00111111); // il byte 1 e' un byte di continuazione,quindi comincia con 10 e finisce con i 6 bit finali di codepoint
        codepoint >>= 6;                                // ho consumato 6 bit,quindi aggiorno codepoint
        out[0] = 0b11000000 | (codepoint & 0b00011111); // il byte iniziale contiene l'header 110 per i caratteri utf8 a due bytes,e 5 bytes di informazione
        return 2;
    }
    else if (codepoint > 0x7FF && codepoint <= 0xFFFF) // il codepoint e' inferiore a 2 ^ 16 - 1,quindi puo' essere contenuto in un carattere utf-8 a 3 bytes,che porta 16 bit di informazione
    {
        out[2] = 0b10000000 | (codepoint & 0b00111111);
        codepoint >>= 6;
        out[1] = 0b10000000 | (codepoint & 0b00111111);
        codepoint >>= 6;
        out[0] = 0b11100000 | (codepoint & 0b00001111); // contiene l'header 1110 e i 4 bit di codepoint
        return 3;
    }
    else if (codepoint > 0xFFFF && codepoint <= 0x10FFFF) // 10FFFF e' il limite massimo di unicode
    {
        out[3] = 0b10000000 | (codepoint & 0b00111111);
        codepoint >>= 6;
        out[2] = 0b10000000 | (codepoint & 0b00111111);
        codepoint >>= 6;
        out[1] = 0b10000000 | (codepoint & 0b00111111);
        codepoint >>= 6;
        out[0] = 0b11110000 | (codepoint & 0b00000111); // header 11110 e 3 bit di codepoint
        return 4;
    }
    return 0; // codepoint non valido o fuori standard Unicode.
}

int main()
{
    unsigned char out[4];
    printf("%d\n", utf8_encode(0x1F4A9, out)); // 4 bytes
    for (int i = 0; i < 4; i++)
        printf("%X", out[i]); // cosi' l'emoji Pile of Poo,codepoint U+1F4A9, diventa F09F92A9
}