/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gsabatin <gsabatin@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 17:18:51 by gsabatin          #+#    #+#             */
/*   Updated: 2025/06/26 19:22:17 by gsabatin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

static int	init_philos(t_table *table)
{
	int	i;

	table->philos = ft_calloc(table->n_philos, sizeof(t_philo));
	if (!table->philos)
		return (1);
	i = -1;
	while (++i < table->n_philos)
	{
		(table->philos)[i].id = i + 1;
		(table->philos)[i].last_meal_time = 0;
		(table->philos)[i].meals_eaten = 0;
		(table->philos)[i].left_fork_id = i;
		(table->philos)[i].right_fork_id = (i + 1) % table->n_philos;
		(table->philos)[i].table = table;
		if (pthread_mutex_init(&(table->philos)[i].mutexes[MEALS], NULL) != 0)
			return (0);
		if (pthread_mutex_init(&(table->philos)[i].mutexes[TIME], NULL) != 0)
			return (0);
	}
	return (0);
}

static int	init_forks(t_table *table)
{
	int	i;

	table->forks = ft_calloc(table->n_philos, sizeof(t_fork));
	if (!table->forks)
		return (1);
	i = -1;
	while (++i < table->n_philos)
	{
		table->forks[i].id = i;
		if (pthread_mutex_init(&table->forks[i].mutex, NULL) != 0)
			return (1);
	}
	return (0);
}

int	init_table(t_table *table)
{
	table->simulation_end = false;
	if (init_forks(table) != 0)
		clean_exit(table, "Fork allocation failed");
	if (init_philos(table) != 0)
		clean_exit(table, "Philo allocation failed");
	if (pthread_mutex_init(&table->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&table->simulation_end_mutex, NULL) != 0)
		return (1);
	return (0);
}
