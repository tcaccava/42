*This project has been created as part of the 42 curriculum by ggiument.*

# Libft

La mia prima libreria personale in C, scritta da zero durante il percorso a 42 Roma.

---

## Descrizione

Libft è una raccolta di funzioni in C che riproducono (e in alcuni casi estendono) quelle della libreria standard. L'obiettivo è capire davvero come funzionano queste funzioni, invece di usarle come scatole nere.

La libreria è divisa in tre parti:
- **Parte 1** — reimplementazione di funzioni della libc (es. `strlen`, `memcpy`, `atoi`...)
- **Parte 2** — funzioni aggiuntive non presenti nella libc standard (es. `ft_split`, `ft_itoa`...)
- **Parte 3 (bonus)** — funzioni per manipolare liste collegate (`t_list`)

---

## Istruzioni

### Compilare la libreria
```bash
make
```

### Compilare con i bonus
```bash
make bonus
```

### Pulire i file oggetto
```bash
make clean
```

### Rimuovere tutto (inclusa la libreria)
```bash
make fclean
```

### Ricompilare da zero
```bash
make re
```

Una volta compilata, viene generato il file `libft.a` nella root del repository. Per usarla in un progetto:
```bash
cc main.c -L. -lft -o programma
```

---

## Funzioni implementate

### Parte 1 — Funzioni libc

| Funzione | Descrizione |
|---|---|
| `ft_isalpha` | Controlla se il carattere è alfabetico |
| `ft_isdigit` | Controlla se il carattere è una cifra |
| `ft_isalnum` | Controlla se il carattere è alfanumerico |
| `ft_isascii` | Controlla se il carattere è ASCII |
| `ft_isprint` | Controlla se il carattere è stampabile |
| `ft_strlen` | Restituisce la lunghezza di una stringa |
| `ft_memset` | Riempie un'area di memoria con un valore |
| `ft_bzero` | Azzera un'area di memoria |
| `ft_memcpy` | Copia un'area di memoria |
| `ft_memmove` | Copia un'area di memoria gestendo le sovrapposizioni |
| `ft_strlcpy` | Copia una stringa con limite di dimensione |
| `ft_strlcat` | Concatena due stringhe con limite di dimensione |
| `ft_toupper` | Converte un carattere in maiuscolo |
| `ft_tolower` | Converte un carattere in minuscolo |
| `ft_strchr` | Cerca un carattere in una stringa (da sinistra) |
| `ft_strrchr` | Cerca un carattere in una stringa (da destra) |
| `ft_strncmp` | Confronta due stringhe fino a n caratteri |
| `ft_memchr` | Cerca un byte in un'area di memoria |
| `ft_memcmp` | Confronta due aree di memoria |
| `ft_strnstr` | Cerca una sottostringa in una stringa con limite |
| `ft_atoi` | Converte una stringa in intero |
| `ft_calloc` | Alloca memoria inizializzata a zero |
| `ft_strdup` | Duplica una stringa allocando nuova memoria |

### Parte 2 — Funzioni aggiuntive

| Funzione | Descrizione |
|---|---|
| `ft_substr` | Estrae una sottostringa da una stringa |
| `ft_strjoin` | Concatena due stringhe in una nuova |
| `ft_strtrim` | Rimuove caratteri da inizio e fine di una stringa |
| `ft_split` | Divide una stringa in array usando un delimitatore |
| `ft_itoa` | Converte un intero in stringa |
| `ft_strmapi` | Applica una funzione a ogni carattere, restituisce nuova stringa |
| `ft_striteri` | Applica una funzione a ogni carattere (modifica in-place) |
| `ft_putchar_fd` | Scrive un carattere su un file descriptor |
| `ft_putstr_fd` | Scrive una stringa su un file descriptor |
| `ft_putendl_fd` | Scrive una stringa seguita da newline su un file descriptor |
| `ft_putnbr_fd` | Scrive un intero su un file descriptor |

### Parte 3 — Liste collegate (bonus)

| Funzione | Descrizione |
|---|---|
| `ft_lstnew` | Crea un nuovo nodo |
| `ft_lstadd_front` | Aggiunge un nodo all'inizio della lista |
| `ft_lstsize` | Conta i nodi della lista |
| `ft_lstlast` | Restituisce l'ultimo nodo |
| `ft_lstadd_back` | Aggiunge un nodo alla fine della lista |
| `ft_lstdelone` | Elimina un singolo nodo |
| `ft_lstclear` | Elimina tutta la lista |
| `ft_lstiter` | Applica una funzione a ogni nodo |
| `ft_lstmap` | Crea una nuova lista applicando una funzione a ogni nodo |

---

## Risorse

- [Man pages Linux](https://man7.org/linux/man-pages/) — per capire il comportamento atteso di ogni funzione
- [Beej's Guide to C](https://beej.us/guide/bgc/) — guida pratica al C
- [42 Docs](https://harm-smits.github.io/42docs/projects/libft) — note di altri studenti su libft

### Uso dell'intelligenza artificiale

Ho usato Claude (Anthropic) come strumento di supporto all'apprendimento, non per generare codice.

In pratica: quando incontravo un bug o non capivo un comportamento, chiedevo aiuto tramite domande. L'AI mi guidava con domande a sua volta, portandomi a ragionare da solo sul problema fino a trovare la soluzione. Non mi ha mai scritto codice direttamente.

Esempi concreti:
- Ho trovato un bug off-by-one in `ft_strtrim` ragionando sulla differenza tra indice e lunghezza
- Ho capito perché `ft_split` causava un invalid read in valgrind, tracciando passo per passo cosa succedeva con `i = 0` e un tipo `size_t`

Questo approccio mi ha aiutato a capire davvero i problemi, invece di copiarmi una soluzione che non avrei saputo spiegare.
