typedef struct s_list
{
    struct s_list *next;
    void *data;
} t_list;

void ft_list_reverse(t_list **begin_list)
{
    t_list *prev = NULL;
    t_list *curr = *begin_list;
    while (curr)
    {
        t_list *next = curr->next; // salvi next in una temporanea
        curr->next = prev;         // inverti il nodo,ora next invece di puntare al successivo,punta al precedente
        prev = curr;               // sposti prev avanti
        curr = next;               // sposti curr avanti
    }
    *begin_list = prev;
}