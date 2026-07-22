/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiaby <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 17:55:56 by mdiaby            #+#    #+#             */
/*   Updated: 2026/07/22 17:56:06 by mdiaby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	must_stop(t_table *table, t_philo *philo)
{
	int	limit;

	limit = table->number_of_times_each_philosopher_must_eat;
	if (limit != -1 && philo->n_time_eats >= limit)
	{
		pthread_mutex_lock(&table->lock_meal);
		philo->is_full = 1;
		pthread_mutex_unlock(&table->lock_meal);
		pthread_mutex_lock(&table->lock_die);
		table->full_count++;
		pthread_mutex_unlock(&table->lock_die);
		return (1);
	}
	return (0);
}

static int	check_death(t_table *table, int i)
{
	long	since;
	int		full;

	pthread_mutex_lock(&table->lock_meal);
	full = table->eaters[i].is_full;
	since = get_time() - table->eaters[i].last_meal;
	pthread_mutex_unlock(&table->lock_meal);
	if (full)
		return (0);
	if (since > table->time_to_die)
	{
		ft_print(table, table->eaters[i].id, "died");
		set_dead(table);
		return (1);
	}
	return (0);
}

static int	all_full(t_table *table)
{
	int	full;

	pthread_mutex_lock(&table->lock_die);
	full = (table->number_of_times_each_philosopher_must_eat != -1
			&& table->full_count >= table->number_of_philosophers);
	pthread_mutex_unlock(&table->lock_die);
	return (full);
}

void	*monitor(void *arg)
{
	t_table	*table;
	int		i;

	table = (t_table *)arg;
	while (!is_dead(table))
	{
		i = 0;
		while (i < table->number_of_philosophers)
		{
			if (check_death(table, i))
				return (NULL);
			i++;
		}
		if (all_full(table))
		{
			set_dead(table);
			return (NULL);
		}
		ft_usleep(1);
	}
	return (NULL);
}
