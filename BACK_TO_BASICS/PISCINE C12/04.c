#include <stdlib.h>

typedef struct s_list
{
    struct s_list *next;
    void *data;
} t_list;

t_list *ft_create_elem(void *data)
{
    t_list *new_node = malloc(sizeof(t_list));
    if (!new_node)
        return NULL;
    new_node->data = data;
    new_node->next = NULL;
    return new_node;
}

void ft_list_push_back(t_list **begin_list, void *data)
{
    t_list *tmp;
    t_list *new_node;

    if (!begin_list)
        return;
    new_node = ft_create_elem(data);
    if (!new_node)
        return;
    if (*begin_list == NULL)
    {
        *begin_list = new_node;
        return;
    }
    tmp = *begin_list;//sempre meglio usare una variabile temp per scorrere la lista,cosi da non modificare la head
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new_node;
}
