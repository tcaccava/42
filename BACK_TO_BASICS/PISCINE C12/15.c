#include <stdlib.h>

typedef struct s_list
{
    void *data;
    struct s_list *next;
} t_list;

void ft_list_reverse_fun(t_list *begin_list)
{
    int size = 0;
    t_list *tmp = begin_list;
    t_list **arr;
    int i;

    // 1. Conta i nodi
    while (tmp)
    {
        size++;
        tmp = tmp->next;
    }

    if (size < 2) // niente da fare se 0 o 1 nodo
        return;

    // 2. Metti tutti i puntatori ai nodi in un array
    arr = malloc(sizeof(t_list*) * size);
    if (!arr)
        return; // errore malloc

    tmp = begin_list;
    for (i = 0; i < size; i++)
    {
        arr[i] = tmp;
        tmp = tmp->next;
    }

    // 3. Scambia i dati
    for (i = 0; i < size / 2; i++)
    {
        void *t = arr[i]->data;
        arr[i]->data = arr[size - 1 - i]->data;
        arr[size - 1 - i]->data = t;
    }

    free(arr);
}
