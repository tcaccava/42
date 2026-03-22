typedef struct s_list
{
    void *data;
    struct s_list *next;
} t_list;

void ft_list_foreach_if(t_list *begin_list, void (*f)(void *), void *data_ref, int (*cmp)())
{
    while (begin_list)
    {
        if (!cmp(begin_list->data, data_ref))
            f(begin_list->data);
        begin_list = begin_list->next;
    }
}