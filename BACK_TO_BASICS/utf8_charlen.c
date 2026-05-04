#include <stdio.h>
#include "./count_leading_ones.c"
// L'utf8 e' stato progettato per essere retrocompatibile con ASCII. I bit del primo byte 
// determinano la lunghezza della sequenza: se l'MSB del primo byte e' 0,il carattere e´
// un semplice ASCII a sette bit,che contengono l'informazione necessaria a codificare il carattere
// Se invece il carattere e' UTF8 ma non ASCII,sara'un carattere multibyte,quindi il primo byte inizia con una serie di 1,tanti quanti
// il numero totale di bytes della sequenza(fino a 4 per un utf8),seguiti da uno zero che e' a sua volta seguito
// dalla informazione vera e propria,il cosidetto Code Point. I byte successivi al primo saranno bytes di continuazione,quindi inizieranno tutti con 10.
// Quindi se il primo byte e' 0xxxxxxx si tratta sicuramente di un ASCII,se e' 110xxxxx si tratta di un utf multi bytes
// di 2 bytes totali,110xxxx 3 bytes totali,1110xxxx 4 bytes totali. I bytes di continuazione inizieranno sempre con 10, cd header di continuazione, proprio per evitare
// di confonderli,e quindi contengono ciascuno 6 bit di informazione.
// Questa funzione si propone di ritornare il numero di bytes occupati dal carattere utf8.
int utf8_charlen(unsigned char c) {
    return !(ft_count_leading_ones(c)) ? 1 : ft_count_leading_ones(c);
}
// oppure ancora piu' pulita e branchless
// int utf8_charlen(unsigned char c) {
//     return (ft_count_leading_ones(c) + (!ft_count_leading_ones(c)); 
// }
// Ovviamente bisogna passargli il PRIMO byte del carattere,non un byte successivo(per esempio un byte di continuazione): Garbage in,Garbage out.
int main() {
    printf("%d\n", utf8_charlen(0b11100010));
    printf("%d\n", utf8_charlen(0b11000010));
    printf("%d\n", utf8_charlen(0b11110010));
    printf("%d\n", utf8_charlen(0b00000010));
}