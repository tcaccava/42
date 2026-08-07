/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pq.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gipimpin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 17:12:33 by gipimpin          #+#    #+#             */
/*   Updated: 2026/08/03 17:12:33 by gipimpin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_pq_node	pq_pop(t_pq *pq)
{
	int			i;
	int			left;
	int			right;
	int			small;
	t_pq_node	res;

	if (!pq || pq->size == 0)
		return ((t_pq_node){0});
	small = 0;
	res = pq->heap[0];
	pq->heap[0] = pq->heap[--pq->size];
	while (1)
	{
		i = small;
		left = (2 * i) + 1;
		right = (2 * i) + 2;
		if (left < pq->size && is_smaller(&pq->heap[left], &pq->heap[small]))
			small = left;
		if (right < pq->size && is_smaller(&pq->heap[right], &pq->heap[small]))
			small = right;
		if (small == i)
			break ;
		swap_nodes(&pq->heap[i], &pq->heap[small]);
	}
	return (res);
}

int	pq_push(t_pq *pq, int coder_id, long priority)
{
	int	i;
	int	parent;

	if (!pq)
		return (0);
	if (coder_id <= 0 || priority < 0 || pq->size >= pq->capacity)
		return (putstr_fd(2, "Error in pq_push.\n"), 0);
	i = pq->size;
	pq->heap[i].coder_id = coder_id;
	pq->heap[i].priority = priority;
	pq->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (is_smaller(&pq->heap[i], &pq->heap[parent]))
		{
			swap_nodes(&pq->heap[parent], &pq->heap[i]);
			i = parent;
		}
		else
			break ;
	}
	return (1);
}

t_pq	*pq_create(int capacity)
{
	t_pq	*pq;

	if (capacity <= 0)
		return (NULL);
	pq = (t_pq *) ft_calloc(1, sizeof(t_pq));
	if (!pq)
		return (NULL);
	pq->size = 0;
	pq->capacity = capacity;
	pq->heap = (t_pq_node *) ft_calloc (capacity, sizeof(t_pq_node));
	if (!pq->heap)
		return (free(pq), NULL);
	return (pq);
}

void	pq_destroy(t_pq *pq)
{
	if (!pq)
		return ;
	if (pq->heap)
		free(pq->heap);
	free(pq);
}
