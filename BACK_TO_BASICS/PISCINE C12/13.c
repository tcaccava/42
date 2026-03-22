typedef struct s_list
{
    void *data;
    struct s_list *next;
} t_list;

void ft_list_merge(t_list **begin_list1, t_list *begin_list2)
{
    t_list *tmp;

    if (!begin_list1)      // controllo che il doppio puntatore non sia NULL
        return;

    if (!*begin_list1) {   // lista1 vuota → la nuova lista diventa lista2
        *begin_list1 = begin_list2;
        return;
    }

    tmp = *begin_list1;    // tmp scorre la lista1
    while (tmp->next)      // scorre fino all'ultimo nodo
        tmp = tmp->next;

    tmp->next = begin_list2; // collega lista2 alla fine di lista1
}
