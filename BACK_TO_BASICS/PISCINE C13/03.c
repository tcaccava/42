#include "ft_btree.h"

void btree_apply_suffix(t_btree *root, void (*applyf)(void *))
{
    if (!root)
        return;
    btree_apply_suffix(root->left, applyf);  // suffix traversal o postorder traversal o postfix traversal: prima il ramo sx
    btree_apply_suffix(root->right, applyf); // poi quello destro
    applyf(root->item);                      // poi il nodo
}