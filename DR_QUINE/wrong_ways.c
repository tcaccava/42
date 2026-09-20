
// ==============================================================================
// IL PARADOSSO DELL'INFINITO REGRESSO (limite del codice monolitico)

// Questo e' il massimo livello di autoriferimento che un blocco di codice 
// monolitico puo' raggiungere senza distinzione tra componente attiva e passiva.

#include <stdio.h>
#define S "#include <stdio.h>\n#define S \nint main() {\n\tprintf(S);\n}\n"

void monolithic_limit() {
    printf(S);
}

/*
 ANALISI MATEMATICA (Perche' fallisce):
 Se eseguo la funzione, l'output sara':
 #include <stdio.h>
 #define S 
 int main() {
    printf(S);
  } 
 Manca la stringa stessa assegnata alla macro S nella riga 2.
 Per far sì che il codice stampi la propria stringa, la stringa 'S' dovrebbe 
 contenere se stessa. Se provassimo ad aggiungerla, S conterrebbe S, che a sua 
 volta contiene S, generando una stringa di lunghezza infinita. 
 Questo è il limite invalicabile dimostrato dal Teorema di Ricorsione di Kleene.
 L'unico modo per risolverlo è scindere il programma:
 - Dati Passivi (Data): una stringa formattabile.
 - Codice Attivo (Code): istruzioni che usano i dati passivi DUE VOLTE 
   (una volta interpretandoli come codice eseguibile, una volta come dati grezzi).
 */


// ==============================================================================
// IL FALSO QUINE (tramite I/O, argv e __FILE__)

// Molti credono che aprire il file sorgente dal disco sia un Quine. 
// E' un approccio perdente e concettualmente nullo.

#include <stdlib.h>

void pseudo_quine_argv_file(int argc, char **argv) {
    
    // TENTATIVO 1: Usare il nome dell'eseguibile (argv[0]) per dedurre il file .c
    // char filename[256];
    // snprintf(filename, sizeof(filename), "%s.c", argv[0]);
    // FILE *file = fopen(filename, "r");

    // TENTATIVO 2: Usare la macro di precompilazione __FILE__
    FILE *file = fopen(__FILE__, "r");

    if (file) {
        int c;
        while ((c = fgetc(file)) != EOF) {
            putchar(c);
        }
        fclose(file);
    }
}

/*
 Perche' e' scientificamente invalido ed equivale ad un cheat:
 - Definizione di input: un vero Quine è una Macchina di Turing definita
   "senza input". Deve generare il proprio codice a partire esclusivamente dal 
   suo stato interno (la memoria del binario). Aprire un file significa leggere 
   un input fornito dal filesystem del Sistema Operativo.

 - Rottura del paradigma di autoreplicazione: l'informazione "genetica" del 
   programma non risiede nel programma stesso (come in un virus), ma nel disco 
   ospite. Se compilo questo file in anti_quine, lo sposto in un'altra cartella 
   ed elimino il sorgente anti_quine.c, il binario si rompe e smette di funzionare. 
   Un VERO Quine (che usa dati in memoria) stamperà il sorgente .c indipendentemente 
   dal fatto che esista sul disco o meno.
 */

int main(int argc, char **argv) {
    printf("--- RISULTATO DEL CODICE MONOLITICO ---\n");
    monolithic_limit();
    
    printf("\n--- RISULTATO DEL CHEAT ---\n");
    pseudo_quine_argv_file(argc, argv);
    
    return 0;
}