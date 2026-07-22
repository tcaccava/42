/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiaby <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 17:57:13 by mdiaby            #+#    #+#             */
/*   Updated: 2026/07/22 17:57:15 by mdiaby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	ft_usleep(long ms)
{
	long	start;

	start = get_time();
	while (get_time() - start < ms)
		usleep(500);
}

int	is_dead(t_table *table)
{
	int	d;

	pthread_mutex_lock(&table->lock_die);
	d = table->dead;
	pthread_mutex_unlock(&table->lock_die);
	return (d);
}

void	set_dead(t_table *table)
{
	pthread_mutex_lock(&table->lock_die);
	table->dead = 1;
	pthread_mutex_unlock(&table->lock_die);
}

void	ft_print(t_table *table, int id, char *str)
{
	long	timestamp;

	pthread_mutex_lock(&table->lock_printf);
	if (!is_dead(table))
	{
		timestamp = get_time() - table->start_time;
		printf("%ld %d %s\n", timestamp, id, str);
	}
	pthread_mutex_unlock(&table->lock_printf);
}
