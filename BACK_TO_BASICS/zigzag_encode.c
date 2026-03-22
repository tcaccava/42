#include <stdio.h>
// Codifica un numero signed in un unsigned usando la codifica zigzag — usata in Protocol Buffers
// di Google per comprimere interi piccoli;la formula mappa i numeri cosi':
//  0  → 0
//  -1 → 1
//  1  → 2
//  -2 → 3
//  2  → 4
//  -3 → 5
//  Mentre per i numeri unsigned il processore usa per lo shift a destra >> l'istruzione SHR,cioe'
//  logical shift right,che significa che entrano a sinistra degli zeri,per i signed usa l'istruzione SAR,
//  aritmetic shift right,che replica a sinistra il bit del segno per l'intero shift,il che significa che nel caso di
// un signed negativo,cioe' con il MSB uguale a 1,lo shift a destra fa apparire 1 a sinistra. Cio' avviene perche se
//  //per esempio dividiamo un numero negativo per 2 shiftando di uno a destra >> 1,ovviamente la divisione
//  deve dare numero negativo. Per esempio -4 signed e' 11111100, il suo >> 1 genera con SAR 11111110,cioe' meno due
unsigned char zigzag_encode(signed char n) {
    return (n << 1 ^ n >> 7);
    //con n << 1 raddoppiamo il numero,qualunque esso fosse produce sempre un numero pari
    //con n >> 7 propaghiamo il segno su tutti i bit,quindi otteniamo 0 per i positivi e 255 per i negativi
    //lo XOR di n << 1 con lo 0,quindi nei casi dei positivi, non cambia nulla,invece lo XOR con 255 inverte tutti i bit,il che trasforma 
    //il numero da pari a dispari
}

int main()
{
    printf("%d\n", zigzag_encode(4));//8
    printf("%d\n", zigzag_encode(-4));//7
}