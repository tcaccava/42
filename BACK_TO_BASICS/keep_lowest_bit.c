#include <stdio.h>

unsigned char keep_lowest_bit(unsigned char n) {
    return n &(-n);
}

//il trucco sta nel complemento a due: un numero binario non puo' essere negativo,quindi
//si stabilisce che il negativo di n e' il numero che ,sommato ad n,da 0.
//per un numero a 8 bit per esempio il negativo di n e' quel numero che
//sommato ad n da 10000000,quindi un numero a 9 bit,di cui viene troncato il MSB,quindi si ottiene 
//00000000,ovvero 0. Per esempio il negativo di 00001001,cioe 9,e' quel numero che sommatogli da
//100000000,che e' 256,e quel numero e' quindi 247,cioe 11110111.Quindi -9 a 8 bit e' 11110111.
//Ovviamente per vederlo stampato da printf come -9 bisogna passargli un tipo signed come char,non unsigned char
//Il complemento a due consiste quindi.per passare dal positivo al negativo,nell' invertire tutti i bit e 
//poi aggiungere uno. Il fondamento sta nel fatto che sommando un numero al suo inverso in bit,ottenibile con la ~,
//si ottiene un binario con tutti 1,e per ottenere l'overflow di cui sopra bisogna ancora aggiungere 1.
//Quindi il negativo di un numero e' il suo inverso in bit + 1;
//Ma come arriviamo da questo a keep_lowest_bit? Supponiamo di avere 10110100,cioe 180. Il suo negativo -n e'
//n invertito + 1,quindi ~n + 1,cioe 01001011 + 1,cioe 75 + 1,cioe 76.Infatti 180 + 76 = 256,overflow a 8 bit.
//Quando aggiungo 1 ad 01001011 succede che il carry si propaga fino da destra fino a che non trova uno zero,
//e quel primo zero diventa 1,cioe' 01001100. Questo numero ha tutti i bit sopra il primo 1 di n invertiti,
//e tutti i bit sotto azzerati. L'unico elemento immodificato e' il primo 1 di n,e quindi l'AND
//tra n e il suo negativo isola il primo bit acceso in n.

int main() {
    char meno_nove = 0b11110111;
    printf("%d\n",meno_nove);// -9
    char complemento_a_due = ~
    printf("%d\n", keep_lowest_bit(0b00101001));// 00000001
    printf("%d\n", keep_lowest_bit(0b00010000));// 00010000
}