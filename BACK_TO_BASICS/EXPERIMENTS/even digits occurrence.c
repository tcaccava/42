#include <stdio.h>

int even_digits(int num){
    int occurrence = 0;
    if(num == 0)
        return 0;
    if((num % 10) % 2 == 0)//isola l'ultima cifra
        occurrence++;;
    occurrence+= even_digits(num / 10);//aggiunge il conteggio di tutte le cifre precedenti l'ultima
    return occurrence;//ritorna il numero della call stack
}
int main() {
    printf("%d\n", even_digits(142794));
}