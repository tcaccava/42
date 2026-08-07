/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gipimpin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/03 17:12:33 by gipimpin          #+#    #+#             */
/*   Updated: 2026/08/03 17:12:33 by gipimpin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	stop_routine(t_sim *sim, int i, char *msg)
{
	pthread_mutex_unlock(&sim->coders[i].coder_mutex);
	if (msg)
		log_status(&sim->coders[i], msg);
	set_sim_stop(sim, 1);
	return (1);
}

static int	check_coders(t_sim *sim)
{
	int		i;
	int		n_coder_done;
	long	time;

	i = 0;
	n_coder_done = 0;
	while (i < sim->n_coders)
	{
		pthread_mutex_lock(&sim->coders[i].coder_mutex);
		if (sim->coders[i].last_compile_time != 0)
		{
			time = get_current_time_ms() - sim->coders[i].last_compile_time;
			if (time > sim->time_to_burnout && !sim->coders[i].is_compiling)
				return (stop_routine(sim, i, "burned out"));
			if (sim->coders[i].compiles_done >= sim->n_compiles_required)
				n_coder_done++;
		}
		pthread_mutex_unlock(&sim->coders[i].coder_mutex);
		i++;
	}
	if (n_coder_done == sim->n_coders)
		return (set_sim_stop(sim, 1), 1);
	return (0);
}

static void	unblock_dongles(t_sim *sim)
{
	int		i;
	long	d_time_wait;

	i = 0;
	while (i < sim->n_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		d_time_wait = get_current_time_ms() - sim->dongles[i].last_used_time;
		if (sim->dongles[i].in_cooldown && d_time_wait >= sim->dongle_cooldown)
		{
			sim->dongles[i].in_cooldown = 0;
			pthread_cond_broadcast(&sim->dongles[i].cond);
		}
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		i++;
	}
}

static void	wake_all_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->n_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		i++;
	}
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (1)
	{
		if (check_coders(sim))
		{
			wake_all_dongles(sim);
			return (NULL);
		}
		unblock_dongles(sim);
		usleep(1000);
	}
	return (NULL);
}
