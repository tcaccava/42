#include <stdlib.h>

typedef struct s_list
{
    void *data;
    struct s_list *next;
} t_list;

void ft_list_clear(t_list *begin_list, void (*free_fct)(void *))
{
    while (begin_list)// *
    {
        t_list *tmp = begin_list->next;
        free_fct(begin_list->data);
        free(begin_list);
        begin_list = tmp;
    }
}

// *
///*
// ** DIFFERENZA TRA STRINGHE E LISTE NEL CICLO WHILE:
// **
// ** 1. STRINGA (char *str):
// ** while (*str) scorre i VALORI. Si ferma quando trova il carattere '\0' (valore 0).
// ** Ogni str++ sposta il puntatore al byte successivo in memoria.
// **
// ** 2. LISTA (t_list *list):
// ** while (list) scorre gli INDIRIZZI. Si ferma quando il puntatore è NULL (indirizzo 0).
// ** NON si usa while (*list) perché *list rappresenta il contenuto del nodo (la struct),
// ** non l'indirizzo del nodo successivo.
// **
// ** 3. AVANZAMENTO:
// ** Nelle liste non esiste l'aritmetica dei puntatori (list++ non funziona).
// ** L'unico modo per "camminare" è saltare da un indirizzo all'altro
// ** usando il puntatore salvato nel nodo: list = list->next.
// */