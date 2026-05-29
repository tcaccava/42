/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abecucci <abecucci@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/02 19:45:22 by abecucci          #+#    #+#             */
/*   Updated: 2026/03/03 14:39:53 by abecucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

uint64_t	time_now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	print_state(t_philo *p, char *state)
{
	uint64_t	ts;

	if (should_stop(p->sh))
		return ;
	ts = time_now_ms() - p->sh->start_time;
	pthread_mutex_lock(&p->sh->print_mtx);
	if (should_stop(p->sh))
	{
		pthread_mutex_unlock(&p->sh->print_mtx);
		return ;
	}
	printf("%" PRIu64 " %d %s\n", ts, p->id, state);
	pthread_mutex_unlock(&p->sh->print_mtx);
}

t_bool	should_stop(t_shared *sh)
{
	t_bool	stop;

	pthread_mutex_lock(&sh->stop_mtx);
	stop = sh->stop;
	pthread_mutex_unlock(&sh->stop_mtx);
	return (stop);
}

void	raise_stop(t_shared *sh)
{
	pthread_mutex_lock(&sh->stop_mtx);
	sh->stop = True;
	pthread_mutex_unlock(&sh->stop_mtx);
}

t_bool	ft_msleep(t_shared *sh, uint64_t sleep_time_ms)
{
	uint64_t	start;

	start = time_now_ms();
	while (!should_stop(sh) && (time_now_ms() - start) < sleep_time_ms)
		usleep(500);
	if (time_now_ms() - start < sleep_time_ms)
		return (False);
	return (True);
}
