#include <stdio.h>

int main() {
    char *s = "#include <stdio.h>%1$c%1$cint main() {%1$c    char *s = %2$c%3$s%2$c;%1$c    printf(s, 10, 34, s);%1$c}%1$c";
    printf(s, 10, 34, s);
}

//variante K&R
main(){char*s="main(){char*s=%c%s%c;printf(s,34,s,34,10);}%c";printf(s,34,s,34,10);}


/*
Questo snippet ,e le sue varianti ancora piu' corte scritte nello in C Ansi o nello stile K&R, o disabilitando gli avvisi di prototipo e tipo di ritorno con standard permissivi,
sono il limite matematico assoluto (il pavimento) per un Quine in C basato su printf: non esiste un modo per renderli più corto o più semplice di così, 
perché ogni singolo carattere presente ha una funzione vitale e insostituibile nel ciclo di auto-riproduzione. Se provo a togliere anche solo una parentesi o uno spazio, 
rompo l'isomorfismo geometrico e la stringa smette di descrivere se stessa.
*/