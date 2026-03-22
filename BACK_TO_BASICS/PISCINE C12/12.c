#include <stdlib.h>

typedef struct s_list
{
    void *data;
    struct s_list *next;
} t_list;

void ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(), void (*free_fct)(void *))
{
    while (*begin_list)
    {
        if (!cmp((*begin_list)->data, data_ref))
        { // come vedi -> "toglie" un solo livello di dereferenziazione
            // per questo e' necessario prima dereferenziare begin list,poi con -> accedere a data
            t_list *tmp = *begin_list;         // salva il nodo da liberare
            *begin_list = (*begin_list)->next; // scollega il nodo
            free_fct(tmp->data);               // libera i dati
            free(tmp);                         // libera il nodo
        }
        else
            begin_list = &(*begin_list)->next;
    }
}