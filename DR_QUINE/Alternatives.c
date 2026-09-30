/* 
   Analisi delle tre macro-famiglie di Quine in C
   ==========================================================================
   Un programma self-replicating P non puo generare la propria informazione dal nulla. 
   Deve contenere una componente dati (D) e un algoritmo (A) tale che A(D) = A + D.

   Ciascuna variante sottostante risolve il problema della rappresentazione di
   D e dell'escaping di caratteri speciali (virgole, virgolette, new-line)
   tramite un meccanismo concettualmente e meccanicamente distinto.


   VARIANTE 1: Raw Syscall write() + Parser Byte-per-Byte con Marker ASCII
   --------------------------------------------------------------------------
   Con write (o con la system call sys_write in Assembly) non ho a disposizione la printf che fa la magia dell'espansione, quindi cambia completamente l'approccio: 
   si passa alla stampa sequenziale a blocchi (o tecnica prefix-data-suffix).
   Invece di unire codice e dati in un'unica stringa di formato, divido il programma in parti distinte e faccio più chiamate a write consecutive per ricomporre il puzzle:
   - il Prefisso: stampo tutto il blocco di codice che precede la dichiarazione della stringa dati.
   - il Delimitatore d'Apertura: stampo il carattere delle virgolette doppie (").
   - il Contenuto: stampo la stringa dati stessa (che contiene l'intero codice sorgente inclusa la riga che chiama la write).
   - il Delimitatore di Chiusura: stampo di nuovo le virgolette doppie (").
   - il Suffisso: stampo la parte finale del codice che chiude il programma.
   
   MECCANICA:
   Elimina totalmente le funzioni di formattazione della libc (printf/dprintf)
   e la gestione delle format-string (%s, %c). Senza una funzione che interpreti i segnaposti (%s, %c), la write si limita a trasferire byte grezzi 
   dalla memoria al descrittore di file 1 (stdout)
   
   Usa un'unica stringa D contenente tre caratteri marker di controllo:
     - ^ (ASCII 94): rappresenta il character \n (ASCII 10).
     - $ (ASCII 36): rappresenta il character "  (ASCII 34).
     - @ (ASCII 64): rappresenta l'auto-riferimento al puntatore s stesso.

   ALGORITMO:
   Un ciclo while (*p) scorre il payload D. Quando incontra @, sospende la
   stampa puntuale e invoca write(1, s, len) stampando l'intera stringa D
   tra le virgolette aperte e chiuse dal marker $.
*/

#include <unistd.h>
#include <stdio.h>

int main() {
    char *s = "#include <unistd.h>^int main(){char*s=$@$;char*p=s;while(*p){if(*p==64){int i=0;while(s[i])i++;write(1,s,i);}else if(*p==36){char q=34;write(1,&q,1);}else if(*p==94){char n=10;write(1,&n,1);}else write(1,p,1);p++;}return 0;}^";
    char *p = s;
    while (*p) {
        if (*p == 64) {
            int i = 0;
            while (s[i]) i++;
            write(1, s, i);
        } else if (*p == 36) {
            char q = 34;
            write(1, &q, 1);
        } else if (*p == 94) {
            char n = 10;
            write(1, &n, 1);
        } else {
            write(1, p, 1);
        }
        p++;
    }
    return 0;
}


/* 
   VARIANTE 2: Array di Stringhe + puts() / printf() (Esecuzione a 3 Fasi)
   --------------------------------------------------------------------------
   MECCANICA:
   Elimina la complessita' visiva dello stringone monolitico e le sequenze
   di escape esplicite per i new-line (\n) o i codici ASCII posizionali (%1$c).

   Il codice sorgente e' memorizzato in una matrice di stringhe codice[]
   terminata da un puntatore NULL.

   ALGORITMO (3 Fasi Sequenziali):
   - Fase 1 : puts() emette l'intestazione (#include e apertura array).
     Poiche puts(s) equivale a printf("%s\n", s), ogni nuova linea viene generata
     dall'I/O senza doverla inserire nella costante stringa.
   - Fase 2 (Auto-Replicazione dati): Un ciclo 'for' scansiona codice[]
     dall'indice 0 alla sentinella NULL. Per ogni elemento, printf() inietta
     gli spazi di indentazione, le virgolette aperte/chiuse e la virgola:
     printf("    \"%s\",\n", codice[i]).
   - Fase 3 : puts() riprende la stampa delle righe rimanenti
     partendo dall'indice 3, emettendo il corpo del main() e la chiusura dell'array.
*/


char *codice[] = {
    "#include <stdio.h>",
    "",
    "char *codice[] = {",
    "    NULL",
    "};",
    "",
    "int main() {",
    "    for (int i = 0; i < 3; i++) puts(codice[i]);",
    "    for (int i = 0; codice[i]; i++) printf(\"    \\\"%s\\\",\\n\", codice[i]);",
    "    for (int i = 3; codice[i]; i++) puts(codice[i]);",
    "    return 0;",
    "}",
    NULL
};

int main() {
    for (int i = 0; i < 3; i++) puts(codice[i]);
    for (int i = 0; codice[i]; i++) printf("    \"%s\",\n", codice[i]);
    for (int i = 3; codice[i]; i++) puts(codice[i]);
    return 0;
}


/* 
   VARIANTE 3: Preprocessor Stringification (#) Macro
   --------------------------------------------------------------------------
   MECCANICA:
   Invece di far generare le virgolette o le sequenze di escape al codice
   eseguibile in runtime, si demanda il lavoro al Preprocessore C durante
   la fase di Tokenizzazione.

   L'operatore # (Stringification) prende il token AST passato come
   argomento della macro 'a' e lo trasforma automaticamente in un valore
   literal tra Virgolette nella sezione .rodata del binario compilato.

   ALGORITMO:
   - Viene definita la macro Q(a).
   - Q viene invocata passando come argomento a l'INTERO corpo del main().
   - Il preprocessore genera due istanze della stringa tramite #a:
     1) La prima istanza viene passata a %s per formattare la direttiva #define.
     2) La seconda istanza viene passata a %s per formattare l'invocazione Q(...).
 */


#define Q(a) int main(){printf("#include <stdio.h>\n#define Q(a) %s\nQ(%s)\n", #a, #a);}

Q(int main(){printf("#include <stdio.h>\n#define Q(a) %s\nQ(%s)\n", #a, #a);})

