#include <stdio.h>
#include <unistd.h>

int main() {
    char *array[] = {"ciao","come","va", NULL};
    char **p = array;
    printf("%s\n", *array);// ciao : dereferenzia il **array,quindi accede alla prima stringa;
    printf("%c\n", **array);// c : dereferenzia il **array,quindi accede alla prima stringa,e la dereferenzia nuovamente,ottenendo il primo carattere;
    printf("%c\n", (*array)[2]);// a : le () hanno priorita' maggiore delle [],quindi prima dereferenzia array accedendo alla prima stringa,poi accede al terzo carattere
    printf("%c\n", *array[2]);// v : le [] hanno priorita' > di *,quindi prima accede alla terza stringa,poi dereferenzia accedendo al primo carattere
    char *s = *p++;// *p++ e' uguale a *(p++),si muove p
    printf("%s\n", s);// ciao : s viene fatta puntare dove punta *p,cioe array[0],POI il puntatore p viene spostato,post incremento
    printf("%s\n", *p);// come
    p--;// ripristino la posizione di p su ciao
    s = *++p;// anche qui si muove p
    printf("%s\n", s);// come : PRIMA incrementa p,poi s accede al valore di p spostato,preincremento;
    p--;// ripristino p su ciao
    s = ++*p;// l'ordine sx-dx conta,qui si muove *p, cioe prima dereferenzia p accedendo a ciao,poi muove il puntatore derefereniato saltando la prima lettera 
    printf("%s\n", s);// iao
}

//in sostanza l'ordine delle priorita' e' () [] ++ *
//se ++ e' attaccato a p si muove il puntatore p,se e' attaccato a *p si muove cio che e' puntato