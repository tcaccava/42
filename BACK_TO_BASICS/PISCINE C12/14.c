typedef struct s_list
{
    void *data;
    struct s_list *next;
} t_list;

void ft_list_sort(t_list **begin_list, int (*cmp)())
{
    t_list **naive = begin_list;
    while (*naive && (*naive)->next)
    {
        if ((*naive)->next && ((cmp)((*naive)->data, (*naive)->next->data) > 0))
        {
            void *tmp = (*naive)->data;
            (*naive)->data = (*naive)->next->data;
            (*naive)->next->data = tmp;
            naive = begin_list;
            continue;
        }
        naive = &(*naive)->next;
    }
}