#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

    //--ARRAY FISSO---------------------------------------------------------------------------
    char fixed[3][5] = {"ciao", "come", "va"};//array fisso di 3 righe di 5 colonne ciascuna,a tutti gli effetti un array di array di caratteri
    for(int i = 0; i < 3; i++)
        printf("%s\n",  fixed[i]);
    fixed[0][0] = 'm'; //adesso ciao e' miao, la stringa e' modificabile perche' e' un array di char,non una literal string;
    printf("%s\n", fixed[0]);
    //fixed e' a tutti gli effetti un array di caratteri che risiedono fisicamente sulla stack in spazi di memoria contigui

    //--ARRAY DI PUNTATORI----------------------------------------------------------------------
    char *array[] = {"ciao", "come", "va"};//array di puntatori,che risiedono sulla stack,ognuno dei quali punta ad una literal string che risiede sulla read only
    for(int i = 0; i < 3; i++)
        printf("%s\n",  array[i]);

    char *parray[3];//array di puntatori a char che risiedono sulla stack;
    parray[0] = malloc(5);//a differenza dell'esempio precedente questa volta popolo i puntatori con stringhe malloccate,quindi posso modificarle
    parray[1] = malloc(5);
    parray[2] = malloc(5);

    strcpy(parray[0],"ciao");
    strcpy(parray[1], "come");
    strcpy(parray[2], "va");

    for(int i = 0; i < 3; i++)
        printf("%s\n",  parray[i]);

    parray[0][0] = 'm';
    printf("%s\n", parray[0]);// miao

    //--ARRAY DINAMICO--------------------------------------------------------------------------

    char **matrix = malloc(sizeof(char*) * 3);// qui ho semplicemente allocato memoria dinamica sulla heap per 3 puntatori,e matrix,che e' pure lui un puntatore che sta sulla stack, punta al primo dei 3
    //adesso quei puntatori posso farli puntare a stringhe literal sulla read only memory o a array di char sulla stack o piazzarle sulla heap

    //literal non modificabili
    matrix[0] = "ciao";
    matrix[1] = "come";
    matrix[2] = "va";

    for(int i = 0; i < 3; i++)
        printf("%s\n",  matrix[i]);

    // array di char modificabili ma non riassegnabili 
    char str1[] = "ciaone";
    char str2[] = "lollazzo";
    char str3[] = "stocazzo";

    // str1 = "troll"; ERRORE

    matrix[0] = str1;
    matrix[1] = str2;
    matrix[2] = str3;

    for(int i = 0; i < 3; i++)
        printf("%s\n",  matrix[i]);

    //heap strings modificabili e riassegnabili
    matrix[0] = malloc(5);
    matrix[1] = malloc(5);
    matrix[2] = malloc(5);

    strcpy(matrix[0], "tobi");
    strcpy(matrix[1], "lel");
    strcpy(matrix[2], "hell");

    for(int i = 0; i < 3; i++)
        printf("%s\n",  matrix[i]);

    char *lmao = "cesso";
    matrix[0] = lmao;//riassegnazione
    printf("%s\n", matrix[0]);

    //adesso ricostruiamo i livelli di indirezione
    printf("%c\n", **matrix);//il primo carattere della prima stringa,quindi la c di cesso
    printf("%p\n", *matrix);//dereferenzio un doppio puntatore,quindi accedo al primo puntatore,quello che punta alla prima stringa,e che contiene il suo indirizzo di memoria
    printf("%p\n", &matrix[0]);// identico al precedente,l'indirizzo di memoria della prima stringa,ma ci accedo direttamente tramite indice
    printf("%p\n", matrix);// il puntatore matrix stesso,che contiene l'indirizzo di memoria del primo puntatore,diverso da quello della variabile puntata,che e' quello di sopra
    printf("%s\n", *matrix);// la prima stringa stessa,cioe cesso
    printf("%p\n", &matrix);// l'indirizzo di memoria del puntatore matrix


    //--ARRAY DINAMICO OTTIMIZZATO PER MOTORI GRAFICI-------------------------------------------
    int rows = 3;
    int cols = 5;
    char *optimized = malloc(sizeof(char) * rows * cols); //creo spazio per un unica grande stringa,con caratteri tutti contigui,matrix e' un pointer che sta sulla stack e punta al primo di questi caratteri
    strcpy(optimized, "abcdefghilmnop");//ci piazzo una literal di 14 lettere + /0
    printf("%s\n", optimized);
    //l'accesso al singolo carattere alla riga i colonna j si fa con si fa con matrix[i * cols + j]
    //se voglio scrivere per esempio un carattere alla riga 2 colonna 3 dovro' fare:
    optimized[2 * 5 + 3] = 'Y';
    printf("%s\n", optimized);

    //una variante piu' comoda per accedere con il classico matrix[i][j] consiste nel creare un array di puntatori e farli puntare all'inizio di ogni singola riga dello stringone di sopra
    char **pointers = malloc(sizeof(char *) * 3); //creo sulla heap un array di puntatori
    for (int i = 0;  i < rows; i++)
        pointers[i] = optimized + (i * cols);
    //adesso posso accedere con gli indici i e j contemporaneamente:
    pointers[1][3] = 'Z';
    printf("%s\n", optimized);

    //il vantaggio di questo approccio e' che mi consente di limitare il numero di malloc,addirittura 1 solo per la prima variante,
    //due per la seconda con l'array di puntatori, e che mantengo la possibilita' di modificare tutti i char,perche non sono
    //literal string ma stanno sulla heap,e inoltre tutti i caratteri dello stringone sono contigui in memoria. In una matrice classica,
    //se avessi 1000 righe,dovrei fare 1001 malloc(uno per ogni riga + uno per la matrice intera),con la matrice contigua ne faccio uno o al massimo due
    //la variante con l'array di puntatori. I processori moderni leggono blocchi di memoria contigua,sui quali le operazioni di lettura/scrittura 
    //sono piu' veloci,quindi e' ottimizzato per la cache. Su matrici di centinaia o migliaia di righe o in casi di molte operazioni in sequenze sulle righe,
    //ambiti in cui le performance sono critiche(raycasting,grafica,simulazioni), la differenza con il classico array dinamico e' enorme.

}