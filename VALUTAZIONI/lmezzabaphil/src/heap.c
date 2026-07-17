/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmezzaba <lmezzaba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 16:02:35 by lmezzaba          #+#    #+#             */
/*   Updated: 2026/07/14 18:34:05 by lmezzaba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_higher_priority(t_request *a, t_request *b,
		t_scheduler scheduler)
{
	if (scheduler == FIFO)
	{
		if (a->sended_time != b->sended_time)
			return (a->sended_time < b->sended_time);
		return (a->coder_id < b->coder_id);
	}
	if (a->deadline != b->deadline)
		return (a->deadline < b->deadline);
	if (a->sended_time != b->sended_time)
		return (a->sended_time < b->sended_time);
	return (a->coder_id < b->coder_id);
}

int	push_to_heap(t_heap *heap, t_request *request,
		t_scheduler scheduler)
{
	int			i;
	int			parent;
	t_request	tmp;

	heap->requets[heap->size] = *request;
	i = heap->size;
	heap->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!is_higher_priority(&heap->requets[i],
				&heap->requets[parent], scheduler))
			break ;
		tmp = heap->requets[i];
		heap->requets[i] = heap->requets[parent];
		heap->requets[parent] = tmp;
		i = parent;
	}
	return (0);
}

static void	heapify_down(t_heap *heap, int i, t_scheduler scheduler)
{
	int			left;
	int			right;
	int			smallest;
	t_request	tmp;

	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < heap->size
			&& is_higher_priority(&heap->requets[left],
				&heap->requets[smallest], scheduler))
			smallest = left;
		if (right < heap->size
			&& is_higher_priority(&heap->requets[right],
				&heap->requets[smallest], scheduler))
			smallest = right;
		if (smallest == i)
			break ;
		tmp = heap->requets[i];
		heap->requets[i] = heap->requets[smallest];
		heap->requets[smallest] = tmp;
		i = smallest;
	}
}

int	heap_pop(t_heap *heap, t_request *out, t_scheduler scheduler)
{
	if (heap->size == 0)
		return (-1);
	*out = heap->requets[0];
	heap->size--;
	heap->requets[0] = heap->requets[heap->size];
	heapify_down(heap, 0, scheduler);
	return (0);
}
