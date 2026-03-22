typedef struct s_list
{
    struct s_list *next;
    void *data;
} t_list;

t_list *ft_list_last(t_list *begin_list)
{
    if(!begin_list)
        return NULL;
    while (begin_list->next)// con questo while quando esci dal ciclo begin list punta all'ultimo nodo della lista
        begin_list = begin_list->next;// quindi ha iterato su tutti i nodi tranne l'ultimo
    return begin_list;
}