#include "ft_btree.h"

void btree_apply_prefix(t_btree *root, void (*applyf)(void *))
{
    if (!root)
        return;
    applyf(root->item);                      // prefix traversal o preorder traversal: prima esplora il nodo
    btree_apply_prefix(root->left, applyf);  // poi il ramo sinistro
    btree_apply_prefix(root->right, applyf); // poi quello destro
}