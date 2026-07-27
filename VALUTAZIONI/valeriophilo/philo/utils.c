/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valucare <valucare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 14:14:41 by valucare          #+#    #+#             */
/*   Updated: 2026/07/27 17:21:28 by valucare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000L) + (tv.tv_usec / 1000));
}

int	is_stopped(t_table *table)
{
	int	flag;

	pthread_mutex_lock(&table->stop_mtx);
	flag = table->stopped;
	pthread_mutex_unlock(&table->stop_mtx);
	return (flag);
}

void	stop_sim(t_table *table)
{
	pthread_mutex_lock(&table->stop_mtx);
	table->stopped = 1;
	pthread_mutex_unlock(&table->stop_mtx);
}

void	wait_ms(t_table *table, long delay)
{
	long	deadline;

	deadline = now_ms() + delay;
	while (now_ms() < deadline)
	{
		if (is_stopped(table))
			return ;
		usleep(POLL_US);
	}
}

void	log_state(t_philo *philo, char *state)
{
	t_table	*table;

	table = philo->table;
	pthread_mutex_lock(&table->write_mtx);
	pthread_mutex_lock(&table->stop_mtx);
	if (!table->stopped)
		printf("%ld %d %s\n", now_ms() - table->epoch, philo->rank, state);
	pthread_mutex_unlock(&table->stop_mtx);
	pthread_mutex_unlock(&table->write_mtx);
}
