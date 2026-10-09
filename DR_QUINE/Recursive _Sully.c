/*
Questo approccio con un main autoricorsivo e' concettualmente valido e puo' essere a tutti gli effetti considerato un Quine,ma non e' pienamente conforme al subject
perche' prevede che ci sia un solo processo padre che ad ogni ricorsione forka,generando un figlio che si limita a compilare il nuovo file Sully_x.c generando un eseguibile 
che pero' non viene mai eseguito.Questo approccio implementa una Bounded Process Cascade (cascata di processi lineare finita).Il punto didattico di Sully  è che ogni generazione deve essere un'entità indipendente.
Se ho inteso bene il subject,pretende che ogni nuovo figlio ,che differisce dal precedente solo per il valore di run,compili generando un 
eseguibile che viene runnato,generando un nuovo sorgente e cosi' via. Ho implementato questo approccio,in cui il padre forka due volte,generando due figli,una per la compilazione
e una per la esecuzione,nel codice che ho pushato.
In C e' legale secondo lo standard ISO C usare un main ricorsivo,essendo il main a tutti gli effetti una normale funzione con linkage esterno. Puo' quindi essere chiamata ricorsivamente (sia direttamente che indirettamente) e 
se ne può anche prelevare l'indirizzo tramite un puntatore a funzione. Ogni chiamata ricorsiva crea un nuovo stack frame nello stack utente. Se non si definisce una condizione di terminazione, il programma andrà in stack overflow (SIGSEGV).
Il ritorno (return) dalle chiamate ricorsive intermedie smonta semplicemente lo stack frame corrente; l'esecuzione del processo termina solo quando si esegue il return dal main originale (quello invocato dalla libc) o quando viene chiamata exit().
In C++ (Standard ISO C++) invece un main ricorsivo è vietato. Lo standard ISO C++ (sezione [basic.start.main]) stabilisce esplicitamente:"The function main shall not be used within a program. "Non è consentito chiamare main() ricorsivamente, prenderne l'indirizzo, 
né effettuare l'overloading della funzione.Invocare main() in un sorgente C++ produce un errore di compilazione oppure un Undefined Behavior (UB) se il compilatore non applica rigorosamente il vincolo.
*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char **argv, char **envp)
{
    (void)argc;
    (void)argv;
    static short runs = 5;
    if (runs < 0)
        return 0;

    char filename[] = "Sully_ .c";
    filename[6] = (unsigned char)(runs + '0');
    
    int file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (file < 0)
    {
        perror("Error opening the file");
        return 1;
    }

    char *src_code = "#include <stdio.h>%1$c#include <unistd.h>%1$c#include <fcntl.h>%1$c#include <stdlib.h>%1$c#include <sys/wait.h>%1$c#include <sys/types.h>%1$c%1$cint main(int argc, char **argv, char **envp)%1$c{%1$c    (void)argc;%1$c    (void)argv;%1$c    static short runs = %4$d;%1$c    if (runs < 0)%1$c        return 0;%1$c    char filename[] = %2$cSully_ .c%2$c;%1$c    filename[6] = (unsigned char)(runs + '0');%1$c    int file = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0777);%1$c    if (file < 0)%1$c    {%1$c        perror(%2$cError opening the file%2$c);%1$c        return 1;%1$c    }%1$c    char *src_code = %2$c%3$s%2$c;%1$c    dprintf(file, src_code, 10, 34, src_code, runs);%1$c    close(file);%1$c    pid_t pid = fork();%1$c    if (pid < 0)%1$c        return 1;%1$c    char output[8] = %2$cSully%2$c;%1$c    if (runs > 0)%1$c    {%1$c        output[5] = '_';%1$c        output[6] = (unsigned char)(runs + '0');%1$c        output[7] = 0;%1$c    }%1$c    int exit_status = 0;%1$c    if (pid == 0)%1$c    {%1$c        execve(%2$c/usr/bin/gcc%2$c, (char *[]){%2$cgcc%2$c, filename, %2$c-o%2$c, output, NULL}, envp);%1$c        exit(1);%1$c    }%1$c    else%1$c    {%1$c        waitpid(pid, &exit_status, 0);%1$c        if (WIFEXITED(exit_status) && WEXITSTATUS(exit_status) == 0)%1$c        {%1$c            runs--;%1$c            main(argc, argv, envp);%1$c        }%1$c    }%1$c    return 0;%1$c}%1$c";

    dprintf(file, src_code, 10, 34, src_code, runs);
    close(file);

    pid_t pid = fork();
    if (pid < 0)
        return 1;

    char output[8] = "Sully";
    if (runs > 0)
    {
        output[5] = '_';
        output[6] = (unsigned char)(runs + '0');
        output[7] = 0;
    }

    int exit_status = 0;
    if (pid == 0)
    {
        execve("/usr/bin/gcc", (char *[]){"gcc", filename, "-o", output, NULL}, envp);
        exit(1);
    }
    else
    {
        waitpid(pid, &exit_status, 0);
        if (WIFEXITED(exit_status) && WEXITSTATUS(exit_status) == 0)
        {
            runs--;
            main(argc, argv, envp);
        }
    }
    return 0;
}
