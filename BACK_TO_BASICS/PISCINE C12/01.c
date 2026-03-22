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
void ft_list_push_front(t_list **begin_list, void *data)
{
    t_list *new_node = ft_create_elem(data);
    new_node->next = *begin_list; // il nuovo nodo e' il primo della lista,quindi ha come next il puntatore alla testa della lista precedente
    *begin_list = new_node;       // la testa e' il nuovo nodo
}