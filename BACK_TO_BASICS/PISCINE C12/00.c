#include <stdlib.h>
//una linked list e' una struttura dati dinamica composta da noi,ognuno dei quali contiene
// un dato e un puntatore al nodo successivo della lista
//a differenza degli array,gli elementi della lista non sono contigui in memoria
typedef struct s_list
{
    struct s_list *next;
    void *data;
} t_list;

t_list *ft_create_elem(void *data){
    t_list *node = malloc(sizeof(t_list));
    if(!node)
        return NULL;
    node->data = data;
    node->next = NULL;//e' a fine lista
    return node;
}