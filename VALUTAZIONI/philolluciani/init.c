/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lluciani <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 15:29:51 by lluciani          #+#    #+#             */
/*   Updated: 2025/06/23 15:32:05 by lluciani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_init(t_table *table)
{
	int	i;

	table->start_time = get_current_time();
	table->philo = safe_malloc(table->n_philo * sizeof(t_philo));
	i = 0;
	while (i < table->n_philo)
	{
		table->philo[i].id = i + 1;
		table->philo[i].meals_eaten = 0;
		table->philo[i].last_meal = table->start_time;
		table->philo[i].right_fork = (i + 1) % table->n_philo;
		table->philo[i].left_fork = i;
		table->philo[i].table = table;
		table->philo[i].eating = 0;
		i++;
	}
}

void	init_forks(t_table *table)
{
	int	i;

	i = 0;
	table->mutex.forks = safe_malloc(sizeof(pthread_mutex_t) * table->n_philo);
	while (i < table->n_philo)
	{
		pthread_mutex_init(&table->mutex.forks[i], NULL);
		i++;
	}
}

void	init_mutex(t_table *table)
{
	pthread_mutex_init(&table->mutex.print_log, NULL);
	pthread_mutex_init(&table->mutex.death_lock, NULL);
	pthread_mutex_init(&table->mutex.meals_eaten_lock, NULL);
	init_forks(table);
}

void	init_table(t_table *table, char **av)
{
	table->n_philo = ft_atol(av[1]);
	table->time_to_die = ft_atol(av[2]);
	table->time_to_eat = ft_atol(av[3]);
	table->time_to_sleep = ft_atol(av[4]);
	if (av[5])
		table->n_to_eat = ft_atol(av[5]);
	else
		table->n_to_eat = -1;
	table->someone_die = 0;
	init_mutex(table);
	philo_init(table);
	return ;
}

void	*safe_malloc(size_t size)
{
	void	*res;

	res = malloc(size);
	if (!res)
	{
		printf("Error with malloc");
		return (NULL);
	}
	return (res);
}
