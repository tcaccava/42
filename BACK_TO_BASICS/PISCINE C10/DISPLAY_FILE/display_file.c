#include "display_file.h"

void display_file(char *av)
{
    int fd = open(av, O_RDONLY); // open,primo parametro nome file,secondo modalita
    // O_RDONLY, O_WRONLY, O_RDWR; ritorna un fd, o -1 se fallisce
    if (fd == -1)
    {
        write(1, "Cannot read file.\n", 19);
        return;
    }
    char buf[4096];
    int bytes_read = 0;
    while ((bytes_read = read(fd, buf, sizeof(buf))) > 0) // doppie parentesi per fare in modo che l'assegnazione di bytes read preceda il confronto con zero
        write(1, buf, bytes_read);                        // read,primo parametro fd da leggere,secondo buffer in cui salvare,
    close(fd);                                            // terzo numero massimo di byte da leggere ad ogni chiamata
} // ritorna il numero di byte ssize_t letti,zero se EOF,-1 se errore;
  // write legge tutto il buffer di bytesread letti durante l'ultima chiamata,che quindi
  // sovrascrive solo i primi bytesread del buffer: se bytesread < sizeofbuf,il resto del
  // buffer rimane invariato,con i dati vecchi delle precedenti chiamate

// Ogni file descriptor mantiene un offset interno nel kernel, che indica la posizione corrente nel file.
// Quando chiami read(fd, buf, size):
// Il kernel legge i byte a partire dall’offset corrente.
// Aggiorna l’offset automaticamente, spostandolo di n byte letti.
// La prossima chiamata a read sullo stesso file descriptor continua da dove era rimasta, senza che tu debba passare nulla.

int main(int c, char **v)
{
    if (c == 1)
    {
        write(1, "File name missing.", 19);
        return 1;
    }
    if (c > 2)
    {
        write(1, "Too many arguments.", 20);
        return 1;
    }
    display_file(v[1]);
}