#include "ft_btree.h"

#include <stdlib.h>

typedef struct s_btree
{
    void            *item;
    struct s_btree  *left;
    struct s_btree  *right;
}   t_btree;

// Nodo per la coda BFS
typedef struct s_queue_node
{
    t_btree *node;
    int level;
    struct s_queue_node *next;
} t_queue_node;

// Coda
typedef struct s_queue
{
    t_queue_node *front;
    t_queue_node *rear;
} t_queue;

// Funzioni della coda
t_queue *queue_create()
{
    t_queue *q = malloc(sizeof(t_queue));
    q->front = q->rear = NULL;
    return q;
}

void queue_push(t_queue *q, t_btree *node, int level)
{
    t_queue_node *n = malloc(sizeof(t_queue_node));
    n->node = node;
    n->level = level;
    n->next = NULL;
    if (!q->rear)
        q->front = q->rear = n;
    else
    {
        q->rear->next = n;
        q->rear = n;
    }
}

t_queue_node *queue_pop(t_queue *q)
{
    if (!q->front)
        return NULL;
    t_queue_node *tmp = q->front;
    q->front = q->front->next;
    if (!q->front)
        q->rear = NULL;
    return tmp;
}

int queue_empty(t_queue *q)
{
    return q->front == NULL;
}

// Funzione principale
void btree_apply_by_level(t_btree *root, void (*applyf)(void*, int, int))
{
    if (!root)
        return;

    t_queue *q = queue_create();
    queue_push(q, root, 0);

    int current_level = 0;
    int first = 1;

    while (!queue_empty(q))
    {
        t_queue_node *qn = queue_pop(q);
        t_btree *node = qn->node;
        int level = qn->level;
        free(qn);

        // Se siamo in un nuovo livello, resettiamo first
        if (level != current_level)
        {
            current_level = level;
            first = 1;
        }

        applyf(node->item, level, first);
        first = 0; // solo il primo nodo del livello deve avere first=1

        if (node->left)
            queue_push(q, node->left, level + 1);
        if (node->right)
            queue_push(q, node->right, level + 1);
    }

    free(q);
}
