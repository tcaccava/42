#include "ft_btree.h"

void *btree_search_item(t_btree *root, void *data_ref, int (*cmpf)(void *, void *))
{
    void *res;
    if (!root)
        return NULL;
    res = btree_search_item(root->left, data_ref, cmpf);
    if (res)
        return res;
    if (cmpf(root->item, data_ref) == 0)
        return root->item; // root->item e' un void *,quindi compatibile con il ritorno della funzione
    return btree_search_item(root->right, data_ref, cmpf);
}