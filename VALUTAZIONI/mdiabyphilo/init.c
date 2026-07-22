/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdiaby <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/22 17:54:01 by mdiaby            #+#    #+#             */
/*   Updated: 2026/07/22 17:54:04 by mdiaby           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_paras(t_table *table, int ac, char **av)
{
	table->number_of_philosophers = ft_atoi(av[1]);
	table->time_to_die = ft_atoi(av[2]);
	table->time_to_eat = ft_atoi(av[3]);
	table->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		table->number_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
	else
		table->number_of_times_each_philosopher_must_eat = -1;
	return (0);
}

static int	init_mutexes(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->number_of_philosophers)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
			return (1);
		table->forks_init++;
		i++;
	}
	if (pthread_mutex_init(&table->lock_printf, NULL) != 0)
		return (1);
	table->printf_init = 1;
	if (pthread_mutex_init(&table->lock_die, NULL) != 0)
		return (1);
	table->die_init = 1;
	if (pthread_mutex_init(&table->lock_meal, NULL) != 0)
		return (1);
	table->meal_init = 1;
	return (0);
}

static void	init_eaters(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->number_of_philosophers)
	{
		table->eaters[i].id = i + 1;
		table->eaters[i].n_time_eats = 0;
		table->eaters[i].last_meal = table->start_time;
		table->eaters[i].is_full = 0;
		table->eaters[i].table = table;
		i++;
	}
}

int	alloc_paras(t_table *table)
{
	table->forks = malloc(sizeof(pthread_mutex_t)
			* table->number_of_philosophers);
	if (table->forks == NULL)
		return (1);
	table->eaters = malloc(sizeof(t_philo) * table->number_of_philosophers);
	if (table->eaters == NULL)
		return (1);
	if (init_mutexes(table) != 0)
		return (1);
	table->start_time = get_time();
	init_eaters(table);
	return (0);
}

void	clean_up(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->forks_init)
	{
		pthread_mutex_destroy(&table->forks[i]);
		i++;
	}
	if (table->printf_init)
		pthread_mutex_destroy(&table->lock_printf);
	if (table->die_init)
		pthread_mutex_destroy(&table->lock_die);
	if (table->meal_init)
		pthread_mutex_destroy(&table->lock_meal);
	if (table->forks)
		free(table->forks);
	if (table->eaters)
		free(table->eaters);
}
