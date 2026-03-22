#include <unistd.h>

// converti un singolo char nelle sue cifre esadecimali
void hex_conv(unsigned char c)//su operazioni bitwise usa sempre unsigned char
{
    char hex[] = "0123456789abcdef";
    char high = hex[c >> 4];
    char low = hex[c & 0x0f];
    write(1, &high, 1);
    write(1, &low, 1);
    // un char corrisponde ad 1 byte,cioe' 8 bit,quindi viene internamente processato
    //  in forma binaria,per cui per es. 'a' = 97 corrisponde a 0110 0001; puoi estrarre le prime 4
    // cifre binarie con l'operatore bitwise >>,e le ultime 4 con l'operatore & usando una maschera 0x0f,cioe 16 in esadecimamle,
    // cioe 0000 1111.
    // nel caso di 'a' high e' 0110,cio',96,che in esadecimale e' 6;low,e' 0001,che in hex e' 1,
    //quindi a in esadecimale e' 0x61 
}

int main()
{
    hex_conv('a');
}