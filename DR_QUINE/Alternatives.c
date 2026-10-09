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
   In un quine puro non posso usare funzioni ad alto livello come printf per iniettare stringhe o %c per stampare caratteri speciali. Se uso la syscall grezza write, mi scontro con tre ostacoli della sintassi C:
   - Le virgolette (" - ASCII 34): per dichiarare char *s = "..."; servono le virgolette. Ma se metto le virgolette dentro la stringa per descrivere sé stessa, dovrei scriverla come \", generando una ricorsione infinita di backslash di escape.
   - L'a capo (\n - ASCII 10): per mandare a capo il codice mi serve \n, che contiene il carattere speciale \,di cui dovrei fare l'escape.
   - L'espansione dell'autoreferenza (il payload D): la stringa s deve contenere sé stessa dentro la sua stessa dichiarazione char *s = "...";.

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
     - ^ (ASCII 94): rappresenta il character \n (ASCII 10 o 0x5E).
     - $ (ASCII 36): rappresenta il character "  (ASCII 34 o 0x24).
     - @ (ASCII 64 o 0x40): rappresenta l'auto-riferimento al puntatore s stesso, il cuore del Quine. Interrompe la stampa del singolo carattere e stampa l'intera stringa s dall'inizio alla fine.
    Perche' si scelgono questi 3 caratteri? Perche' non compaiono mai nel codice sorgente C e li si usa
    come "istruzioni di parsing" per il ciclo while. In sostanza i tre marker permettono di serializzare e 
    deserializzare il sorgente senza mai dover scrivere \n o \"  nel testo della stringa.

   ALGORITMO:
   Un ciclo while (*p) scorre il payload D. Quando incontra @, sospende la
   stampa puntuale e invoca write(1, s, len) stampando l'intera stringa D
   tra le virgolette aperte e chiuse dal marker $.

*/

// #include <unistd.h>

// int main() {
//     char *s = "#include <unistd.h>^int main(){char*s=$@$;char*p=s;while(*p){if(*p==64){int i=0;while(s[i])i++;write(1,s,i);}else if(*p==36){char q=34;write(1,&q,1);}else if(*p==94){char n=10;write(1,&n,1);}else write(1,p,1);p++;}return 0;}^";
//     char *p = s;
//     while (*p) {
//         if (*p == 64) {
//             int i = 0;
//             while (s[i]) i++;
//             write(1, s, i);
//         } else if (*p == 36) {
//             char q = 34;
//             write(1, &q, 1);
//         } else if (*p == 94) {
//             char n = 10;
//             write(1, &n, 1);
//         } else {
//             write(1, p, 1);
//         }
//         p++;
//     }
//     return 0;
// }


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

#include <stdio.h>

char *s[] = {
    "#include <stdio.h>",
    "",
    "char *s[] = {",
    "    NULL",
    "};",
    "",
    "int main() {",
    "    int i;",
    "    for (i = 0; i < 3; i++) puts(s[i]);",
    "    for (i = 0; s[i]; i++) printf(s[13], 34, s[i], 34, 10);",
    "    for (i = 3; i < 13; i++) puts(s[i]);",
    "    return 0;",
    "}",
    "    %c%s%c,%c",
    NULL
};

int main() {
    int i;
    for (i = 0; i < 3; i++) puts(s[i]);
    for (i = 0; s[i]; i++) printf(s[13], 34, s[i], 34, 10);
    for (i = 3; i < 13; i++) puts(s[i]);
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
   argomento della macro a e lo trasforma automaticamente in un valore
   literal tra Virgolette nella sezione .rodata del binario compilato
   a tempo di compilazione.

   ALGORITMO:
   - Viene definita la macro Q(a).
   - Q viene invocata passando come argomento a l'INTERO corpo del main().
   - Il preprocessore genera due istanze della stringa tramite #a:
     1) La prima istanza viene passata a %s per formattare la direttiva #define.
     2) La seconda istanza viene passata a %s per formattare l'invocazione Q(...).
 */


// #define Q(a) int main(){printf("#include <stdio.h>\n#define Q(a) %s\nQ(%s)\n", #a, #a);}

// Q(int main(){printf("#include <stdio.h>\n#define Q(a) %s\nQ(%s)\n", #a, #a);})


