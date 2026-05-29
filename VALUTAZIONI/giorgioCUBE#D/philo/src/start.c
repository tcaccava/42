/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsabatin <gsabatin@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:31:13 by gsabatin          #+#    #+#             */
/*   Updated: 2025/06/26 20:06:04 by gsabatin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	join_philos(t_table *table, t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < table->n_philos)
	{
		if (pthread_join(philos[i].thread, NULL) != 0)
			clean_exit(table, "Thread join failed.");
	}
	return (0);
}

static int	launch_philos(t_table *table, t_philo *philos)
{
	int	i;

	i = -1;
	while (++i < table->n_philos)
	{
		if (pthread_create(&philos[i].thread, NULL,
				philo_routine, (void *)&philos[i]) != 0)
			clean_exit(table, "Thread creation failed.");
	}
	return (1);
}

static bool	have_eaten_enough(t_table *table, t_philo *philos)
{
	int		i;
	bool	is_enough;

	if (table->must_eat_times == NO_MEALS_LIMIT)
		return (0);
	is_enough = true;
	i = -1;
	while (++i < table->n_philos)
	{
		pthread_mutex_lock(&philos[i].mutexes[MEALS]);
		if (philos[i].meals_eaten < table->must_eat_times)
		{
			pthread_mutex_unlock(&philos[i].mutexes[MEALS]);
			is_enough = false;
			break ;
		}
		pthread_mutex_unlock(&philos[i].mutexes[MEALS]);
	}
	if (is_enough)
	{
		pthread_mutex_lock(&table->simulation_end_mutex);
		table->simulation_end = true;
		pthread_mutex_unlock(&table->simulation_end_mutex);
	}
	return (is_enough);
}

static bool	is_someone_dead(t_table *table, t_philo *philos)
{
	int			i;
	long long	time_nomeal;

	i = -1;
	while (++i < table->n_philos)
	{
		pthread_mutex_lock(&table->simulation_end_mutex);
		time_nomeal = time_diff_philo(&philos[i], get_time_ms(NULL));
		if (time_nomeal >= 0 && time_nomeal > table->time_to_die)
		{
			table->simulation_end = true;
			pthread_mutex_unlock(&table->simulation_end_mutex);
			print_state(DIE, &philos[i]);
			return (true);
		}
		pthread_mutex_unlock(&table->simulation_end_mutex);
	}
	return (false);
}

int	start_simulation(t_table *table, t_philo *philos)
{
	int	i;

	table->start_time = get_time_ms(table);
	i = -1;
	while (++i < table->n_philos)
		philos[i].last_meal_time = table->start_time;
	launch_philos(table, philos);
	while (1)
	{
		if (is_someone_dead(table, table->philos))
			break ;
		else if (have_eaten_enough(table, table->philos))
			break ;
	}
	join_philos(table, philos);
	return (0);
}
