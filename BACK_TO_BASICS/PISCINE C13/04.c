#include "ft_btree.h"

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

void btree_insert_data(t_btree **root, void *item, int (*cmpf)(void *, void *))
{
    if (!*root) // condizione base: scendi finche' non trovi NULL,quindi inserisci il nodo
    {
        btree_create_node(item);
        return;
    }

    if (cmpf((*root)->item, item) < 0)                 // il nodo gia' presente e' inferiore a quello da aggiungere
        btree_insert_data(&(*root)->left, item, cmpf); // vai a sinistra
    else
        btree_insert_data(&(*root)->right, item, cmpf); // vai a destra
}

// oppure iterativo
//  void btree_insert_data(t_btree **root, void *item,
//                         int (*cmpf)(void *, void *))
//  {
//      t_btree *current;

//     if (!root)
//         return;
//     if (!*root)
//     {
//         *root = btree_create_node(item);
//         return;
//     }
//     current = *root;
//     while (1)
//     {
//         if (cmpf(item, current->item) < 0)
//         {
//             if (!current->left)
//             {
//                 current->left = btree_create_node(item);
//                 return;
//             }
//             current = current->left;
//         }
//         else
//         {
//             if (!current->right)
//             {
//                 current->right = btree_create_node(item);
//                 return;
//             }
//             current = current->right;
//         }
//     }
// }
