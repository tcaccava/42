/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: valucare <valucare@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 14:11:14 by valucare          #+#    #+#             */
/*   Updated: 2026/07/27 17:19:43 by valucare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	link_forks(t_table *table, t_philo *philo, int i)
{
	if (i == table->seats - 1)
	{
		philo->first_fork = &table->forks[0];
		philo->second_fork = &table->forks[i];
	}
	else
	{
		philo->first_fork = &table->forks[i];
		philo->second_fork = &table->forks[i + 1];
	}
}

static void	setup_philos(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->seats)
	{
		table->philos[i].rank = i + 1;
		table->philos[i].meals_done = 0;
		table->philos[i].last_bite = table->epoch;
		table->philos[i].table = table;
		link_forks(table, &table->philos[i], i);
		i++;
	}
}

static int	init_mutexes(t_table *table)
{
	int	i;

	if (pthread_mutex_init(&table->write_mtx, NULL))
		return (1);
	if (pthread_mutex_init(&table->state_mtx, NULL))
		return (1);
	if (pthread_mutex_init(&table->stop_mtx, NULL))
		return (1);
	i = 0;
	while (i < table->seats)
	{
		if (pthread_mutex_init(&table->forks[i], NULL))
			return (1);
		i++;
	}
	return (0);
}

int	setup_table(t_table *table, char **argv)
{
	table->seats = (int)parse_long(argv[1]);
	table->die_ms = parse_long(argv[2]);
	table->eat_ms = parse_long(argv[3]);
	table->sleep_ms = parse_long(argv[4]);
	table->meals_goal = -1;
	if (argv[5])
		table->meals_goal = (int)parse_long(argv[5]);
	table->stopped = 0;
	table->forks = malloc(sizeof(pthread_mutex_t) * table->seats);
	table->philos = malloc(sizeof(t_philo) * table->seats);
	if (!table->forks || !table->philos)
		return (free(table->forks), free(table->philos), 1);
	if (init_mutexes(table))
		return (free(table->forks), free(table->philos), 1);
	table->epoch = now_ms();
	setup_philos(table);
	return (0);
}

void	destroy_table(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->seats)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&table->write_mtx);
	pthread_mutex_destroy(&table->state_mtx);
	pthread_mutex_destroy(&table->stop_mtx);
	free(table->forks);
	free(table->philos);
}
