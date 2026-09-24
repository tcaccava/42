/* ==========================================================================
   alternatives.c — Analisi delle tre macro-famiglie di Quine in C
   ==========================================================================
   Teorema di Ricorsione di Kleene: Un programma self-replicating $P$ non puo
   generare la propria informazione dal nulla. Deve contenere una componente
   dati ($D$) e un algoritmo ($A$) tale che $A(D) \implies A + D$.

   Ciascuna variante sottostante risolve il problema della rappresentazione di
   $D$ e dell'escaping di caratteri speciali (virgole, virgolette, new-line)
   tramite un meccanismo concettualmente e meccanicamente distinto.
   ========================================================================== */

#if defined(RUN_WRITE_VARIANT)

/* --------------------------------------------------------------------------
   VARIANTE 1: Raw Syscall write() + Parser Byte-per-Byte con Marker ASCII
   --------------------------------------------------------------------------
   MECCANICA:
   Elimina totalmente le funzioni di formattazione della libc (printf/dprintf)
   e la gestione delle format-string (%s, %c).
   
   Usa un'unica stringa $D$ contenente tre caratteri marker di controllo:
     - '^' (ASCII 94): Rappresenta il character '\n' (ASCII 10).
     - '$' (ASCII 36): Rappresenta il character '"'  (ASCII 34).
     - '@' (ASCII 64): Rappresenta l'auto-riferimento al puntatore 's' stesso.

   ALGORITMO:
   Un ciclo 'while (*p)' scorre il payload $D$. Quando incontra '@', sospende la
   stampa puntuale e invoca write(1, s, len) stampando l'intera stringa $D$
   tra le virgolette aperte e chiuse dal marker '$'.
   -------------------------------------------------------------------------- */

#include <unistd.h>

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

#elif defined(RUN_PUTS_VARIANT)

/* --------------------------------------------------------------------------
   VARIANTE 2: Array di Stringhe + puts() / printf() (Esecuzione a 3 Fasi)
   --------------------------------------------------------------------------
   MECCANICA:
   Elimina la complessita visiva dello "stringone monolitico" e le sequenze
   di escape esplicite per i new-line (\n) o i codici ASCII posizionali (%1$c).

   Il codice sorgente e memorizzato in una matrice di stringhe 'codice[]'
   terminata da un puntatore NULL (sentinella).

   ALGORITMO (3 Fasi Sequenziali):
   - Fase 1 (Linee 0..2): puts() emette l'intestazione (#include e apertura array).
     Poiche puts(s) equivale a printf("%s\n", s), ogni nuova linea viene generata
     dall'I/O senza doverla inserire nella costante stringa.
   - Fase 2 (Auto-Replicazione dati): Un ciclo 'for' scansiona 'codice[]'
     dall'indice 0 alla sentinella NULL. Per ogni elemento, printf() inietta
     gli spazi di indentazione, le virgolette aperte/chiuse e la virgola:
     printf("    \"%s\",\n", codice[i]).
   - Fase 3 (Linee 3..end): puts() riprende la stampa delle righe rimanenti
     partendo dall'indice 3, emettendo il corpo del main() e la chiusura dell'array.
   -------------------------------------------------------------------------- */

#include <stdio.h>

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

#else

/* --------------------------------------------------------------------------
   VARIANTE 3: Preprocessor Stringification (#) Macro
   --------------------------------------------------------------------------
   MECCANICA:
   Invece di far generare le virgolette o le sequenze di escape al codice
   eseguibile in runtime, si demanda il lavoro al Preprocessore C durante
   la fase di Tokenizzazione.

   L'operatore '#' (Stringification) prende il token AST passato come
   argomento della macro 'a' e lo trasforma automaticamente in un valore
   literal Tra Virgolette nella sezione .rodata del binario compilato.

   ALGORITMO:
   - Viene definita la macro 'Q(a)'.
   - 'Q' viene invocata passando come argomento 'a' l'INTERO corpo del main().
   - Il preprocessore genera due istanze della stringa tramite '#a':
     1) La prima istanza viene passata a %s per formattare la direttiva #define.
     2) La seconda istanza viene passata a %s per formattare l'invocazione Q(...).
   -------------------------------------------------------------------------- */

#include <stdio.h>

#define Q(a) int main(){printf("#include <stdio.h>\n#define Q(a) %s\nQ(%s)\n", #a, #a);}

Q(int main(){printf("#include <stdio.h>\n#define Q(a) %s\nQ(%s)\n", #a, #a);})

#endif

// # Compila ed esegue la variante write()
// gcc -DRUN_WRITE_VARIANT alternatives.c -o quine_write && ./quine_write

// # Compila ed esegue la variante puts()
// gcc -DRUN_PUTS_VARIANT alternatives.c -o quine_puts && ./quine_puts

// # Compila ed esegue la variante Macro # (Default)
// gcc alternatives.c -o quine_macro && ./quine_macro