#include <stdio.h>
//popcount,conta il numero di bit accesi,chiamato cosi in onore di Richard Hamming
unsigned char hamming_weight(unsigned char n) {
    //approccio SWAR puro
    unsigned char even_bits = n & 0b01010101;//bit in posizione 0 2 4 6 con 0x55
    unsigned char odd_bits = n & 0b10101010;// bit in posizione 1 3 5 7 con 0xAA
    odd_bits = odd_bits >> 1;//allineo i bit dispari ai pari
    unsigned char sum = odd_bits + even_bits;//adesso ogni coppia di bits contiene il numero di bit accesi di quella coppia,quindi o 00, o 01, 0 10
    //replico lo stesso concetto prima per coppie di coppie,poi per i due nibble
    unsigned low_couples = sum & 0b00110011;//isolo le coppie basse 0x33
    unsigned high_couples = sum & 0b11001100;//isolo le coppie alte 0x
    high_couples = high_couples >> 2;//allineo le alte alle basse
    sum = low_couples + high_couples;//adesso ogni nibble vale 0,1,2,3,4
    unsigned char low_nibble = sum & 0b00001111;//0x0F
    unsigned char high_nibble = sum & 0b11110000;//0xF0
    high_nibble = high_nibble >> 4;
    return sum = low_nibble + high_nibble;
}
// oppure compressa 
// unsigned char hamming_weight(unsigned char n) {
//     n = (n & 0x55) + ((n & 0xAA) >> 1);
//     n = (n & 0x33) + ((n & 0xCC) >> 2);
//     n = (n & 0x0F) + ((n & 0xF0) >> 4);
//     return n;
// }
//seguiamo il flusso sul numero 01101100
//bit pari con maschera 0x55 = 01000100
//bit dispari con maschera 0xAA = 00101000
//shifto i bit dispari di 1 = 00010100
//sommo 01011000
//isolo le coppie basse con la maschera & 0x33 = 00010000
//coppie alte con maschera & 0xCC 01001000
//shifto le maschere alte di 2 a destra = 00010010
//sommo le coppie 00100010
//nibble basso con 0x0f = 00000010
//nibble alto con 0xF0 = 00100000
//shifto il nibble alto di 4 a destra = 00000010
//sommo e ottengo la somma finane = 00000100,cioe' 4
int main() {
    printf("%d\n", hamming_weight(0b00110101));
    printf("%d\n", hamming_weight(0b11111111));
    printf("%d\n", hamming_weight(0b11111001));
}