/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valucare <valucare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 14:13:32 by valucare          #+#    #+#             */
/*   Updated: 2026/07/27 17:18:41 by valucare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	announce_death(t_table *table, int rank)
{
	pthread_mutex_lock(&table->write_mtx);
	pthread_mutex_lock(&table->stop_mtx);
	if (!table->stopped)
	{
		table->stopped = 1;
		printf("%ld %d died\n", now_ms() - table->epoch, rank);
	}
	pthread_mutex_unlock(&table->stop_mtx);
	pthread_mutex_unlock(&table->write_mtx);
}

static int	is_starving(t_table *table, int i, int *full)
{
	long	silence;

	pthread_mutex_lock(&table->state_mtx);
	silence = now_ms() - table->philos[i].last_bite;
	if (table->meals_goal >= 0 && table->philos[i].meals_done
		>= table->meals_goal)
		(*full)++;
	pthread_mutex_unlock(&table->state_mtx);
	return (silence > table->die_ms);
}

static int	scan_table(t_table *table)
{
	int	i;
	int	full;

	i = 0;
	full = 0;
	while (i < table->seats)
	{
		if (is_starving(table, i, &full))
		{
			announce_death(table, table->philos[i].rank);
			return (1);
		}
		i++;
	}
	if (table->meals_goal >= 0 && full == table->seats)
	{
		stop_sim(table);
		return (1);
	}
	return (0);
}

void	*watch_routine(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	while (!is_stopped(table))
	{
		if (scan_table(table))
			break ;
		usleep(POLL_US);
	}
	return (NULL);
}
