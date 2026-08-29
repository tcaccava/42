#include <stdlib.h>

typedef struct s_list
{
    void *data;          // Offset 0  (8 byte su x86-64)
    struct s_list *next; // Offset 8  (8 byte su x86-64)
} t_list;

void ft_list_push_front(t_list **begin_list, void *data)
{
    t_list *new_node;

    if (!begin_list)
        return;

    new_node = malloc(sizeof(t_list));
    if (!new_node)
        return;

    new_node->data = data;
    new_node->next = *begin_list;
    *begin_list = new_node;
}

int ft_list_size(t_list *begin_list)
{
    int i = 0;
    while (begin_list)
    {
        begin_list = begin_list->next;
        i++;
    }
    return i;
}

void ft_list_sort(t_list **begin_list, int (*cmp)())
{
    if (!begin_list || !*begin_list)
        return;
    void *tmp = 0;
    t_list *curr = *begin_list;
    while (curr->next)
    {
        if (cmp((curr)->data, (curr)->next->data) > 0)
        {
            tmp = curr->data;
            curr->data = curr->next->data;
            curr->next->data = tmp;
            curr = *begin_list;
        }
        else
            curr = curr->next;
    }
}

void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *))
{
    if (!begin_list || !*begin_list)
        return;

    t_list *curr = *begin_list;
    t_list *top = curr;
    t_list *previous = NULL;
    t_list *next;

    while (curr)
    {
        next = curr->next;
        if (!(cmp(curr->data, data_ref)))
        {
            free_fct(curr->data);
            if (curr == top)
            {
                top = next;
                previous = NULL;
            }
            else
                previous->next = next;

            free(curr); // <-- FIX: Liberazione del nodo t_list
        }
        else
            previous = curr;

        curr = next;
    }
    *begin_list = top;
}

void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *))
{
    if (!begin_list || !*begin_list)
        return;

    t_list **curr = begin_list;

    while (*curr)
    {
        if (cmp((*curr)->data, data_ref) == 0)
        {
            t_list *tmp = *curr;
            *curr = (*curr)->next; // Ricollega automaticamente la testa o il nodo precedente!
            free_fct(tmp->data);
            free(tmp);
        }
        else
        {
            curr = &(*curr)->next; // Avanza il puntatore al prossimo next
        }
    }
}