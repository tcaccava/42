#include <stdlib.h>

typedef struct s_list
{
    struct s_list *next;
    void *data;
} t_list;

t_list *ft_create_elem(void *data)
{
    t_list *node = malloc(sizeof(t_list));
    if (!node)
        return NULL;
    node->data = data;
    node->next = NULL;
    return node;
}

void ft_sorted_list_insert(t_list **begin_list, void *data, int (*cmp)())
{
    t_list *new_node = ft_create_elem(data);
    t_list **tmp = begin_list;
    t_list *prev = NULL;
    while (*tmp)
    {
        if (cmp((*tmp)->data, new_node->data) > 0)
        {
            if (prev)
                prev->next = new_node;
            else
                *begin_list = new_node; // se il nuovo nodo e' piu piccolo del primo elemento della lista,la testa della lista va aggiornata al nuovo nodo
            new_node->next = *tmp;
            return; // esci subito dopo l'inserimento
        }
        prev = *tmp;
        tmp = &(*tmp)->next;
    }
    // se siamo arrivati alla fine senza alcun inserimento,lo inseriamo in coda
    if (prev)
        prev->next = new_node;
    else
        *begin_list = new_node;
}