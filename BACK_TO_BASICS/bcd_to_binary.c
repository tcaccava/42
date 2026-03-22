#include <stdio.h>
//BCD,binary code decimal e' un formato in cui ogni nibble rappresenta un cifra decimale,per cui 
//ad esempio 23 e' fatto da due,nibble basso,e da 3 nibble alto,quindi 00100011,che in esadecimale e' 0x23
//mentre normalmente in binario 23 sarebbe 00010111, cioe 0x17; quindi in sostanza il bcd e' quel formato binario
//in cui la forma esadecimale del numero equivale alla sua forma decimale
unsigned char bcd_to_binary(unsigned char bcd){
    return ((bcd >> 4) * 10) + (bcd & 15);
}

int main() {
    printf("%d\n", bcd_to_binary(0x23));
}