/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pq_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gipimpin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 17:12:34 by gipimpin          #+#    #+#             */
/*   Updated: 2026/08/03 17:12:34 by gipimpin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_smaller(t_pq_node *a, t_pq_node *b)
{
	if (a->priority < b->priority)
		return (1);
	if (a->priority == b->priority && a->coder_id < b->coder_id)
		return (1);
	return (0);
}

void	swap_nodes(t_pq_node *node1, t_pq_node *node2)
{
	t_pq_node	tmp;

	tmp = *node1;
	*node1 = *node2;
	*node2 = tmp;
}

long	calculate_priority(t_coder *coder)
{
	long	priority;
	long	last_compile_time;

	pthread_mutex_lock(&coder->coder_mutex);
	last_compile_time = coder->last_compile_time;
	pthread_mutex_unlock(&coder->coder_mutex);
	if (coder->sim->scheduler == EDF)
		priority = coder->sim->time_to_burnout + last_compile_time;
	else
	{
		pthread_mutex_lock(&coder->sim->monitor_mutex);
		priority = coder->sim->fifo_counter++;
		pthread_mutex_unlock(&coder->sim->monitor_mutex);
	}
	return (priority);
}
