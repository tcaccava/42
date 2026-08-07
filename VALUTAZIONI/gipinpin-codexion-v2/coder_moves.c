/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_moves.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gipimpin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 17:12:33 by gipimpin          #+#    #+#             */
/*   Updated: 2026/08/03 17:12:33 by gipimpin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	wait_and_take(t_coder *coder, t_dongle *d)
{
	pthread_mutex_lock(&d->mutex);
	while (d->in_use || d->in_cooldown
		|| d->wait_queue->heap[0].coder_id != coder->id)
	{
		if (check_sim_stop(coder->sim))
		{
			pthread_mutex_unlock(&d->mutex);
			return (0);
		}
		pthread_cond_wait(&d->cond, &d->mutex);
	}
	d->in_use = 1;
	pq_pop(d->wait_queue);
	pthread_mutex_unlock(&d->mutex);
	log_status(coder, "has taken a dongle");
	return (1);
}

int	enqueue_target_dongle(t_coder *coder, int is_left, long priority)
{
	t_dongle	*d;

	if (is_left == 1)
		d = coder->left_dongle;
	else
		d = coder->right_dongle;
	pthread_mutex_lock(&d->mutex);
	if (!pq_push(d->wait_queue, coder->id, priority))
	{
		pthread_mutex_unlock(&d->mutex);
		set_sim_stop(coder->sim, 1);
		return (0);
	}
	pthread_mutex_unlock(&d->mutex);
	return (1);
}

int	acquire_both_dongles(t_coder *coder, t_dongle *d)
{
	long	priority;

	if (!(d && coder))
		return (0);
	priority = calculate_priority(coder);
	if (!enqueue_target_dongle(coder, 1, priority))
		return (0);
	if (coder->sim->n_coders == 1)
		return (wait_and_take(coder, coder->left_dongle));
	if (!enqueue_target_dongle(coder, 0, priority))
		return (0);
	if (!wait_and_take(coder, coder->left_dongle))
		return (0);
	if (!wait_and_take(coder, coder->right_dongle))
		return (release_dongle(coder->left_dongle, coder->sim), 0);
	return (1);
}

void	release_dongle(t_dongle *dongle, t_sim *sim)
{
	if (!dongle)
		return ;
	pthread_mutex_lock(&dongle->mutex);
	dongle->last_used_time = get_current_time_ms();
	if (sim->dongle_cooldown > 0)
		dongle->in_cooldown = 1;
	else
		dongle->in_cooldown = 0;
	dongle->in_use = 0;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}