/*
Le tre presentate prima sono solo quelle affrontabili piu' facilmente, ma non sono assolutamente
le uniche alternative in cui si risolve il problema della generazione di un Quine. 
Qui presento le altre alternative tratte dalla letteratura disponibile sui Quine.
Sul piano implementativo ed espressivo esistono altre 5 varianti distinte che affrontano il problema con paradigmi totalmente diversi.

1) La variante ad Array Numerico / Hex-Dump (senza alcuna stringa)
Invece di memorizzare il codice come stringa di testo char *s = "...", lo si memorizza come array di interi contenenti i codici ASCII o Hex delle istruzioni.
ESEMPIO:
#include <stdio.h>

int c[] = {35,105,110,99,108,117,100,101,32,60,115,116,100,105,111,46,104,62,10,10,105,110,116,32,109,97,105,110,40,41,123,0};

int main() {
    // Ciclo 1: Stampa l'array convertendo gli interi in caratteri ASCII (il codice vero)
    for (int i = 0; c[i]; i++) putchar(c[i]);
    // Ciclo 2: Ristampa l'array in formato "int c[] = { ... }" per ricostruire la variabile
    // ...
}
Non esiste neanche una costante stringa nel sorgente ("" non appare mai). Elimina alla radice il problema dell'escaping delle virgolette perché i dati sono solo numeri.

2) La variante Inline Assembly / Register-Level (__asm__)
Sfrutta la possibilità del C di iniettare Assembly x86-64. Invece di usare variabili C, usa il puntatore all'istruzione (RIP) o legge direttamente il segmento .rodata sfruttando l'indirizzamento 
relativo della CPU ([rel ...]).
ESEMPIO: 
#include <unistd.h>

int main() {
    __asm__(
        ".global src\n"
        "src:\n"
        ".string \"...\"\n"
    );
    // Codice C che recupera l'indirizzo della label 'src' esportata dall'ASM
    // ed esegue la syscall write()
}
Il ponte dati non è gestito dal compilatore C, ma dal linker e dalle direttive dell'assembler (.string` / `.asciz).

3) La variante Self-Reading / File System (open(__FILE__))

Questa è la variante cheating (esplicitamente vietata dal subject e nei contest di quine puri).
ESEMPIO:
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    char buf[1024];
    int fd = open(__FILE__, O_RDONLY);
    int n = read(fd, buf, sizeof(buf));
    write(1, buf, n);
    close(fd);
}

Viola il Teorema di Kleene. Il programma non contiene la propria informazione, ma la legge dall'esterno (I/O su disco).

4) I Quine Relè / Multi-Linguaggio (Quine Relay)

Un programma C che quando viene eseguito non stampa C, ma genera un sorgente C++, che quando eseguito genera un sorgente Python, che a sua volta rigenera il sorgente C originale.

Sorgente C --->exec----->Sorgente C++----->exec----->Sorgente Python----->exec----->Sorgente C

La funzione A(D) non produce A+D, ma applica una trasformazione sintattica T_1(D) --> T_2(D) --> T_0(D) mantenendo invariata l'informazione D attraverso N linguaggi diversi.

5) I Quine "Radiation Hardened" (Ouroboros / Intron Quines)

Sono quine progettati con ridondanza di codice o strutture a matrici simmetriche. Se cancello una qualsiasi riga di codice o elimino un carattere casuale, il programma continua a 
compilare e, quando eseguito, ripara il danno stampando il quine originale completo di tutte le righe.

### Sintesi delle Famiglie in C

QUINE IN LINGUAGGIO C
├── 1. PURI (Self-Contained in Memoria)
│   ├── Stringa Monolitica
│   │   ├── printf() con ASCII posizionali (%1$c, %2$c, 34, 10)
│   │   └── write() / putchar() con Parser e Marker ASCII (@, $, ^)
│   ├── Strutturati
│   │   ├── Array di Stringhe + puts() (A 3 fasi)
│   │   └── Array Numerico/Hex (Senza stringhe literals)
│   └── Preprocessore
│       └── Macro con Stringificazione (#a)
│
├── 2. IBRIDI / AVANZATI
│   ├── Inline Assembly (__asm__ + .rodata labels)
│   ├── Quine Relay (C -> C++ -> Python -> C)
│   └── Radiation Hardened (Self-healing code)
│
└── 3. SPURI / IMPURI (I/O Esterno)
    └── Reading File (__FILE__ / open) [VIETATO A 42]

Tutto il resto rientra nell'esoteric programming, un reame dove l'obiettivo non è scrivere software efficiente o manutenibile, ma fare sfoggio di virtuosismo teorico.
Gli unici spunti didattici che si possono trarre da queste versioni sono :
- Array Numerico / Hex-Dump (virus informatici): il concetto di trattare il codice come un array di numeri è alla base della cybersecurity offensiva (shellcode injection) e dei compilatori JIT (Just-In-Time). 
  Se scrivo un exploit, non inietto una stringa, inietti un array di byte esadecimali nella memoria e costringo la CPU a saltarci dentro. 

- Radiation Hardened (la genetica del software): il concetto è affascinante perché mima esattamente i meccanismi di riparazione del DNA cellulare. Usa ridondanza dell'informazione (codici di correzione d'errore) 
  per ricostruire il dato originale se un segmento viene danneggiato da una radiazione cosmica (o da un utente che cancella una riga),una delle maggiori cause di bit flip(e conseguenti BSOD). È un parallelismo biologico stupendo, ma 
  informaticamente si usa solo nei sistemi aerospaziali, e non certo sotto forma di quine.

*/