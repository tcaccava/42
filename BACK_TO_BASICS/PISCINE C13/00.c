#include "ft_btree.h"

// Un albero binario è una struttura dati gerarchica composta da nodi.
// Ogni nodo contiene un valore e ha al massimo due figli : sinistro e destro

t_btree *btree_create_node(void *item)
{
    t_btree *new_node = malloc(sizeof(t_btree));
    if (!new_node)
        return NULL;
    new_node->item = item;
    new_node->left = NULL;
    new_node->right = NULL;
    return new_node;
}