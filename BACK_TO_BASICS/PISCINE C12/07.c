typedef struct s_list
{
    void *data;
    struct s_list *next;
} t_list;

t_list *ft_list_at(t_list *begin_list, unsigned int nbr)
{
    unsigned int index = 0;
    while (begin_list)
    {
        if (nbr == index)
            return begin_list;
        begin_list = begin_list->next;
        index++;
    }
    return NULL;
}