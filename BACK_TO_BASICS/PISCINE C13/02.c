#include "ft_btree.h"

void btree_apply_infix(t_btree *root, void (*applyf)(void *))
{
    if (!root)
        return;
    btree_apply_infix(root->left, applyf);  // infix traversal o in order traversal: prima il ramo sx
    applyf(root->item);                     // poi il nodo
    btree_apply_infix(root->right, applyf); // poi il ramo dx
}