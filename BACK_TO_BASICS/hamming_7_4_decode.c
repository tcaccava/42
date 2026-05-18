#include <stdio.h>
#include "./hamming_7_4_encode.c"
unsigned char hamming_decode(unsigned char code)
{
    unsigned char sindrome = 0;
    unsigned char original_nibble = 0;
    sindrome |= (code >> 1 & 1) ^ (code >> 3 & 1) ^ (code >> 5 & 1) ^ (code >> 7 & 1);        // il primo bit di sindrome è lo XOR tra il bit di parità 1 e i bit 3,5,7 di code
    sindrome |= ((code >> 2 & 1) ^ (code >> 3 & 1) ^ (code >> 6 & 1) ^ (code >> 7 & 1)) << 1; // il 2° bit di sindrome è lo XOR tra il p2 e i bit 3,6,7
    sindrome |= ((code >> 4 & 1) ^ (code >> 5 & 1) ^ (code >> 6 & 1) ^ (code >> 7 & 1)) << 2; // il 3° è lo XOR tra il p3 e i bit 5,6,7
    if (!sindrome)
    {
        printf("Sindrome is 0,the code is correct,the 4 bit information is: ");
        return original_nibble |= ((code & 8) >> 3) | ((code & 32) >> 4) | ((code & 64) >> 4) | ((code & 128) >> 4); // restituiamo i 4 bit di informazione,cioè i bit 3,5,6,7 di code
    }
    else
    {
        printf("Sindrome is %u, the code is incorrect,correction incoming,the correct bit information is: ", sindrome);
        code ^= (1 << sindrome); // sindrome è esattamente l'indice del bit alterato,quindi invertiamo quel singolo bit di code
        return original_nibble |= ((code & 8) >> 3) | ((code & 32) >> 4) | ((code & 64) >> 4) | ((code & 128) >> 4);
    }
}
// oppure ancora più semplicemente
// unsigned char hamming_decode(unsigned char code)
// {
//     unsigned char sindrome = 0;

//     (invariato)
//     sindrome |= (code >> 1 & 1) ^ (code >> 3 & 1) ^ (code >> 5 & 1) ^ (code >> 7 & 1);
//     sindrome |= ((code >> 2 & 1) ^ (code >> 3 & 1) ^ (code >> 6 & 1) ^ (code >> 7 & 1)) << 1;
//     sindrome |= ((code >> 4 & 1) ^ (code >> 5 & 1) ^ (code >> 6 & 1) ^ (code >> 7 & 1)) << 2;

//     // Sfruttiamo il fatto che il bit 0 di hamming code non porta informazione,quindi anche se lo impostiamo a 1 non succede nulla
//     // Se è da 1 a 7, corregge il bit esatto. Zero condizioni.
//     code ^= (1 << sindrome);

//     // Sfruttiamo lo shift uniforme >> 4 per i tre bit alti, e ci sommiamo d1 isolato.
//     return ((code >> 3) & 1) | ((code & 0xE0) >> 4);
// }
int main()
{
    unsigned char number = 213;     // numero originario,nell'encoding verrà perso a favore del suo nibble basso
    unsigned char hamming_code = 0; // l'hamming code parte da 0
    hamming_encode(number, &hamming_code);
    printf("%u\n", hamming_code);                      // dopo aver encodato, hamming code è 90
    printf("%u\n", hamming_decode(hamming_code));      // il decode restituisce 5,cioè il nibble basse del numero originario 213
    hamming_code ^= (1 << 7); // altero volutamente il bit 7 di hamming code
    printf("%u\n", hamming_decode(hamming_code));      // il codice viene corretto,e viene restituito il codice corretto,90
}