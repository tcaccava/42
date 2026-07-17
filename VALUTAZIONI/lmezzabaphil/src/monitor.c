/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lmezzaba <lmezzaba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/14 16:02:21 by lmezzaba          #+#    #+#             */
/*   Updated: 2026/07/15 10:56:03 by lmezzaba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	wake_all_coders(t_global_data *g)
{
	int	i;

	i = -1;
	while (++i < g->number_of_coders)
		pthread_cond_signal(&g->coders[i].cond);
}

static void	handle_requests(t_global_data *g, t_request *pending,
			int *pending_count)
{
	t_request	request;
	int			left_id;
	int			right_id;

	while (g->heap->size > 0)
	{
		heap_pop(g->heap, &request, g->scheduler);
		left_id = request.coder_id;
		right_id = (request.coder_id + 1) % g->number_of_coders;
		if (dongles_available(g, left_id, right_id))
		{
			g->dungles[left_id].is_in_use = 1;
			g->dungles[right_id].is_in_use = 1;
			g->coders[request.coder_id].has_dungles = 1;
			pthread_cond_signal(&g->coders[request.coder_id].cond);
		}
		else
			pending[(*pending_count)++] = request;
	}
}

static void	restore_pending(t_global_data *g, t_request *pending,
			int pending_count)
{
	int	i;

	i = -1;
	while (++i < pending_count)
		push_to_heap(g->heap, &pending[i], g->scheduler);
}

static int	monitor_cycle(t_global_data *g, t_request *pending)
{
	int	pending_count;
	int	expired;

	pthread_mutex_lock(&g->mutex);
	check_expired(g, &expired);
	if (g->stop_simulation || all_coders_done(g))
	{
		g->stop_simulation = 1;
		wake_all_coders(g);
		pthread_mutex_unlock(&g->mutex);
		return (1);
	}
	pending_count = 0;
	handle_requests(g, pending, &pending_count);
	restore_pending(g, pending, pending_count);
	pthread_mutex_unlock(&g->mutex);
	return (0);
}

void	*monitor_runtime(void *args)
{
	t_global_data	*g;
	t_request		*pending;

	g = (t_global_data *)args;
	pending = malloc(sizeof(t_request) * g->number_of_coders);
	if (!pending)
		return (NULL);
	while (!monitor_cycle(g, pending))
		;
	free(pending);
	return (NULL);
}
