#include "cat.h"

void read_stdin()
{
    int n = 0;
    char buf[4096];
    while ((n = read(0, buf, sizeof(buf))) > 0)
        write(1, buf, n);
    if (n == -1)
        perror("cat"); // perror stampa una stringa di errore nello stderr basata su errno anche se non glielo passi, o argomento: <descrizione errore> se gli passi un argomento
}

void put_str(char *s)
{
    while (*s)
        write(2, s++, 1);
    write(2, "\n", 1);
}

void print_error(char *av)
{
    write(2, "cat: ", 5);
    char *file_name = basename(av);
    write(2, file_name, strlen(file_name));
    write(2, ": ", 2);        // basename prende come parametro una char* che rappresenta il path assoluto o relativo del file e ritona un char * che punta al nome del file o della directory nel path
    put_str(strerror(errno)); // strerror converte la macro errno in una stringa che ritorna
}

void display_file(char *av)
{
    ssize_t n = 0;
    int fd = open(av, O_RDONLY);
    if (fd == -1)
    {
        print_error(av); // strerror converte la macro errno in una stringa che ritorna
        return;
    }
    char buf[4096];
    while ((n = read(fd, buf, sizeof(buf))) > 0)
        write(1, buf, n);
    if (n == -1)
    {
        print_error(av);
        return;
    }
    close(fd);
}
void cat(int c, char **av) // cat concatena i file passati come argomenti e li stampa sullo STDOUT
{
    int i = 1;
    while (i < c)
        display_file(av[i++]);
}

int main(int c, char **v)
{
    if (c == 1)
        read_stdin();
    else
        cat(c, v);
}