#include <stdlib.h>
#include <stdio.h>

typedef struct s_list
{
    struct s_list *next;
    void *data;
} t_list;

int ft_cmp_int(int *a, int *b)
{
    return (*a - *b);
}

// Crea un nuovo nodo
t_list *ft_create_elem(void *data)
{
    t_list *new = malloc(sizeof(t_list));
    if (!new)
        return (NULL);
    new->data = data;
    new->next = NULL;
    return (new);
}

// Stampa la lista
void print_list(t_list *list)
{
    while (list)
    {
        printf("%d -> ", *(int *)list->data);
        list = list->next;
    }
    printf("NULL\n");
}

// riuso la funzione di 14.c
void ft_list_sort(t_list **begin_list, int (*cmp)())
{
    t_list **naive = begin_list;
    while (*naive && (*naive)->next)
    {
        if ((cmp)((*naive)->data, (*naive)->next->data) > 0)
        {
            void *tmp = (*naive)->data; // classico swap,lasci i nodi immutati ma fai sqwap dei dati
            (*naive)->data = (*naive)->next->data;
            (*naive)->next->data = tmp;
            naive = begin_list;
            continue;
        }
        naive = &(*naive)->next;
    }
}

void ft_sorted_list_merge(t_list **begin_list1, t_list *begin_list2, int (*cmp)())
{
    t_list *curr;

    if (!begin_list2)
        return;

    // 1. Se la prima lista è vuota, diventa la seconda
    if (!*begin_list1)
    {
        *begin_list1 = begin_list2;
    }
    else
    {
        // 2. Altrimenti, vai in fondo alla prima lista e attacca la seconda
        curr = *begin_list1;
        while (curr->next)
            curr = curr->next;
        curr->next = begin_list2;
    }

    // 3. Ordina tutto
    ft_list_sort(begin_list1, cmp);
}
// in sostanza prima unisco la lista 2 alla 1,poi riuso la funzione di sorting per sortare il tutto

int main(void)
{
    // Creazione Lista 1: 10 -> 20 -> 30
    int v1 = 10, v2 = 20, v3 = 30;
    t_list *l1 = ft_create_elem(&v1);
    l1->next = ft_create_elem(&v2);
    l1->next->next = ft_create_elem(&v3);

    // Creazione Lista 2: 5 -> 15 -> 25
    int v4 = 5, v5 = 15, v6 = 25;
    t_list *l2 = ft_create_elem(&v4);
    l2->next = ft_create_elem(&v5);
    l2->next->next = ft_create_elem(&v6);

    printf("Lista 1: ");
    print_list(l1);
    printf("Lista 2: ");
    print_list(l2);

    printf("\nEseguo ft_sorted_list_merge...\n");
    ft_sorted_list_merge(&l1, l2, ft_cmp_int);

    printf("Risultato: ");
    print_list(l1);

    return (0);
}