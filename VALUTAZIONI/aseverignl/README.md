*This project has been created as part of the 42 curriculum by aseveri.*

## Description

`get_next_line` è una funzione C che legge e restituisce una riga alla volta da un file descriptor. Ogni chiamata successiva restituisce la riga successiva, fino a quando non ci sono più dati da leggere o si verifica un errore. La funzione supporta qualsiasi file descriptor valido, incluso lo standard input.

Il progetto introduce il concetto di **variabili statiche** in C — variabili che mantengono il loro valore tra chiamate successive alla stessa funzione — fondamentali per implementare la lettura incrementale senza rileggere dati già processati.

## Instructions

```bash
# Compilare con BUFFER_SIZE personalizzato
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 -c get_next_line.c get_next_line_utils.c

# Compilare senza flag (usa il valore di default 42 definito nell'header)
cc -Wall -Wextra -Werror -c get_next_line.c get_next_line_utils.c
```

Per usare la funzione in un progetto:
```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd;
    char    *line;

    fd = open("file.txt", O_RDONLY);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

```bash
# compilare tutto insieme:
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
 
# oppure se i file oggetto sono già compilati:
cc get_next_line.o get_next_line_utils.o main.c -o gnl
```

### Bonus

La versione bonus supporta più file descriptor contemporaneamente con una sola variabile statica:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c
```

## Algorithm

### Principio fondamentale

L'algoritmo si basa su un **buffer statico riutilizzato in-place** — a differenza dell'approccio classico che accumula i dati letti in una stringa separata con `strjoin` ad ogni iterazione, qui il buffer di lettura è anche il buffer residuo. Questo elimina le riallocazioni intermedie nella maggior parte dei casi.

### Struttura del buffer

Il buffer statico ha dimensione `BUFFER_SIZE + 1`. Ogni chiamata a `read()` sovrascrive i primi `n_byte` byte del buffer con i nuovi dati, e `buffer[n_byte] = '\0'` tronca il vecchio contenuto. Il buffer non viene mai azzerato tra una lettura e l'altra — questo è intenzionale e corretto perché ogni `read()` sovrascrive esattamente i byte necessari.

### Ciclo di vita di una chiamata

**Fase 1 — Gestione del residuo (funzione `init_gnl`):**
Se il buffer statico è NULL viene allocato e la funzione `init_gnl` restituisce `0` e inizializza `*str` a NULL in caso di esito positivo e l'esecuzione prosegue, `-1` in caso di fallimento e GNL restituirà `*str` inizializzato a NULL.
Altrimenti, essendo il buffer statico non-NULL, contiene dati di una lettura precedente. Si cerca in esso il primo `\n`:

- **Il carattere successivo al `\n` è `'\0'`:** il buffer può essere sovrascritto con una nuova lettura non essendoci perdita di dati, `init_gnl` restituisce `0` e l'esecuzione prosegue.
- **Il carattere successivo al `\n` è non-`'\0'`:** contiene già dati da restituire: se è presente un solo `\n` nel residuo, `init_gnl` alloca la stringa dal primo `\n` fino al `'\0'` del buffer e restituisce `0` con la stringa già popolata — l'esecuzione prosegue nel loop di lettura per accumulare il resto.
  Se invece ci sono più `\n` nel residuo, il primo viene sostituito con `'a'` (un carattere arbitrario non-`'\0'`) per "nasconderlo" senza rompere la stringa, e il contenuto fino al secondo `\n` viene restituito come riga e `init_gnl` restituisce `1` e quindi GNL la stringa allocata. In entrambi i casi, in caso di fallimento dell'allocazione `init_gnl` restituisce `-1` e GNL restituisce NULL.

Quando la funzione restituisce `-1` internamente verrà effettuato il free del buffer per evitare leak.

**Fase 2 — Loop di lettura:**
Ad ogni iterazione si legge un blocco di `BUFFER_SIZE` byte nel buffer e si imposta `buffer[n_byte] = '\0'` per troncare il vecchio contenuto. Se `read()` restituisce `-1`, si liberano buffer e str e GNL restituisce NULL.Se restituisce `0` (EOF), si libera il buffer impostandolo a NULL e si restituisce `str` — che può essere NULL per esempio se il file era vuoto, oppure contenere i dati accumulati se si trattava dell'ultima riga senza `\n`. Se invece la lettura ha prodotto dati, il comportamento dipende dallo stato di `str`:

- **`str` è NULL (`read_no_str`):** si cerca `\n` nel buffer — se non c'è si fa `ft_strdup` del buffer in `str` e `read_no_str` restituisce `0`, se c'è si alloca esattamente i byte necessari: si copia fino al `\n` incluso e `read_no_str` restituisce `1`.
- **`str` è già popolata (`read_with_str`):** si cerca `\n` nel buffer — se non c'è si fa `ft_strjoin(str, buffer)`, si   libera la vecchia `str` e `read_with_str` restituisce `0`, se c'è si salva temporaneamente il carattere dopo il `\n`, si imposta `'\0'` per delimitare il join, si fa `ft_strjoin(str, buffer)`, si ripristina il carattere salvato, si libera la vecchia `str` e `read_with_str` restituisce `1`.

In entrambe le funzioni in caso di fallimento dell'allocazione si libera il buffer e si restituisce `-1`. GNL a seconda del valore di ritorno: se `0` continua il loop di lettura, se `1` restituisce `str`, se `-1` restituisce NULL.

### Gestione della memoria

Il buffer statico viene allocato una volta sola alla prima chiamata e liberato solo in caso di errore o EOF. La stringa di output viene allocata con la dimensione esatta necessaria tramite aritmetica dei puntatori, evitando allocazioni eccessive indipendentemente dal valore di `BUFFER_SIZE`.

### Versione bonus — multiple file descriptor

La versione bonus sostituisce la singola variabile statica con un array `buffer[OPEN_MAX]` indicizzato per `fd`. Ogni file descriptor mantiene il proprio stato di lettura indipendentemente dagli altri, permettendo letture intercalate su più file senza perdita di dati.

## Resources

- [Linux Man Pages — read](https://man7.org/linux/man-pages/man2/read.2.html) — comportamento di `read()`, gestione di EOF e errori
- [Linux Man Pages — open](https://man7.org/linux/man-pages/man2/open.2.html) — file descriptor e flags
- [C static variables](https://en.cppreference.com/w/c/language/storage_duration) — documentazione sulle variabili statiche in C
- [42 Norm](https://github.com/42School/norminette) — norminette per il rispetto delle regole di stile


