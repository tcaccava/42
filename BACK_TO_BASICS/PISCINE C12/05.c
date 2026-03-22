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

t_list *ft_list_push_strs(int size, char **strs)
{
    t_list *head = NULL;//partiamo da una lista vuota
    int i = size - 1;
    while (i >= 0)
    {
        t_list *new_node = ft_create_elem(strs[i]);
        if(!new_node)
            return NULL;
        new_node->next = head;
        head = new_node;
        i--;
    }
    return head;
}